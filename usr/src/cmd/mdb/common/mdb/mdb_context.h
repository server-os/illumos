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
 * Copyright (c) 1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef	_MDB_CONTEXT_H
#define	_MDB_CONTEXT_H

#include <sys/types.h>
#include <setjmp.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	_MDB

/*
 * We hide the details of the context from the rest of MDB using the opaque
 * mdb_context tag.  This will facilitate later porting activities.
 */
typedef struct mdb_context mdb_context_t;

extern mdb_context_t *mdb_context_create(int (*)(void));
extern void mdb_context_destroy(mdb_context_t *);
extern void mdb_context_switch(mdb_context_t *);
extern jmp_buf *mdb_context_getpcb(mdb_context_t *);

#endif	/* _MDB */

#ifdef	__cplusplus
}
#endif

#endif	/* _MDB_CONTEXT_H */
