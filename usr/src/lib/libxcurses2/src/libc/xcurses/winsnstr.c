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
 * wins_nws.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1994 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#ifdef M_RCSID
#ifndef lint
static char rcsID[] =
"$Header: /team/ps/sun_xcurses/archive/local_changes/xcurses/src/lib/"
"libxcurses/src/libc/xcurses/rcs/winsnstr.c 1.2 1998/04/30 20:30:43 "
"cbates Exp $";
#endif
#endif

#include <private.h>

int
winsnstr(WINDOW *w, const char *mbs, int n)
{
	cchar_t	cc;
	int	i, y, x;
	short	oflags;

	y = w->_cury;
	x = w->_curx;

	if (n < 0)
		n = INT_MAX;

	/*
	 * Disable window flushing until the entire string has
	 * been written into the window.
	 */
	oflags = w->_flags & (W_FLUSH | W_SYNC_UP);
	w->_flags &= ~(W_FLUSH | W_SYNC_UP);

	for (; *mbs != '\0' && 0 < n; n -= i, mbs += i) {
		if ((i = __m_mbs_cc(mbs, w->_bg._at, w->_fg._co, &cc))
			< 0	|| __m_wins_wch(w, y, x, &cc, &y, &x) == ERR)
			break;
	}

	w->_flags |= oflags;

	WSYNC(w);

	return (WFLUSH(w));
}
