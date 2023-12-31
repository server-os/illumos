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
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/

/*
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 * Copyright 2016 Joyent, Inc.
 * Copyright 2018 OmniOS Community Edition (OmniOSce) Association.
 * Copyright 2023 Oxide Computer Company
 *
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_ASSERT_H
#define	_ASSERT_H

#include <sys/feature_tests.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ - 0 >= 199901L
extern _NORETURN_KYWD void __assert_c99(const char *, const char *, int,
    const char *) __NORETURN;
#else
extern _NORETURN_KYWD void __assert(const char *, const char *, int) __NORETURN;
#endif /* __STDC_VERSION__ - 0 >= 199901L */

/*
 * In C11 the static_assert macro is always defined, unlike the assert macro.
 */
#if __STDC_VERSION__ - 0 >= 201112L && !defined(__cplusplus)
#define	static_assert	_Static_assert
#endif /* __STDC_VERSION - 0 >= 201112L && !defined(__cplusplus) */

#ifdef	__cplusplus
}
#endif

#endif	/* _ASSERT_H */

/*
 * Note that the ANSI C Standard requires all headers to be idempotent except
 * <assert.h> which is explicitly required not to be idempotent (section 4.1.2).
 * Therefore, it is by intent that the header guards (#ifndef _ASSERT_H) do
 * not span this entire file.
 */

#undef	assert

#ifdef	NDEBUG

#define	assert(EX) ((void)0)

#else

#if __STDC_VERSION__ - 0 >= 199901L
#define	assert(EX) (void)((EX) || \
	(__assert_c99(#EX, __FILE__, __LINE__, __func__), 0))
#else
#define	assert(EX) (void)((EX) || (__assert(#EX, __FILE__, __LINE__), 0))
#endif /* __STDC_VERSION__ - 0 >= 199901L */

#endif	/* NDEBUG */
