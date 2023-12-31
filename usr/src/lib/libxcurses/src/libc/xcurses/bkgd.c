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
 * bkgd.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#ifdef M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/bkgd.c 1.1 1995/06/07 13:53:06 ant Exp $";
#endif
#endif

#include <private.h>

int
(bkgd)(chtype bg)
{
	int code;
	cchar_t cc;

#ifdef M_CURSES_TRACE
	__m_trace("bkgd(%lx)", bg);
#endif

	if ((code = __m_chtype_cc(bg, &cc)) == OK)
		wbkgrnd(stdscr, &cc);

	return __m_return_code("bkgd", code);
}

int
(wbkgd)(WINDOW *w, chtype bg)
{
	int code;
	cchar_t cc;

#ifdef M_CURSES_TRACE
	__m_trace("wbkgd(%p, %lx)", w, bg);
#endif

	if ((code = __m_chtype_cc(bg, &cc)) == OK)
		wbkgrnd(w, &cc);

	return __m_return_code("wbkgd", code);
}
