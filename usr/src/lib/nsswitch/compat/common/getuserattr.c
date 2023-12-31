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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <user_attr.h>
#include "compat_common.h"

static DEFINE_NSS_DB_ROOT(db_root);

static void
_nss_initf_userattr_compat(nss_db_params_t *p)
{
	p->name = NSS_DBNAM_USERATTR;
	p->config_name = NSS_DBNAM_PASSWD_COMPAT;
	p->default_config = NSS_DEFCONF_PASSWD_COMPAT;
}

static const char *
get_username(nss_XbyY_args_t *argp)
{
	userstr_t *user = (userstr_t *)argp->returnval;

	return (user->name);
}

static int
check_name(nss_XbyY_args_t *argp)
{
	userstr_t	*user = (userstr_t *)argp->returnval;
	const char	*name = argp->key.name;

#ifdef	DEBUG
	(void) fprintf(stdout,
	    "\n[getuserattr.c: check_name %s with %s]\n", user->name, name);
#endif	/* DEBUG */

	if (strcmp(user->name, name) == 0) {
		return (1);
	}
	return (0);
}

static nss_status_t
getbynam(compat_backend_ptr_t be, void *a)
{
	nss_status_t	res;
	nss_XbyY_args_t *argp = (nss_XbyY_args_t *)a;

#ifdef	DEBUG
	(void) fprintf(stdout, "\n[getuserattr.c: getbynam]\n");
#endif	/* DEBUG */

	res = _attrdb_compat_XY_all(be,
	    argp, 1, check_name, NSS_DBOP_USERATTR_BYNAME);

	return (res);
}

static compat_backend_op_t userattr_ops[] = {
	_nss_compat_destr,
	_nss_compat_endent,
	_nss_compat_setent,
	_nss_compat_getent,
	getbynam
};

/*ARGSUSED*/
nss_backend_t  *
_nss_compat_user_attr_constr(const char *dummy1,
    const char *dummy2,
    const char *dummy3,
    const char *dummy4,
    const char *dummy5)
{
	return (_nss_compat_constr(userattr_ops,
		sizeof (userattr_ops)/sizeof (userattr_ops[0]),
		USERATTR_FILENAME,
		NSS_LINELEN_USERATTR,
		&db_root,
		_nss_initf_userattr_compat,
		0,
		get_username,
		NULL));
}
