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
 * Copyright (c) 1996-1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

/*
 * private.h
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 */

#include <sys/isa_defs.h>
#include <mks.h>
#include <curses.h>
#include <termios.h>
#include <unctrl.h>

#define	M_TERM_NAME		"unknown"

/*
 * Interbyte timer value used for processing multibyte function keys.
 */
#ifndef M_CURSES_INTERBYTE_TIME
#define	M_CURSES_INTERBYTE_TIME		10
#endif

/*
 * Maximum number of lines that can be ripped off.
 */
#ifndef M_CURSES_MAX_RIPOFFLINE
#define	M_CURSES_MAX_RIPOFFLINE		5
#endif

/*
 * See copywin() and wrefresh() case 4.  It is unclear whether XPG4 V2
 * disallows supporting case 4 (expanding characters straddling a window
 * boundary).
 */
#define	M_CURSES_SENSIBLE_WINDOWS

/*
 * Enable typeahead() support.
 */
/* using unget buffer as typeahead buffer too */
#define	M_TYPEAHEAD_SIZE	200

/*
 * END OF CONFIGURABLE SECTION
 */

/*
 * Constant WINDOW definition attributes.
 */
#define	W_IS_PAD	0x0001	/* Window is a pad. */
#define	W_END_LINE	0x0002	/* End of line is the margin. */
#define	W_FULL_LINE	0x0004	/* Line spans screen width. */
#define	W_FULL_WINDOW	0x0008	/* Window is full screen. */
#define	W_SCROLL_WINDOW	0x0010	/* Touches bottom-right corner */

/*
 * WINDOW state.
 */
#define	W_CLEAR_WINDOW	0x0020	/* clearok() clear screen next update. */
#define	W_REDRAW_WINDOW 0x0040	/* wredrawln() use simple() next update. */

/*
 * Configurable WINDOW options.
 */
#define	W_FLUSH		0x0080	/* immedok() update when window changes. */
#define	W_CAN_SCROLL	0x0100	/* scrollok() window can software scroll. */
#define	W_LEAVE_CURSOR	0x0200	/* leaveok() don't fuss with the cursor. */
#define	W_SYNC_UP	0x0400	/* syncok() update ancestors when changed. */
#define	W_USE_KEYPAD	0x0800	/* keypad() enbles KEY_ processing. */
#define	W_USE_TIMEOUT	0x1000	/* notimeout() disables the interbyte timer. */
#define	W_CONFIG_MASK	0x1f80 	/* Mask of configurable flags. */

/*
 * Flags used in SCREEN.
 */
#define	S_ECHO		0x0001	/* Software echo enbled. */
#define	S_ENDWIN	0x0002	/* Curses is in "shell" mode. */
#define	S_INS_DEL_CHAR	0x0004	/* idcok() enabled for terminal (future). */
#define	S_INS_DEL_LINE	0x0008	/* idlok() enabled for terminal. */
#define	S_ISATTY	0x0010	/* _kfd is a terminal. */
#define	S_USE_META	0x0020	/* meta() enabled. */
#define	S_TYPEAHEAD_OK	0x0040	/* typeahead flag */

typedef struct t_decode {
	struct t_decode	*sibling;
	struct t_decode	*child;
	short	key;			/* KEY_ value or 0. */
	int	ch;		/* Character found by this node. */
} t_decode;

typedef struct {
	int	top;			/* # of lines off the top. */
	int	bottom;			/* # of lines off the bottom (-ve). */
	struct {
		int	dy;		/* Distance from screen top/bottom. */
		int	(*init)(WINDOW *, int);	/* Init. function for window. */
		int	created;
	} line[M_CURSES_MAX_RIPOFFLINE];
} t_rip;

extern SCREEN	*__m_screen;
extern int	__m_slk_format;
extern int	__m_slk_labels_on;
extern const short	__m_keyindex[][2];

extern const char	*boolnames[];
extern const char	*boolcodes[];
extern const char	*boolfnames[];
extern const char	*numnames[];
extern const char	*numcodes[];
extern const char	*numfnames[];
extern const char	*strnames[];
extern const char	*strcodes[];
extern const char	*strfnames[];

#define	ATTR_STATE	cur_term->_at

#define	PTERMIOS(x)	((struct termios *)(cur_term->x))

#define	__m_cc_width(cc)	wcwidth((cc)->_wc[0])

#define	TPUTS(a, b, c)			tputs(a, b, c)

extern int	__m_wc_cc(wint_t, cchar_t *);
extern int	__m_mbs_cc(const char *, attr_t, short, cchar_t *);
extern int	__m_wcs_cc(const wchar_t *, attr_t, short, cchar_t *);
extern int	__m_acs_cc(chtype, cchar_t *);
extern int	__m_wacs_cc(const cchar_t *, cchar_t *);
extern int	__m_cc_mbs(const cchar_t *, char *, int);

extern int	__m_cc_sort(cchar_t *);
extern int	__m_cc_write(const cchar_t *);
extern int	__m_cc_first(WINDOW *, int, int);
extern int	__m_cc_next(WINDOW *, int, int);
extern int	__m_cc_islast(WINDOW *, int, int);
extern int	__m_cc_expand(WINDOW *, int, int, int);
extern int	__m_cc_erase(WINDOW *, int, int, int, int);
extern int	__m_cc_compare(const cchar_t *, const cchar_t *, int);
extern int	__m_cc_replace(WINDOW *, int, int, const cchar_t *, int);
extern int	__m_cc_add(WINDOW *, int, int, const cchar_t *, int,
	int *, int *);
#if defined(_LP64)
extern void	__m_cc_hash(WINDOW *, unsigned int *, int);
#else
extern void	__m_cc_hash(WINDOW *, unsigned long *, int);
#endif

extern int	__m_set_echo(int);
extern int	__m_tty_get(struct termios *);
extern int	__m_tty_set(struct termios *);
extern int	__m_decode_init(t_decode **);
extern void	__m_decode_free(t_decode **);
extern int	__m_do_scroll(WINDOW *, int, int, int *, int *);
extern int	__m_ptr_move(void **, unsigned, unsigned, unsigned, unsigned);

extern int	__m_doupdate_init(void);
extern int	__m_wins_wch(WINDOW *, int, int, const cchar_t *, int *, int *);
extern int	__m_cc_ins(WINDOW *, int, int, const cchar_t *);
extern void	__m_mvcur_cost(void);

extern int	__m_cc_modify(WINDOW *, int, int, const cchar_t *);
extern int	__m_tty_set_prog_mode(void);
extern int	__m_tputs(const char *, int, int (*)(int));
extern int	__m_slk_clear(int);
extern int	__m_cc_add_k(WINDOW *, int, int, const cchar_t *,
	int, int *, int *);
extern int	__m_cc_equal(const cchar_t *, const cchar_t *);
extern void	__m_touch_locs(WINDOW *, int, int, int);
extern int	__m_wadd_wch(WINDOW *, const cchar_t *);
extern void	__m_slk_doupdate(void);
extern int	__m_outc(int);
extern int	__m_tty_wc(int, wchar_t *);
extern int	__m_chtype_cc(chtype, cchar_t *);
extern chtype	__m_cc_chtype(const cchar_t *);
extern int	__m_copywin(const WINDOW *, WINDOW *, int);
extern WINDOW	*__m_newwin(WINDOW *, int, int, int, int);
extern int	__m_putchar(int);
extern int	__m_mvcur(int, int, int, int, int (*)(int));
extern int	__m_read_terminfo(const char *, TERMINAL *);
extern int	__m_setupterm(char *, int, int, int *);

extern int	pollTypeahead(void);
extern int	wistombs(char *, const wint_t *, int);
extern int	wistowcs(wchar_t *, const wint_t *, int);

extern void	wtouchln_hard(WINDOW *, int, int);

/*
 * Unique callback functions to initialize a SCREEN's wide_io_t structure,
 * which is used by __m_wio_get().   The __xc_ denotes XCurses and is used
 * instead of __m_ to avoid possible name conflicts else-where in MKS
 * libraries and applications.  Note that wgetch() is used for the get
 * function.
 */
extern int	__xc_feof(void *);
extern int	__xc_ferror(void *);
extern void	__xc_clearerr(void *);
extern int	__xc_ungetc(int, void *);

/*
 * Input stack macros replaced by these functions.
 */
extern void	iqAdd(unsigned int ch);
extern void	iqPush(unsigned int ch);
extern int	iqIsEmpty(void);
extern void	iqReset(void);
extern int	iqPull(void);

#define	WSYNC(w)	\
	if ((w)->_flags & W_SYNC_UP) {\
		wsyncup(w);\
	}
#define	WFLUSH(w)	(((w)->_flags & W_FLUSH) ? wrefresh(w) : OK)

/* end */
