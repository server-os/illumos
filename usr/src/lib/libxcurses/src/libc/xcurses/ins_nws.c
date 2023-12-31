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
 * ins_nws.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#if M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/ins_nws.c 1.1 1995/06/15 15:15:08 ant Exp $";
#endif
#endif

#include <private.h>

int
(ins_nwstr)(wcs, n)
const wchar_t *wcs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("ins_nwstr(%p, %d)", wcs, n);
#endif

	code = wins_nwstr(stdscr, wcs, n);

	return __m_return_code("ins_nwstr", code);
}

int
(mvins_nwstr)(y, x, wcs, n)
int y, x;
const wchar_t *wcs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvins_nwstr(%d, %d, %p, %d)", y, x, wcs, n);
#endif

	if ((code = wmove(stdscr, y, x)) == OK)
		code = wins_nwstr(stdscr, wcs, n);

	return __m_return_code("mvins_nwstr", code);
}

int
(mvwins_nwstr)(w, y, x, wcs, n)
WINDOW *w;
int y, x;
const wchar_t *wcs;
int n;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvwins_nwstr(%p, %d, %d, %p, %d)", w, y, x, wcs, n);
#endif

	if ((code = wmove(w, y, x)) == OK)
		code = wins_nwstr(w, wcs, n);

	return __m_return_code("mvwins_nwstr", code);
}

int
(ins_wstr)(wcs)
const wchar_t *wcs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("ins_wstr(%p)", wcs);
#endif

	code = wins_nwstr(stdscr, wcs, -1);

	return __m_return_code("ins_wstr", code);
}

int
(mvins_wstr)(y, x, wcs)
int y, x;
const wchar_t *wcs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvins_wstr(%d, %d, %p)", y, x, wcs);
#endif

	if ((code = wmove(stdscr, y, x)) == OK)
		code = wins_nwstr(stdscr, wcs, -1);

	return __m_return_code("mvins_wstr", code);
}

int
(mvwins_wstr)(w, y, x, wcs)
WINDOW *w;
int y, x;
const wchar_t *wcs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("mvwins_wstr(%p, %d, %d, %p)", w, y, x, wcs);
#endif

	if ((code = wmove(w, y, x)) == OK)
		code = wins_nwstr(w, wcs, -1);

	return __m_return_code("mvwins_wstr", code);
}

int
(wins_wstr)(w, wcs)
WINDOW *w;
const wchar_t *wcs;
{
	int code;

#ifdef M_CURSES_TRACE
	__m_trace("wins_wstr(%p, %p)", w, wcs);
#endif

	code = wins_nwstr(w, wcs, -1);

	return __m_return_code("wins_wstr", code);
}

