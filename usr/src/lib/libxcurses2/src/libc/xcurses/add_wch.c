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
 * Copyright (c) 1995-1998 by Sun Microsystems, Inc.
 * All rights reserved.
 */

/* LINTLIBRARY */

/*
 * add_wch.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#if M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/add_wch.c 1.3 "
"1995/07/07 17:58:47 ant Exp $";
#endif
#endif

#include <private.h>

#undef add_wch

int
add_wch(const cchar_t *cc)
{
	int code;

	code = wadd_wch(stdscr, cc);

	return (code);
}

#undef mvadd_wch

int
mvadd_wch(int y, int x, const cchar_t *cc)
{
	int code;

	if ((code = wmove(stdscr, y, x)) == OK)
		code = wadd_wch(stdscr, cc);

	return (code);
}

#undef mvwadd_wch

int
mvwadd_wch(WINDOW *w, int y, int x, const cchar_t *cc)
{
	int code;

	if ((code = wmove(w, y, x)) == OK)
		code = wadd_wch(w, cc);

	return (code);
}
