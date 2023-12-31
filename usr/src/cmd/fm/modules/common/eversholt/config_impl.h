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

#ifndef	_EFT_CONFIG_IMPL_H
#define	_EFT_CONFIG_IMPL_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * private to config.c and mdb module.
 *
 * Data structure for storing config.  all access to
 * to this information happens using the config.h interfaces.
 */
struct config {
	struct config *next;
	struct config *child;
	struct config *parent;
	const char *s;
	intptr_t num;
	struct lut *props;
};

#ifdef	__cplusplus
}
#endif

#endif	/* _EFT_CONFIG_IMPL_H */
