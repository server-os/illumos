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
 * winnwstr.c
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
"libxcurses/src/libc/xcurses/rcs/winnwstr.c 1.4 1998/05/27 19:20:06 "
"cbates Exp $";
#endif
#endif

#include <private.h>

int
winnwstr(WINDOW *w, wchar_t *wcs, int n)
{
	int	i, y, x;
	int	j = 0;
	cchar_t	*cp;

	if (n < 0)
		n = w->_maxx;

	for (x = w->_curx; x < w->_maxx && 0 < n; n -= i) {
		cp = &w->_line[y = w->_cury][x];

		/* Will entire character fit into buffer? */
		if (n < cp->_n)
			break;

		/* Copy only the character portion. */
		for (i = 0; i < cp->_n; ++i) {
			*wcs++ = cp->_wc[i];
			j++;
		}

		x = __m_cc_next(w, y, x);
	}

	/* Enough room to null terminate the buffer? */
	if (0 < n)
		*wcs = '\0';

	return (j);
}
