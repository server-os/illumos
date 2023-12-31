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
 * Copyright (c) 1988 by Sun Microsystems, Inc.
 */

#include	<sys/types.h>
#include	"curses_inc.h"

int
wadjcurspos(WINDOW *win)
	/*
	 * wmadjcurspos --- moves the cursor to the first column within the
	 * multi-column character somewhere on which the cursor curently is on.
	 */
{
	short	x;
	chtype	*_yy;

	x = win->_curx;
	_yy = win->_y[win->_cury];
	while ((x > 0) && (ISCBIT(_yy[x]))) --x;
	if (win->_curx != x) {
		win->_curx = x;
		return (win->_immed ? wrefresh(win): OK);
	} else {
		return (OK);
	}
}
