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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_PRI_H
#define	_PRI_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define		PRI_GET		0
#define		PRI_WAITGET	1
#define		DS_PRI_DRIVER	"/devices/pseudo/ds_pri@0:ds_pri"

extern int pri_init(void);
extern void pri_fini(void);
extern ssize_t pri_get(uint8_t wait, uint64_t *token, uint64_t **buf,
		void *(*allocp)(size_t), void (*freep)(void *, size_t));

#ifdef	__cplusplus
}
#endif

#endif	/* _PRI_H */
