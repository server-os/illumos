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
 * printw.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#ifdef M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/printw.c 1.2 1995/07/14 20:49:55 ant Exp $";
#endif
#endif

#include <private.h>
#include <limits.h>
#include <stdarg.h>

int
printw(const char *fmt, ...)
{
	int code;
	va_list ap;

#ifdef M_CURSES_TRACE
	__m_trace("printw(%p = \"%s\", ...)", fmt, fmt);
#endif

	va_start(ap, fmt);
	code = vw_printw(stdscr, fmt, ap);
	va_end(ap);

	return __m_return_code("printw", code);
}

int
mvprintw(int y, int x, const char *fmt, ...)
{
	int code;
	va_list ap;

#ifdef M_CURSES_TRACE
	__m_trace("mvprintw(%d, %d, %p = \"%s\", ...)", y, x, fmt, fmt);
#endif

	va_start(ap, fmt);
	if ((code = wmove(stdscr, y, x)) == OK)
		code = vw_printw(stdscr, fmt, ap);
	va_end(ap);

	return __m_return_code("mvprintw", code);
}

int
mvwprintw(WINDOW *w, int y, int x, const char *fmt, ...)
{
	int code;
	va_list ap;

#ifdef M_CURSES_TRACE
	__m_trace("mvwprintw(%p, %d, %d, %p = \"%s\", ...)", w, y, x, fmt, fmt);
#endif

	va_start(ap, fmt);
	if ((code = wmove(w, y, x)) == OK)
		code = vw_printw(w, fmt, ap);
	va_end(ap);

	return __m_return_code("mvwprintw", code);
}

int
wprintw(WINDOW *w, const char *fmt, ...)
{
	int code;
	va_list ap;

#ifdef M_CURSES_TRACE
	__m_trace("wprintw(%p, %p = \"%s\", ...)", w, fmt, fmt);
#endif

	va_start(ap, fmt);
	code = vw_printw(w, fmt, ap);
	va_end(ap);

	return __m_return_code("wprintw", code);
}

