/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright 2020 Tintri by DDN, Inc. All rights reserved.
 */

/*
 * Sid manipulation (stubs).
 */

#include <sys/atomic.h>
#include <sys/avl.h>
#include <sys/cmn_err.h>
#include <sys/kmem.h>
#include <sys/mutex.h>
#include <sys/sid.h>
#include <sys/sysmacros.h>
#include <sys/systm.h>

#ifdef _KERNEL
#include <sys/kidmap.h>
#endif

#include <sys/idmap.h>
#include <util/qsort.h>

static kmutex_t sid_lock;
static avl_tree_t sid_tree;
static boolean_t sid_inited = B_FALSE;

static ksiddomain_t
*ksid_enterdomain(const char *dom)
{
	size_t len = strlen(dom) + 1;
	ksiddomain_t *res;

	ASSERT(MUTEX_HELD(&sid_lock));
	res = kmem_alloc(sizeof (ksiddomain_t), KM_SLEEP);
	res->kd_len = (uint_t)len;
	res->kd_name = kmem_alloc(len, KM_SLEEP);
	bcopy(dom, res->kd_name, len);

	res->kd_ref = 1;

	avl_add(&sid_tree, res);

	return (res);
}

void
ksid_hold(ksid_t *ks)
{
	if (ks->ks_domain != NULL)
		ksiddomain_hold(ks->ks_domain);
}

void
ksid_rele(ksid_t *ks)
{
	if (ks->ks_domain != NULL)
		ksiddomain_rele(ks->ks_domain);
}

void
ksiddomain_hold(ksiddomain_t *kd)
{
	atomic_inc_32(&kd->kd_ref);
}

void
ksiddomain_rele(ksiddomain_t *kd)
{
	if (atomic_dec_32_nv(&kd->kd_ref) == 0) {
		/*
		 * The kd reference can only be incremented from 0 when
		 * the sid_lock is held; so we lock and then check need to
		 * check for 0 again.
		 */
		mutex_enter(&sid_lock);
		if (kd->kd_ref == 0) {
			avl_remove(&sid_tree, kd);
			kmem_free(kd->kd_name, kd->kd_len);
			kmem_free(kd, sizeof (*kd));
		}
		mutex_exit(&sid_lock);
	}
}

void
ksidlist_hold(ksidlist_t *ksl)
{
	atomic_inc_32(&ksl->ksl_ref);
}

void
ksidlist_rele(ksidlist_t *ksl)
{
	if (atomic_dec_32_nv(&ksl->ksl_ref) == 0) {
		int i;

		if (ksl->ksl_sorted != NULL)
			kmem_free(ksl->ksl_sorted,
			    ksl->ksl_nsid * sizeof (ksid_t *));
		for (i = 0; i < ksl->ksl_nsid; i++)
			ksid_rele(&ksl->ksl_sids[i]);

		kmem_free(ksl, KSIDLIST_MEM(ksl->ksl_nsid));
	}
}

/*
 * Linear search is more efficient for 'small' arrays.
 * What's considered 'small' varies by system.
 * supgroupmember() uses 16; we make ours a variable for testing.
 */
int ksl_bin_search_cutoff = 16;

boolean_t
ksidlist_has_sid(ksidlist_t *ksl, const char *domain, uint32_t rid)
{
	int64_t hi, lo, m;
	int cmp;
	ksid_t *sids = ksl->ksl_sids; /* sorted by SID */

	lo = 0;
	hi = ksl->ksl_nsid - 1;

	if (hi < ksl_bin_search_cutoff) {
		for (; lo <= hi; lo++) {
			if (rid == sids[lo].ks_rid &&
			    strcmp(domain, ksid_getdomain(&sids[lo])) == 0)
				return (B_TRUE);
		}
		return (B_FALSE);
	}

	do {
		/* This is an overflow-safe version of m = (lo + hi) / 2 */
		m = (int64_t)((uint64_t)(lo + hi) >> 1);

		cmp = AVL_CMP(rid, sids[m].ks_rid);
		if (cmp == 0)
			cmp = strcmp(domain, ksid_getdomain(&sids[m]));

		if (cmp > 0)
			lo = m + 1;
		else if (cmp < 0)
			hi = m - 1;
		else
			return (B_TRUE);

	} while (lo <= hi);

	return (B_FALSE);
}

boolean_t
ksidlist_has_pid(ksidlist_t *ksl, uint32_t pid)
{
	int64_t hi, lo, m;
	int cmp;
	ksid_t **sidsp = ksl->ksl_sorted; /* sorted by posix ID */

	lo = 0;
	hi = ksl->ksl_nsid - 1;

	if (hi < ksl_bin_search_cutoff) {
		for (; lo <= hi; lo++) {
			if (pid == ksl->ksl_sids[lo].ks_id)
				return (B_TRUE);
		}
		return (B_FALSE);
	}

	do {
		/* This is an overflow-safe version of m = (lo + hi) / 2 */
		m = (int64_t)((uint64_t)(lo + hi) >> 1);

		cmp = AVL_CMP(pid, sidsp[m]->ks_id);

		if (cmp > 0)
			lo = m + 1;
		else if (cmp < 0)
			hi = m - 1;
		else
			return (B_TRUE);

	} while (lo <= hi);

	return (B_FALSE);
}

static int
ksid_cmp(const void *a, const void *b)
{
	const ksiddomain_t *ap = a;
	const ksiddomain_t *bp = b;
	int res;

	res = strcmp(ap->kd_name, bp->kd_name);

	return (AVL_ISIGN(res));
}

/*
 * Lookup the named domain in the AVL tree.
 * If no entry is found, add the domain to the AVL tree.
 * The domain is returned held and needs to be released
 * when done.
 */
ksiddomain_t
*ksid_lookupdomain(const char *dom)
{
	ksiddomain_t *res;
	ksiddomain_t tmpl;

	mutex_enter(&sid_lock);

	if (!sid_inited) {
		avl_create(&sid_tree, ksid_cmp, sizeof (ksiddomain_t),
		    offsetof(ksiddomain_t, kd_link));

		res = ksid_enterdomain(dom);
		sid_inited = B_TRUE;
		mutex_exit(&sid_lock);
		return (res);
	}

	tmpl.kd_name = (char *)dom;

	res = avl_find(&sid_tree, &tmpl, NULL);
	if (res == NULL) {
		res = ksid_enterdomain(dom);
	} else {
		ksiddomain_hold(res);
	}

	mutex_exit(&sid_lock);
	return (res);
}

const char *
ksid_getdomain(ksid_t *ks)
{
	return (ks->ks_domain->kd_name);
}

uint_t
ksid_getrid(ksid_t *ks)
{
	return (ks->ks_rid);
}

uid_t
ksid_getid(ksid_t *ks)
{
	return (ks->ks_id);
}

#ifdef _KERNEL
int
ksid_lookupbyuid(zone_t *zone, uid_t id, ksid_t *res)
{
	const char *sid_prefix;

	if (kidmap_getsidbyuid(zone, id, &sid_prefix, &res->ks_rid)
	    != IDMAP_SUCCESS)
		return (-1);

	res->ks_domain = ksid_lookupdomain(sid_prefix);

	res->ks_id = id;

	return (0);
}

int
ksid_lookupbygid(zone_t *zone, gid_t id, ksid_t *res)
{
	const char *sid_prefix;

	if (kidmap_getsidbygid(zone, id, &sid_prefix, &res->ks_rid)
	    != IDMAP_SUCCESS)
		return (-1);

	res->ks_domain = ksid_lookupdomain(sid_prefix);

	res->ks_id = id;

	return (0);
}
#endif

credsid_t *
kcrsid_alloc(void)
{
	credsid_t *kcr = kmem_zalloc(sizeof (*kcr), KM_SLEEP);
	kcr->kr_ref = 1;
	return (kcr);
}

/*
 * Returns a credsid_t with a refcount of 1.
 */
static credsid_t *
kcrsid_dup(credsid_t *org)
{
	credsid_t *new;
	ksid_index_t ki;

	if (org == NULL)
		return (kcrsid_alloc());
	if (org->kr_ref == 1)
		return (org);
	new = kcrsid_alloc();

	/* Copy, then update reference counts */
	*new = *org;
	new->kr_ref = 1;
	for (ki = 0; ki < KSID_COUNT; ki++)
		ksid_hold(&new->kr_sidx[ki]);

	if (new->kr_sidlist != NULL)
		ksidlist_hold(new->kr_sidlist);

	kcrsid_rele(org);
	return (new);
}

void
kcrsid_hold(credsid_t *kcr)
{
	atomic_inc_32(&kcr->kr_ref);
}

void
kcrsid_rele(credsid_t *kcr)
{
	if (atomic_dec_32_nv(&kcr->kr_ref) == 0) {
		ksid_index_t i;

		for (i = 0; i < KSID_COUNT; i++)
			ksid_rele(&kcr->kr_sidx[i]);

		if (kcr->kr_sidlist != NULL)
			ksidlist_rele(kcr->kr_sidlist);

		kmem_free(kcr, sizeof (*kcr));
	}
}

/*
 * Copy the SID credential into a previously allocated piece of memory.
 */
void
kcrsidcopy_to(const credsid_t *okcr, credsid_t *nkcr)
{
	int i;

	ASSERT(nkcr->kr_ref == 1);

	if (okcr == NULL)
		return;
	*nkcr = *okcr;
	for (i = 0; i < KSID_COUNT; i++)
		ksid_hold(&nkcr->kr_sidx[i]);
	if (nkcr->kr_sidlist != NULL)
		ksidlist_hold(nkcr->kr_sidlist);
	nkcr->kr_ref = 1;
}

static int
kcrsid_sidcount(const credsid_t *kcr)
{
	int cnt = 0;
	int i;

	if (kcr == NULL)
		return (0);

	for (i = 0; i < KSID_COUNT; i++)
		if (kcr->kr_sidx[i].ks_domain != NULL)
			cnt++;

	if (kcr->kr_sidlist != NULL)
		cnt += kcr->kr_sidlist->ksl_nsid;
	return (cnt);
}

/*
 * Argument needs to be a ksid_t with a properly held ks_domain reference.
 */
credsid_t *
kcrsid_setsid(credsid_t *okcr, ksid_t *ksp, ksid_index_t i)
{
	int ocnt = kcrsid_sidcount(okcr);
	credsid_t *nkcr;

	/*
	 * Unset the particular ksid; if there are no other SIDs or if this
	 * is the last SID, remove the auxilary data structure.
	 */
	if (ksp == NULL) {
		if (ocnt == 0 ||
		    (ocnt == 1 && okcr->kr_sidx[i].ks_domain != NULL)) {
			if (okcr != NULL)
				kcrsid_rele(okcr);
			return (NULL);
		}
	}
	nkcr = kcrsid_dup(okcr);
	ksid_rele(&nkcr->kr_sidx[i]);
	if (ksp == NULL)
		bzero(&nkcr->kr_sidx[i], sizeof (ksid_t));
	else
		nkcr->kr_sidx[i] = *ksp;

	return (nkcr);
}

static int
ksid_sid_cmp(const void *arg1, const void *arg2)
{
	ksid_t *sid1 = (ksid_t *)arg1;
	ksid_t *sid2 = (ksid_t *)arg2;
	int cmp = AVL_CMP(sid1->ks_rid, sid2->ks_rid);

	if (cmp == 0)
		cmp = AVL_ISIGN(strcmp(ksid_getdomain(sid1),
		    ksid_getdomain(sid2)));

	return (cmp);
}

static int
ksid_id_cmp(const void *arg1, const void *arg2)
{
	ksid_t *sid1 = *(ksid_t **)arg1;
	ksid_t *sid2 = *(ksid_t **)arg2;

	return (AVL_CMP(sid1->ks_id, sid2->ks_id));
}

/*
 * Argument needs to be a ksidlist_t with properly held ks_domain references
 * and a reference count taking the new reference into account.
 */
credsid_t *
kcrsid_setsidlist(credsid_t *okcr, ksidlist_t *ksl)
{
	int ocnt = kcrsid_sidcount(okcr);
	credsid_t *nkcr;
	int i;

	/*
	 * Unset the sidlist; if there are no further SIDs, remove the
	 * auxilary data structure.
	 */
	if (ksl == NULL) {
		if (ocnt == 0 || (okcr->kr_sidlist != NULL &&
		    ocnt == okcr->kr_sidlist->ksl_nsid)) {
			if (okcr != NULL)
				kcrsid_rele(okcr);
			return (NULL);
		}
	}
	nkcr = kcrsid_dup(okcr);
	if (nkcr->kr_sidlist != NULL)
		ksidlist_rele(nkcr->kr_sidlist);

	/* sort the lists so that we can do binary search */
	nkcr->kr_sidlist = ksl;

	if (ksl->ksl_sorted == NULL) {
		qsort(ksl->ksl_sids, ksl->ksl_nsid, sizeof (ksid_t),
		    ksid_sid_cmp);

		ksl->ksl_sorted = kmem_alloc(ksl->ksl_nsid * sizeof (ksid_t *),
		    KM_SLEEP);
		for (i = 0; i < ksl->ksl_nsid; i++)
			ksl->ksl_sorted[i] = &ksl->ksl_sids[i];
		qsort(ksl->ksl_sorted, ksl->ksl_nsid, sizeof (ksid_t *),
		    ksid_id_cmp);
	}

	return (nkcr);
}

ksidlist_t *
kcrsid_gidstosids(zone_t *zone, int ngrp, gid_t *grp)
{
	int i;
	ksidlist_t *list;
	int cnt;

	if (ngrp == 0)
		return (NULL);

	cnt = 0;
	list = kmem_zalloc(KSIDLIST_MEM(ngrp), KM_SLEEP);

	list->ksl_nsid = ngrp;
	list->ksl_ref = 1;

	for (i = 0; i < ngrp; i++) {
		if (grp[i] > MAXUID) {
			list->ksl_neid++;
#ifdef _KERNEL
			if (ksid_lookupbygid(zone,
			    grp[i], &list->ksl_sids[i]) != 0) {
				while (--i >= 0)
					ksid_rele(&list->ksl_sids[i]);
				cnt = 0;
				break;
			}
#endif
			cnt++;
		} else {
			list->ksl_sids[i].ks_id = grp[i];
		}
	}
	if (cnt == 0) {
		kmem_free(list, KSIDLIST_MEM(ngrp));
		return (NULL);
	}
	return (list);
}
