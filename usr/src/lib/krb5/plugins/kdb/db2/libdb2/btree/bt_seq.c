/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright (C) 2002 by the Massachusetts Institute of Technology.
 * All rights reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */

/*-
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Olson.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)bt_seq.c	8.9 (Berkeley) 6/20/95";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db-int.h"
#include "btree.h"

static int __bt_first __P((BTREE *, const DBT *, EPG *, int *));
static int __bt_seqadv __P((BTREE *, EPG *, int));
static int __bt_seqset __P((BTREE *, EPG *, DBT *, int));

/*
 * Sequential scan support.
 *
 * The tree can be scanned sequentially, starting from either end of the
 * tree or from any specific key.  A scan request before any scanning is
 * done is initialized as starting from the least node.
 */

/*
 * __bt_seq --
 *	Btree sequential scan interface.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *	key:	key for positioning and return value
 *	data:	data return value
 *	flags:	R_CURSOR, R_FIRST, R_LAST, R_NEXT, R_PREV.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
int
__bt_seq(dbp, key, data, flags)
	const DB *dbp;
	DBT *key, *data;
	u_int flags;
{
	BTREE *t;
	EPG e;
	int status;

	t = dbp->internal;

	/* Toss any page pinned across calls. */
	if (t->bt_pinned != NULL) {
		mpool_put(t->bt_mp, t->bt_pinned, 0);
		t->bt_pinned = NULL;
	}

	/*
	 * If scan unitialized as yet, or starting at a specific record, set
	 * the scan to a specific key.  Both __bt_seqset and __bt_seqadv pin
	 * the page the cursor references if they're successful.
	 */
	switch (flags) {
	case R_NEXT:
	case R_PREV:
		if (F_ISSET(&t->bt_cursor, CURS_INIT)) {
			status = __bt_seqadv(t, &e, flags);
			break;
		}
		/* FALLTHROUGH */
	case R_FIRST:
	case R_LAST:
	case R_CURSOR:
		status = __bt_seqset(t, &e, key, flags);
		break;
	default:
		errno = EINVAL;
		return (RET_ERROR);
	}

	if (status == RET_SUCCESS) {
		__bt_setcur(t, e.page->pgno, e.index);

		status =
		    __bt_ret(t, &e, key, &t->bt_rkey, data, &t->bt_rdata, 0);

		/*
		 * If the user is doing concurrent access, we copied the
		 * key/data, toss the page.
		 */
		if (F_ISSET(t, B_DB_LOCK))
			mpool_put(t->bt_mp, e.page, 0);
		else
			t->bt_pinned = e.page;
	}
	return (status);
}

/*
 * __bt_seqset --
 *	Set the sequential scan to a specific key.
 *
 * Parameters:
 *	t:	tree
 *	ep:	storage for returned key
 *	key:	key for initial scan position
 *	flags:	R_CURSOR, R_FIRST, R_LAST, R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the cursor references.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
static int
__bt_seqset(t, ep, key, flags)
	BTREE *t;
	EPG *ep;
	DBT *key;
	int flags;
{
	PAGE *h;
	db_pgno_t pg;
	int exact;

	/*
	 * Find the first, last or specific key in the tree and point the
	 * cursor at it.  The cursor may not be moved until a new key has
	 * been found.
	 */
	switch (flags) {
	case R_CURSOR:				/* Keyed scan. */
		/*
		 * Find the first instance of the key or the smallest key
		 * which is greater than or equal to the specified key.
		 */
		if (key->data == NULL || key->size == 0) {
			errno = EINVAL;
			return (RET_ERROR);
		}
		return (__bt_first(t, key, ep, &exact));
	case R_FIRST:				/* First record. */
	case R_NEXT:
		/* Walk down the left-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);

			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, 0)->pgno;
			mpool_put(t->bt_mp, h, 0);
		}
		ep->page = h;
		ep->index = 0;
		break;
	case R_LAST:				/* Last record. */
	case R_PREV:
		/* Walk down the right-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);

			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, NEXTINDEX(h) - 1)->pgno;
			mpool_put(t->bt_mp, h, 0);
		}

		ep->page = h;
		ep->index = NEXTINDEX(h) - 1;
		break;
	}
	return (RET_SUCCESS);
}

/*
 * __bt_seqadvance --
 *	Advance the sequential scan.
 *
 * Parameters:
 *	t:	tree
 *	flags:	R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the new key/data record is on.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
static int
__bt_seqadv(t, ep, flags)
	BTREE *t;
	EPG *ep;
	int flags;
{
	CURSOR *c;
	PAGE *h;
	indx_t idx;
	db_pgno_t pg;
	int exact, rval;

	/*
	 * There are a couple of states that we can be in.  The cursor has
	 * been initialized by the time we get here, but that's all we know.
	 */
	c = &t->bt_cursor;

	/*
	 * The cursor was deleted and there weren't any duplicate records,
	 * so the cursor's key was saved.  Find out where that key would
	 * be in the current tree.  If the returned key is an exact match,
	 * it means that a key/data pair was inserted into the tree after
	 * the delete.  We could reasonably return the key, but the problem
	 * is that this is the access pattern we'll see if the user is
	 * doing seq(..., R_NEXT)/put(..., 0) pairs, i.e. the put deletes
	 * the cursor record and then replaces it, so the cursor was saved,
	 * and we'll simply return the same "new" record until the user
	 * notices and doesn't do a put() of it.  Since the key is an exact
	 * match, we could as easily put the new record before the cursor,
	 * and we've made no guarantee to return it.  So, move forward or
	 * back a record if it's an exact match.
	 *
	 * XXX
	 * In the current implementation, put's to the cursor are done with
	 * delete/add pairs.  This has two consequences.  First, it means
	 * that seq(..., R_NEXT)/put(..., R_CURSOR) pairs are going to exhibit
	 * the same behavior as above.  Second, you can return the same key
	 * twice if you have duplicate records.  The scenario is that the
	 * cursor record is deleted, moving the cursor forward or backward
	 * to a duplicate.  The add then inserts the new record at a location
	 * ahead of the cursor because duplicates aren't sorted in any way,
	 * and the new record is later returned.  This has to be fixed at some
	 * point.
	 */
	if (F_ISSET(c, CURS_ACQUIRE)) {
		if ((rval = __bt_first(t, &c->key, ep, &exact)) == RET_ERROR)
			return (RET_ERROR);
		if (!exact)
			return (rval);
		/*
		 * XXX
		 * Kluge -- get, release, get the page.
		 */
		c->pg.pgno = ep->page->pgno;
		c->pg.index = ep->index;
		mpool_put(t->bt_mp, ep->page, 0);
	}

	/* Get the page referenced by the cursor. */
	if ((h = mpool_get(t->bt_mp, c->pg.pgno, 0)) == NULL)
		return (RET_ERROR);

	/*
 	 * Find the next/previous record in the tree and point the cursor at
	 * it.  The cursor may not be moved until a new key has been found.
	 */
	switch (flags) {
	case R_NEXT:			/* Next record. */
		/*
		 * The cursor was deleted in duplicate records, and moved
		 * forward to a record that has yet to be returned.  Clear
		 * that flag, and return the record.
		 */
		if (F_ISSET(c, CURS_AFTER))
			goto usecurrent;
		idx = c->pg.index;
		if (++idx == NEXTINDEX(h)) {
			pg = h->nextpg;
			mpool_put(t->bt_mp, h, 0);
			if (pg == P_INVALID)
				return (RET_SPECIAL);
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
			idx = 0;
		}
		break;
	case R_PREV:			/* Previous record. */
		/*
		 * The cursor was deleted in duplicate records, and moved
		 * backward to a record that has yet to be returned.  Clear
		 * that flag, and return the record.
		 */
		if (F_ISSET(c, CURS_BEFORE)) {
usecurrent:		F_CLR(c, CURS_AFTER | CURS_BEFORE);
			ep->page = h;
			ep->index = c->pg.index;
			return (RET_SUCCESS);
		}
		idx = c->pg.index;
		if (idx == 0) {
			pg = h->prevpg;
			mpool_put(t->bt_mp, h, 0);
			if (pg == P_INVALID)
				return (RET_SPECIAL);
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
			idx = NEXTINDEX(h) - 1;
		} else
			--idx;
		break;
	}

	ep->page = h;
	ep->index = idx;
	return (RET_SUCCESS);
}

/*
 * __bt_first --
 *	Find the first entry.
 *
 * Parameters:
 *	t:	the tree
 *    key:	the key
 *  erval:	return EPG
 * exactp:	pointer to exact match flag
 *
 * Returns:
 *	The first entry in the tree greater than or equal to key,
 *	or RET_SPECIAL if no such key exists.
 */
static int
__bt_first(t, key, erval, exactp)
	BTREE *t;
	const DBT *key;
	EPG *erval;
	int *exactp;
{
	PAGE *h;
	EPG *ep, save;
	db_pgno_t pg;

	/*
	 * Find any matching record; __bt_search pins the page.
	 *
	 * If it's an exact match and duplicates are possible, walk backwards
	 * in the tree until we find the first one.  Otherwise, make sure it's
	 * a valid key (__bt_search may return an index just past the end of a
	 * page) and return it.
	 */
	if ((ep = __bt_search(t, key, exactp)) == NULL)
		return (RET_SPECIAL);
	if (*exactp) {
		if (F_ISSET(t, B_NODUPS)) {
			*erval = *ep;
			return (RET_SUCCESS);
		}

		/*
		 * Walk backwards, as long as the entry matches and there are
		 * keys left in the tree.  Save a copy of each match in case
		 * we go too far.
		 */
		save = *ep;
		h = ep->page;
		do {
			if (save.page->pgno != ep->page->pgno) {
				mpool_put(t->bt_mp, save.page, 0);
				save = *ep;
			} else
				save.index = ep->index;

			/*
			 * Don't unpin the page the last (or original) match
			 * was on, but make sure it's unpinned if an error
			 * occurs.
			 */
			if (ep->index == 0) {
				if (h->prevpg == P_INVALID)
					break;
				if (h->pgno != save.page->pgno)
					mpool_put(t->bt_mp, h, 0);
				if ((h = mpool_get(t->bt_mp,
				    h->prevpg, 0)) == NULL) {
					if (h->pgno == save.page->pgno)
						mpool_put(t->bt_mp,
						    save.page, 0);
					return (RET_ERROR);
				}
				ep->page = h;
				ep->index = NEXTINDEX(h);
			}
			--ep->index;
		} while (__bt_cmp(t, key, ep) == 0);

		/*
		 * Reach here with the last page that was looked at pinned,
		 * which may or may not be the same as the last (or original)
		 * match page.  If it's not useful, release it.
		 */
		if (h->pgno != save.page->pgno)
			mpool_put(t->bt_mp, h, 0);

		*erval = save;
		return (RET_SUCCESS);
	}

	/* If at the end of a page, find the next entry. */
	if (ep->index == NEXTINDEX(ep->page)) {
		h = ep->page;
		pg = h->nextpg;
		mpool_put(t->bt_mp, h, 0);
		if (pg == P_INVALID)
			return (RET_SPECIAL);
		if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
			return (RET_ERROR);
		ep->index = 0;
		ep->page = h;
	}
	*erval = *ep;
	return (RET_SUCCESS);
}

/*
 * __bt_setcur --
 *	Set the cursor to an entry in the tree.
 *
 * Parameters:
 *	t:	the tree
 *   pgno:	page number
 *  index:	page index
 */
void
__bt_setcur(t, pgno, idx)
	BTREE *t;
	db_pgno_t pgno;
	u_int idx;
{
	/* Lose any already deleted key. */
	if (t->bt_cursor.key.data != NULL) {
		free(t->bt_cursor.key.data);
		t->bt_cursor.key.size = 0;
		t->bt_cursor.key.data = NULL;
	}
	F_CLR(&t->bt_cursor, CURS_ACQUIRE | CURS_AFTER | CURS_BEFORE);

	/* Update the cursor. */
	t->bt_cursor.pg.pgno = pgno;
	t->bt_cursor.pg.index = idx;
	F_SET(&t->bt_cursor, CURS_INIT);
}

/* Recursive descent cursor. */
typedef struct rcursor_ {
	CURSOR	cursor;
	size_t	ssize;
	EPGNO	*stack;
	EPGNO	*sp;
} RCURSOR;
#define RCURSOR_MINSS	64

static int	 bt_rcinit(void **);
static void	 bt_rcdestroy(void **);
static int	 bt_rcpush(RCURSOR *, db_pgno_t, u_int);
static EPGNO	*bt_rcpop(RCURSOR *);
static void	 bt_rcclr(RCURSOR *);
static int	 bt_rcgrowstk(RCURSOR *);
static int	 bt_rseqset(BTREE *, EPG *, DBT *, RCURSOR *, int);
static int	 bt_rseqadv(BTREE *, EPG *, RCURSOR *, int);

static int
bt_rcinit(curs)
	void **curs;
{
	RCURSOR *rc;

	rc = *curs = malloc(sizeof(RCURSOR));
	if (rc == NULL) {
		errno = ENOMEM;
		return RET_ERROR;
	}
	memset(rc, 0, sizeof(*rc));

	rc->ssize = RCURSOR_MINSS;
	rc->stack = malloc(rc->ssize * sizeof(EPGNO));
	if (rc->stack == NULL) {
		free(rc);
		errno = ENOMEM;
		return RET_ERROR;
	}
	bt_rcclr(rc);
	return RET_SUCCESS;
}

static void
bt_rcdestroy(curs)
	void **curs;
{
	RCURSOR *rc;

	rc = *curs;
	free(rc->stack);
	free(rc);
	*curs = NULL;
}

static int
bt_rcpush(rc, p, i)
	RCURSOR *rc;
	db_pgno_t p;
	u_int i;
{
	int status;

	rc->sp->pgno = p;
	rc->sp->index = i;
	if (++rc->sp > rc->stack + rc->ssize) {
		status = bt_rcgrowstk(rc);
		if (status != RET_SUCCESS)
			return status;
	}
	return RET_SUCCESS;
}

static EPGNO *
bt_rcpop(rc)
	RCURSOR *rc;
{
	return (rc->sp == rc->stack) ? NULL : --rc->sp;
}

static void
bt_rcclr(rc)
	RCURSOR *rc;
{
	rc->sp = rc->stack;
}

static int
bt_rcgrowstk(rc)
	RCURSOR *rc;
{
	size_t osize;
	EPGNO *e;

	osize = rc->ssize;
	rc->ssize *= 2;
	e = realloc(rc->stack, rc->ssize * sizeof(EPGNO));
	if (e == NULL) {
		rc->ssize = osize;
		errno = ENOMEM;
		return RET_ERROR;
	}
	rc->stack = e;
	return RET_SUCCESS;
}

/*
 * bt_rseq --
 *	Like __bt_seq but does recursive descent tree traversal
 *	instead of using the prev/next pointers.
 */
int
bt_rseq(dbp, key, data, curs, flags)
	const DB *dbp;
	DBT *key, *data;
	void **curs;
	u_int flags;
{
	RCURSOR *rc;
	BTREE *t;
	EPG e;
	int status;

	t = dbp->internal;

	/* Toss any page pinned across calls. */
	if (t->bt_pinned != NULL) {
		mpool_put(t->bt_mp, t->bt_pinned, 0);
		t->bt_pinned = NULL;
	}

	if (curs == NULL) {
		errno = EINVAL;
		return RET_ERROR;
	}
	if (*curs == NULL) {
		status = bt_rcinit(curs);
		if (status != RET_SUCCESS)
			return RET_ERROR;
	}
	rc = *curs;

	/*
	 * If scan unitialized as yet, or starting at a specific record, set
	 * the scan to a specific key.  Both bt_rseqset and bt_rseqadv pin
	 * the page the cursor references if they're successful.
	 */
	switch (flags) {
	case R_NEXT:
	case R_PREV:
		if (F_ISSET(&rc->cursor, CURS_INIT)) {
			status = bt_rseqadv(t, &e, rc, flags);
			break;
		}
		/* FALLTHROUGH */
	case R_FIRST:
	case R_LAST:
	case R_CURSOR:
		status = bt_rseqset(t, &e, key, rc, flags);
		break;
	default:
		errno = EINVAL;
		return (RET_ERROR);
	}

	if (status == RET_SUCCESS) {
		status =
		    __bt_ret(t, &e, key, &t->bt_rkey, data, &t->bt_rdata, 0);

		/*
		 * If the user is doing concurrent access, we copied the
		 * key/data, toss the page.
		 */
		if (F_ISSET(t, B_DB_LOCK))
			mpool_put(t->bt_mp, e.page, 0);
		else
			t->bt_pinned = e.page;
	} else if (status == RET_SPECIAL)
		bt_rcdestroy(curs);
	return (status);
}

/*
 * bt_rseqset --
 *	Set the sequential scan to a specific key.
 *
 * Parameters:
 *	t:	tree
 *	ep:	storage for returned key
 *	key:	key for initial scan position
 *	rc:	recursion cursor
 *	flags:	R_CURSOR, R_FIRST, R_LAST, R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the cursor references.
 *	Updates rc's stack and cursor.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
static int
bt_rseqset(t, ep, key, rc, flags)
	BTREE *t;
	EPG *ep;
	DBT *key;
	RCURSOR *rc;
	int flags;
{
	PAGE *h;
	db_pgno_t pg;
	int status;

	/*
	 * Find the first, last or specific key in the tree and point the
	 * cursor at it.  The cursor may not be moved until a new key has
	 * been found.
	 */
	switch (flags) {
	case R_CURSOR:		/* Not implemented. */
		errno = EINVAL;
		return RET_ERROR;
	case R_FIRST:				/* First record. */
	case R_NEXT:
		bt_rcclr(rc);
		/* Walk down the left-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);

			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, 0)->pgno;
			status = bt_rcpush(rc, h->pgno, 0);
			mpool_put(t->bt_mp, h, 0);
			if (status != RET_SUCCESS)
				return status;
		}
		ep->page = h;
		ep->index = 0;
		break;
	case R_LAST:				/* Last record. */
	case R_PREV:
		bt_rcclr(rc);
		/* Walk down the right-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);

			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, NEXTINDEX(h) - 1)->pgno;
			status = bt_rcpush(rc, h->pgno, NEXTINDEX(h) - 1);
			mpool_put(t->bt_mp, h, 0);
			if (status != RET_SUCCESS)
				return status;
		}
		ep->page = h;
		ep->index = NEXTINDEX(h) - 1;
		break;
	}
	rc->cursor.pg.pgno = ep->page->pgno;
	rc->cursor.pg.index = ep->index;
	F_CLR(&rc->cursor, CURS_ACQUIRE | CURS_AFTER | CURS_BEFORE);
	F_SET(&rc->cursor, CURS_INIT);
	return (RET_SUCCESS);
}

/*
 * bt_rseqadvance --
 *	Advance the sequential scan.
 *
 * Parameters:
 *	t:	tree
 *	ep:	return page
 *	rc:	recursion cursor
 *	flags:	R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the new key/data record is on.
 *	Updates rc's stack and cursor.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
static int
bt_rseqadv(t, ep, rc, flags)
	BTREE *t;
	EPG *ep;
	RCURSOR *rc;
	int flags;
{
	CURSOR *c;
	PAGE *h;
	indx_t idx;
	db_pgno_t pg;
	int status;
	EPGNO *e;

	/*
	 * There are a couple of states that we can be in.  The cursor has
	 * been initialized by the time we get here, but that's all we know.
	 */
	c = &rc->cursor;

	/* Get the page referenced by the cursor. */
	if ((h = mpool_get(t->bt_mp, c->pg.pgno, 0)) == NULL)
		return (RET_ERROR);

	/*
 	 * Find the next/previous record in the tree and point the cursor at
	 * it.  The cursor may not be moved until a new key has been found.
	 */
	switch (flags) {
	case R_NEXT:			/* Next record. */
		idx = c->pg.index;
		while (++idx == NEXTINDEX(h)) {
			/* Crawl up if we hit the right edge. */
			e = bt_rcpop(rc);
			mpool_put(t->bt_mp, h, 0);
			if (e == NULL) /* Hit the right edge of root. */
				return RET_SPECIAL;
			idx = e->index;
			pg = e->pgno;
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
		}
		while (!(h->flags & (P_BLEAF | P_RLEAF))) {
			/* Crawl down the left until we hit a leaf. */
			status = bt_rcpush(rc, h->pgno, idx);
			pg = GETBINTERNAL(h, idx)->pgno;
			mpool_put(t->bt_mp, h, 0);
			if (status != RET_SUCCESS)
				return status;
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
			idx = 0;
		}
		break;
	case R_PREV:			/* Previous record. */
		idx = c->pg.index;
		while (!idx) {
			/* Crawl up if we hit the left edge. */
			e = bt_rcpop(rc);
			mpool_put(t->bt_mp, h, 0);
			if (e == NULL) /* Hit the left edge of root. */
				return RET_SPECIAL;
			idx = e->index;
			pg = e->pgno;
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
		}
		idx--;
		while (!(h->flags & (P_BLEAF | P_RLEAF))) {
			/* Crawl down the right until we hit a leaf. */
			status = bt_rcpush(rc, h->pgno, idx);
			pg = GETBINTERNAL(h, idx)->pgno;
			mpool_put(t->bt_mp, h, 0);
			if (status != RET_SUCCESS)
				return status;
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
				return (RET_ERROR);
			idx = NEXTINDEX(h) - 1;
		}
		break;
	}

	ep->page = h;
	ep->index = idx;
	c->pg.pgno = h->pgno;
	c->pg.index = idx;
	F_CLR(c, CURS_ACQUIRE | CURS_AFTER | CURS_BEFORE);
	F_SET(c, CURS_INIT);
	return (RET_SUCCESS);
}
