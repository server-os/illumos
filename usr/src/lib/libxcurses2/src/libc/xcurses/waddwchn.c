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
 * waddwchn.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#if M_RCSID
#ifndef lint
static char rcsID[] =
"$Header: /team/ps/sun_xcurses/archive/local_changes/xcurses/src/lib/"
"libxcurses/src/libc/xcurses/rcs/waddwchn.c 1.2 1998/04/21 16:21:32 "
"cbates Exp $";
#endif
#endif

#include <private.h>

int
wadd_wchnstr(WINDOW *w, const cchar_t *cp, int n)
{
	int	x, width;

	if (n < 0 || w->_maxx < (n += w->_curx))
		n = w->_maxx;

	for (x = w->_curx; x < n && cp->_n != 0 && cp->_wc[0] != '\0';
		x += width, ++cp)
		width = __m_cc_replace(w, w->_cury, x, cp, 0);

	WSYNC(w);

	return (WFLUSH(w));
}
