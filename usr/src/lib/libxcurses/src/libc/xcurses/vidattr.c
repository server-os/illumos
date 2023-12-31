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
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

/*
 * vidattr.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#if M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/vidattr.c 1.2 1995/07/19 16:38:27 ant Exp $";
#endif
#endif

#include <private.h>

int
vidattr(chtype ch)
{
	int code;
	cchar_t cc;

#ifdef M_CURSES_TRACE
	__m_trace("vidattr(%lx)", ch);
#endif

	(void) __m_chtype_cc(ch, &cc);
	code = vid_puts(cc._at, cc._co, (void *) 0, __m_putchar);

	return __m_return_code("vidattr", code);
}

int
vidputs(chtype ch, int (*putout)(int))
{
	int code;
	cchar_t cc;

#ifdef M_CURSES_TRACE
	__m_trace("vidputs(%lx, %p)", ch, putout);
#endif

	(void) __m_chtype_cc(ch, &cc);
	code = vid_puts(cc._at, cc._co, (void *) 0, putout);

	return __m_return_code("vidputs", code);
}
