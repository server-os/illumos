/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*LINTLIBRARY*/

#ifndef lint
static char
sccsid[] = "@(#)toucholap.c 1.4 89/08/03 SMI"; /* from UCB 5.1 85/06/07 */
#endif	/* not lint */

#include	"curses.ext"

#define	min(a, b)	((a) < (b) ? (a) : (b))
#define	max(a, b)	((a) > (b) ? (a) : (b))

/*
 *	Touch, on win2, the part that overlaps with win1.
 */

int
touchoverlap(WINDOW *win1, WINDOW *win2)
{
	int	y, endy, endx, starty, startx;

#ifdef DEBUG
	fprintf(outf, "TOUCHOVERLAP(%0.2o, %0.2o);\n", win1, win2);
#endif
	starty = max(win1->_begy, win2->_begy);
	startx = max(win1->_begx, win2->_begx);
	endy = min(win1->_maxy + win1->_begy, win2->_maxy + win2->_begy);
	endx = min(win1->_maxx + win1->_begx, win2->_maxx + win2->_begx);
#ifdef DEBUG
	fprintf(outf, "TOUCHOVERLAP:from (%d,%d) to (%d,%d)\n",
	    starty, startx, endy, endx);
	fprintf(outf, "TOUCHOVERLAP:win1 (%d,%d) to (%d,%d)\n",
	    win1->_begy, win1->_begx, win1->_begy + win1->_maxy,
	    win1->_begx + win1->_maxx);
	fprintf(outf, "TOUCHOVERLAP:win2 (%d,%d) to (%d,%d)\n",
	    win2->_begy, win2->_begx, win2->_begy + win2->_maxy,
	    win2->_begx + win2->_maxx);
#endif
	if (starty >= endy || startx >= endx)
		return (OK);
	starty -= win2->_begy;
	startx -= win2->_begx;
	endy -= win2->_begy;
	endx -= win2->_begx;
	endx--;
	for (y = starty; y < endy; y++)
		(void) touchline(win2, y, startx, endx);
	return (OK);
}
