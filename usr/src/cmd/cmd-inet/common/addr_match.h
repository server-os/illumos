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

#ifndef _ADDR_MATCH_H
#define	_ADDR_MATCH_H

/*
 * Prototypes for functions in addr_match.c
 */

#ifdef __cplusplus
extern "C" {
#endif

void check_address(const char *prog, const struct sockaddr_storage *fromp,
    const struct sockaddr_in *sin, const struct sockaddr_in6 *sin6,
    const char *printable_addr, char *hostname, size_t hostsize);

#ifdef __cplusplus
}
#endif

#endif /* _ADDR_MATCH_H */
