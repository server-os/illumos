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

#pragma weak _getrctl = getrctl
#pragma weak _setrctl = setrctl

#include "lint.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/rctl.h>

int
getrctl(const char *name, rctlblk_t *old_rblk, rctlblk_t *new_rblk,
    int flags)
{
	return (syscall(SYS_rctlsys,
	    0, name, old_rblk, new_rblk, 0, flags));
}

int
setrctl(const char *name, rctlblk_t *old_rblk, rctlblk_t *new_rblk,
    int flags)
{
	return (syscall(SYS_rctlsys,
	    1, name, old_rblk, new_rblk, 0, flags));
}

size_t
rctllist(char *list_buf, size_t list_bufsz)
{
	sysret_t rval;
	int error;

	error = __systemcall(&rval, SYS_rctlsys, 2, NULL, list_buf, NULL,
	    list_bufsz, 0);

	if (error)
		(void) __set_errno(error);
	return ((size_t)rval.sys_rval1);
}

int
rctlctl(const char *name, rctlblk_t *rblk, int flags)
{
	return (syscall(SYS_rctlsys, 3, name, rblk, NULL, 0, flags));
}

int
setprojrctl(const char *name, rctlblk_t *new_rblk, size_t size, int flags)
{
	return (syscall(SYS_rctlsys,
	    4, name, NULL, new_rblk, size, flags));
}
