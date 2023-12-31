/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
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
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_MDB_ERR_H
#define	_MDB_ERR_H

#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>

#include <mdb/mdb_errno.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef _MDB

extern const char *mdb_strerror(int);

extern void vwarn(const char *, va_list);
extern void vdie(const char *, va_list);
extern void vfail(const char *, va_list);

extern void warn(const char *, ...);
extern void die(const char *, ...);
extern void fail(const char *, ...);

extern int set_errbytes(size_t, size_t);
extern int set_errno(int);

extern int ctf_to_errno(int);

#ifndef _KMDB
extern int tdb_to_errno(int);
extern int rdb_to_errno(int);
#endif

#endif /* _MDB */

#ifdef	__cplusplus
}
#endif

#endif	/* _MDB_ERR_H */
