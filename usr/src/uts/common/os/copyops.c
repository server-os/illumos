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

#include <sys/types.h>
#include <sys/copyops.h>
#include <sys/systm.h>

int
physio(int (*strat)(struct buf *), struct buf *bp, dev_t dev,
    int rw, void (*mincnt)(struct buf *), struct uio *uio)
{
	return (default_physio(strat, bp, dev, rw, mincnt, uio));
}

void
install_copyops(kthread_id_t tp, copyops_t *cp)
{
	ASSERT(tp->t_copyops == NULL);
	tp->t_copyops = cp;
}

void
remove_copyops(kthread_id_t tp)
{
	ASSERT(tp->t_copyops != NULL);
	tp->t_copyops = NULL;
}

int
copyops_installed(kthread_id_t tp)
{
	return (tp->t_copyops != NULL);
}
