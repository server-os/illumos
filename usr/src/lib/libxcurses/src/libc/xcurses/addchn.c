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
 * addchn.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#if M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/addchn.c 1.1 1995/05/30 13:39:22 ant Exp $";
#endif
#endif

#include <private.h>

#undef addchnstr

int
addchnstr(chs, n)
const chtype *chs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("addchnstr(%p, %d)", chs, n);
#endif

	code = waddchnstr(stdscr, chs, n);

	return __m_return_code("addchnstr", code);
}

#undef mvaddchnstr

int
mvaddchnstr(y, x, chs, n)
int y, x;
const chtype *chs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvaddchnstr(%d, %d, %p, %d)", y, x, chs, n);
#endif

	if ((code = wmove(stdscr, y, x)) == OK)
		code = waddchnstr(stdscr, chs, n);

	return __m_return_code("mvaddchnstr", code);
}

#undef mvwaddchnstr

int
mvwaddchnstr(w, y, x, chs, n)
WINDOW *w;
int y, x;
const chtype *chs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvwaddchnstr(%p, %d, %d, %p, %d)", w, y, x, chs, n);
#endif

	if ((code = wmove(w, y, x)) == OK)
		code = waddchnstr(w, chs, n);

	return __m_return_code("mvwaddchnstr", code);
}

#undef addchstr

int
addchstr(chs)
const chtype *chs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("addchstr(%p)", chs);
#endif

	code = waddchnstr(stdscr, chs, -1);

	return __m_return_code("addchstr", code);
}

#undef mvaddchstr

int
mvaddchstr(y, x, chs)
int y, x;
const chtype *chs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvaddchstr(%d, %d, %p)", y, x, chs);
#endif

	if ((code = wmove(stdscr, y, x)) == OK)
		code = waddchnstr(stdscr, chs, -1);

	return __m_return_code("mvaddchstr", code);
}

#undef mvwaddchstr

int
mvwaddchstr(w, y, x, chs)
WINDOW *w;
int y, x;
const chtype *chs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvwaddchstr(%p, %d, %d, %p)", w, y, x, chs);
#endif

	if ((code = wmove(w, y, x)) == OK)
		code = waddchnstr(w, chs, -1);

	return __m_return_code("mvwaddchstr", code);
}

#undef waddchstr

int
waddchstr(w, chs)
WINDOW *w;
const chtype *chs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("waddchstr(%p, %p)", w, chs);
#endif

	code = waddchnstr(w, chs, -1);

	return __m_return_code("waddchstr", code);
}

