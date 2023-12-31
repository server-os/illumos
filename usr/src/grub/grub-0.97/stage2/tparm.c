/****************************************************************************
 * Copyright (c) 1998,2000,2002 Free Software Foundation, Inc.              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/**********************************************************************
 * This code is a modification of lib_tparm.c found in ncurses-5.2. The
 * modification are for use in grub by replacing all libc function through
 * special grub functions. This also meant to delete all dynamic memory
 * allocation and replace it by a number of fixed buffers.
 *
 * Modifications by Tilmann Bubeck <t.bubeck@reinform.de> 2002
 **********************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 ****************************************************************************/

/*
 *	tparm.c
 *
 */

#include "shared.h"

#include "tparm.h"

/*
 * Common/troublesome character definitions
 */
typedef char grub_bool;
#undef isdigit
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#ifndef FALSE
# define FALSE (0)
#endif
#ifndef TRUE
# define TRUE (!FALSE)
#endif
#define MAX_FORMAT_LEN 256
#define max(a,b) ((a) > (b) ? (a) : (b))

//MODULE_ID("$Id: tparm.c,v 1.1.1.1 2003/11/20 02:04:59 fengshuo Exp $")

/*
 *	char *
 *	tparm(string, ...)
 *
 *	Substitute the given parameters into the given string by the following
 *	rules (taken from terminfo(7)):
 *
 *	     Cursor addressing and other strings  requiring  parame-
 *	ters in the terminal are described by a parameterized string
 *	capability, with like escapes %x in  it.   For  example,  to
 *	address  the  cursor, the cup capability is given, using two
 *	parameters: the row and column to  address  to.   (Rows  and
 *	columns  are  numbered  from  zero and refer to the physical
 *	screen visible to the user, not to any  unseen  memory.)  If
 *	the terminal has memory relative cursor addressing, that can
 *	be indicated by
 *
 *	     The parameter mechanism uses  a  stack  and  special  %
 *	codes  to manipulate it.  Typically a sequence will push one
 *	of the parameters onto the stack and then print it  in  some
 *	format.  Often more complex operations are necessary.
 *
 *	     The % encodings have the following meanings:
 *
 *	     %%        outputs `%'
 *	     %c        print pop() like %c in printf()
 *	     %s        print pop() like %s in printf()
 *           %[[:]flags][width[.precision]][doxXs]
 *                     as in printf, flags are [-+#] and space
 *                     The ':' is used to avoid making %+ or %-
 *                     patterns (see below).
 *
 *	     %p[1-9]   push ith parm
 *	     %P[a-z]   set dynamic variable [a-z] to pop()
 *	     %g[a-z]   get dynamic variable [a-z] and push it
 *	     %P[A-Z]   set static variable [A-Z] to pop()
 *	     %g[A-Z]   get static variable [A-Z] and push it
 *	     %l        push strlen(pop)
 *	     %'c'      push char constant c
 *	     %{nn}     push integer constant nn
 *
 *	     %+ %- %* %/ %m
 *	               arithmetic (%m is mod): push(pop() op pop())
 *	     %& %| %^  bit operations: push(pop() op pop())
 *	     %= %> %<  logical operations: push(pop() op pop())
 *	     %A %O     logical and & or operations for conditionals
 *	     %! %~     unary operations push(op pop())
 *	     %i        add 1 to first two parms (for ANSI terminals)
 *
 *	     %? expr %t thenpart %e elsepart %;
 *	               if-then-else, %e elsepart is optional.
 *	               else-if's are possible ala Algol 68:
 *	               %? c1 %t b1 %e c2 %t b2 %e c3 %t b3 %e c4 %t b4 %e b5 %;
 *
 *	For those of the above operators which are binary and not commutative,
 *	the stack works in the usual way, with
 *			%gx %gy %m
 *	resulting in x mod y, not the reverse.
 */

#define STACKSIZE	20

typedef struct {
    union {
	unsigned int num;
	char *str;
    } data;
    grub_bool num_type;
} stack_frame;

static stack_frame stack[STACKSIZE];
static int stack_ptr;

static char out_buff[256];
static int out_size = 256;
static int out_used;

static inline void
get_space(int need)
{
    need += out_used;
    if (need > out_size) {
	// FIX ME! buffer full, what now?
	;
    }
}

static inline void
save_text(const char *fmt, const char *s, int len)
{
    int s_len = grub_strlen(s);
    if (len > (int) s_len)
	s_len = len;

    get_space(s_len + 1);

    (void) grub_sprintf(out_buff + out_used, fmt, s);
    out_used += grub_strlen(out_buff + out_used);
}

static inline void
save_number(const char *fmt, int number, int len)
{
    if (len < 30)
	len = 30;		/* actually log10(MAX_INT)+1 */

    get_space(len + 1);

    (void) grub_sprintf(out_buff + out_used, fmt, number);
    out_used += grub_strlen(out_buff + out_used);
}

static inline void
save_char(int c)
{
    if (c == 0)
	c = 0200;
    get_space(1);
    out_buff[out_used++] = c;
}

static inline void
npush(int x)
{
    if (stack_ptr < STACKSIZE) {
	stack[stack_ptr].num_type = TRUE;
	stack[stack_ptr].data.num = x;
	stack_ptr++;
    }
}

static inline int
npop(void)
{
    int result = 0;
    if (stack_ptr > 0) {
	stack_ptr--;
	if (stack[stack_ptr].num_type)
	    result = stack[stack_ptr].data.num;
    }
    return result;
}

static inline void
spush(char *x)
{
    if (stack_ptr < STACKSIZE) {
	stack[stack_ptr].num_type = FALSE;
	stack[stack_ptr].data.str = x;
	stack_ptr++;
    }
}

static inline char *
spop(void)
{
    static char dummy[] = "";	/* avoid const-cast */
    char *result = dummy;
    if (stack_ptr > 0) {
	stack_ptr--;
	if (!stack[stack_ptr].num_type && stack[stack_ptr].data.str != 0)
	    result = stack[stack_ptr].data.str;
    }
    return result;
}

static inline const char *
parse_format(const char *s, char *format, int *len)
{
    grub_bool done = FALSE;
    grub_bool allowminus = FALSE;
    grub_bool dot = FALSE;
    grub_bool err = FALSE;
    char *fmt = format;
    int prec = 0;
    int width = 0;
    int value = 0;

    *len = 0;
    *format++ = '%';
    while (*s != '\0' && !done) {
	switch (*s) {
	case 'c':		/* FALLTHRU */
	case 'd':		/* FALLTHRU */
	case 'o':		/* FALLTHRU */
	case 'x':		/* FALLTHRU */
	case 'X':		/* FALLTHRU */
	case 's':
	    *format++ = *s;
	    done = TRUE;
	    break;
	case '.':
	    *format++ = *s++;
	    if (dot) {
		err = TRUE;
	    } else {
		dot = TRUE;
		prec = value;
	    }
	    value = 0;
	    break;
	case '#':
	    *format++ = *s++;
	    break;
	case ' ':
	    *format++ = *s++;
	    break;
	case ':':
	    s++;
	    allowminus = TRUE;
	    break;
	case '-':
	    if (allowminus) {
		*format++ = *s++;
	    } else {
		done = TRUE;
	    }
	    break;
	default:
	    if (isdigit(*s)) {
		value = (value * 10) + (*s - '0');
		if (value > 10000)
		    err = TRUE;
		*format++ = *s++;
	    } else {
		done = TRUE;
	    }
	}
    }

    /*
     * If we found an error, ignore (and remove) the flags.
     */
    if (err) {
	prec = width = value = 0;
	format = fmt;
	*format++ = '%';
	*format++ = *s;
    }

    if (dot)
	width = value;
    else
	prec = value;

    *format = '\0';
    /* return maximum string length in print */
    *len = (prec > width) ? prec : width;
    return s;
}

#define isUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define isLOWER(c) ((c) >= 'a' && (c) <= 'z')

static inline char *
tparam_internal(const char *string, int *dataptr)
{
#define NUM_VARS 26
    char *p_is_s[9];
    int param[9];
    int lastpop;
    int popcount;
    int number;
    int len;
    int level;
    int x, y;
    int i;
    int len2;
    register const char *cp;
    static int len_fmt = MAX_FORMAT_LEN;
    static char dummy[] = "";
    static char format[MAX_FORMAT_LEN];
    static int dynamic_var[NUM_VARS];
    static int static_vars[NUM_VARS];

    out_used = 0;
    if (string == NULL)
	return NULL;

    if ((len2 = grub_strlen(string)) > len_fmt) {
	return NULL;
    }

    /*
     * Find the highest parameter-number referred to in the format string.
     * Use this value to limit the number of arguments copied from the
     * variable-length argument list.
     */

    number = 0;
    lastpop = -1;
    popcount = 0;
    grub_memset(p_is_s, 0, sizeof(p_is_s));

    /*
     * Analyze the string to see how many parameters we need from the varargs
     * list, and what their types are.  We will only accept string parameters
     * if they appear as a %l or %s format following an explicit parameter
     * reference (e.g., %p2%s).  All other parameters are numbers.
     *
     * 'number' counts coarsely the number of pop's we see in the string, and
     * 'popcount' shows the highest parameter number in the string.  We would
     * like to simply use the latter count, but if we are reading termcap
     * strings, there may be cases that we cannot see the explicit parameter
     * numbers.
     */
    for (cp = string; (cp - string) < (int) len2;) {
	if (*cp == '%') {
	    cp++;
	    cp = parse_format(cp, format, &len);
	    switch (*cp) {
	    default:
		break;

	    case 'd':		/* FALLTHRU */
	    case 'o':		/* FALLTHRU */
	    case 'x':		/* FALLTHRU */
	    case 'X':		/* FALLTHRU */
	    case 'c':		/* FALLTHRU */
		number++;
		lastpop = -1;
		break;

	    case 'l':
	    case 's':
		if (lastpop > 0)
		    p_is_s[lastpop - 1] = dummy;
		++number;
		break;

	    case 'p':
		cp++;
		i = (*cp - '0');
		if (i >= 0 && i <= 9) {
		    lastpop = i;
		    if (lastpop > popcount)
			popcount = lastpop;
		}
		break;

	    case 'P':
	    case 'g':
		cp++;
		break;

	    case '\'':
		cp += 2;
		lastpop = -1;
		break;

	    case '{':
		cp++;
		while (*cp >= '0' && *cp <= '9') {
		    cp++;
		}
		break;

	    case '+':
	    case '-':
	    case '*':
	    case '/':
	    case 'm':
	    case 'A':
	    case 'O':
	    case '&':
	    case '|':
	    case '^':
	    case '=':
	    case '<':
	    case '>':
	    case '!':
	    case '~':
		lastpop = -1;
		number += 2;
		break;

	    case 'i':
		lastpop = -1;
		if (popcount < 2)
		    popcount = 2;
		break;
	    }
	}
	if (*cp != '\0')
	    cp++;
    }

    if (number > 9)
	number = 9;
    for (i = 0; i < max(popcount, number); i++) {
	/*
	 * A few caps (such as plab_norm) have string-valued parms.
	 * We'll have to assume that the caller knows the difference, since
	 * a char* and an int may not be the same size on the stack.
	 */
	if (p_is_s[i] != 0) {
	  p_is_s[i] = (char *)(*(dataptr++));
	} else {
	  param[i] = (int)(*(dataptr++));
	}
    }

    /*
     * This is a termcap compatibility hack.  If there are no explicit pop
     * operations in the string, load the stack in such a way that
     * successive pops will grab successive parameters.  That will make
     * the expansion of (for example) \E[%d;%dH work correctly in termcap
     * style, which means tparam() will expand termcap strings OK.
     */
    stack_ptr = 0;
    if (popcount == 0) {
	popcount = number;
	for (i = number - 1; i >= 0; i--)
	    npush(param[i]);
    }

    while (*string) {
        /* skip delay timings */
	if (*string == '$' && *(string + 1) == '<') {
	    while( *string && *string != '>') 
	        string++;
	    if ( *string == '>' ) string++;
	} else if ( *string == '%') {
	    string++;
	    string = parse_format(string, format, &len);
	    switch (*string) {
	    default:
		break;
	    case '%':
		save_char('%');
		break;

	    case 'd':		/* FALLTHRU */
	    case 'o':		/* FALLTHRU */
	    case 'x':		/* FALLTHRU */
	    case 'X':		/* FALLTHRU */
	    case 'c':		/* FALLTHRU */
		save_number(format, npop(), len);
		break;

	    case 'l':
		save_number("%d", strlen(spop()), 0);
		break;

	    case 's':
		save_text(format, spop(), len);
		break;

	    case 'p':
		string++;
		i = (*string - '1');
		if (i >= 0 && i < 9) {
		    if (p_is_s[i])
			spush(p_is_s[i]);
		    else
			npush(param[i]);
		}
		break;

	    case 'P':
		string++;
		if (isUPPER(*string)) {
		    i = (*string - 'A');
		    static_vars[i] = npop();
		} else if (isLOWER(*string)) {
		    i = (*string - 'a');
		    dynamic_var[i] = npop();
		}
		break;

	    case 'g':
		string++;
		if (isUPPER(*string)) {
		    i = (*string - 'A');
		    npush(static_vars[i]);
		} else if (isLOWER(*string)) {
		    i = (*string - 'a');
		    npush(dynamic_var[i]);
		}
		break;

	    case '\'':
		string++;
		npush(*string);
		string++;
		break;

	    case '{':
		number = 0;
		string++;
		while (*string >= '0' && *string <= '9') {
		    number = number * 10 + *string - '0';
		    string++;
		}
		npush(number);
		break;

	    case '+':
		npush(npop() + npop());
		break;

	    case '-':
		y = npop();
		x = npop();
		npush(x - y);
		break;

	    case '*':
		npush(npop() * npop());
		break;

	    case '/':
		y = npop();
		x = npop();
		npush(y ? (x / y) : 0);
		break;

	    case 'm':
		y = npop();
		x = npop();
		npush(y ? (x % y) : 0);
		break;

	    case 'A':
		npush(npop() && npop());
		break;

	    case 'O':
		npush(npop() || npop());
		break;

	    case '&':
		npush(npop() & npop());
		break;

	    case '|':
		npush(npop() | npop());
		break;

	    case '^':
		npush(npop() ^ npop());
		break;

	    case '=':
		y = npop();
		x = npop();
		npush(x == y);
		break;

	    case '<':
		y = npop();
		x = npop();
		npush(x < y);
		break;

	    case '>':
		y = npop();
		x = npop();
		npush(x > y);
		break;

	    case '!':
		npush(!npop());
		break;

	    case '~':
		npush(~npop());
		break;

	    case 'i':
		if (p_is_s[0] == 0)
		    param[0]++;
		if (p_is_s[1] == 0)
		    param[1]++;
		break;

	    case '?':
		break;

	    case 't':
		x = npop();
		if (!x) {
		    /* scan forward for %e or %; at level zero */
		    string++;
		    level = 0;
		    while (*string) {
			if (*string == '%') {
			    string++;
			    if (*string == '?')
				level++;
			    else if (*string == ';') {
				if (level > 0)
				    level--;
				else
				    break;
			    } else if (*string == 'e' && level == 0)
				break;
			}

			if (*string)
			    string++;
		    }
		}
		break;

	    case 'e':
		/* scan forward for a %; at level zero */
		string++;
		level = 0;
		while (*string) {
		    if (*string == '%') {
			string++;
			if (*string == '?')
			    level++;
			else if (*string == ';') {
			    if (level > 0)
				level--;
			    else
				break;
			}
		    }

		    if (*string)
			string++;
		}
		break;

	    case ';':
		break;

	    }			/* endswitch (*string) */
	} else {             	/* endelse (*string == '%') */
	    save_char(*string);
	}

	if (*string == '\0')
	    break;

	string++;
    }				/* endwhile (*string) */

    get_space(1);
    out_buff[out_used] = '\0';

    return (out_buff);
}

char *
grub_tparm(const char *string,...)
{
    char *result;
    int *dataptr = (int *) &string;

    dataptr++;

    result = tparam_internal(string, dataptr);

    return result;
}
