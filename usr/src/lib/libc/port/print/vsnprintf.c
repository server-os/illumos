/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved	*/

#include "lint.h"
#include <mtlib.h>
#include <stdarg.h>
#include <values.h>
#include <errno.h>
#include <synch.h>
#include <thread.h>
#include <sys/types.h>
#include "print.h"
#include "libc.h"

/*
 * 32-bit shadow function _vsnprintf_c89() is included here.
 * When using the c89 compiler to build 32-bit applications, the size
 * of intmax_t is 32-bits, otherwise the size of intmax_t is 64-bits.
 * The shadow function uses 32-bit size of intmax_t for j conversion.
 * The #pragma redefine_extname in <stdio.h> selects the proper routine
 * at compile time for the user application.
 * NOTE: the shadow function only exists in the 32-bit library.
 */

/*VARARGS2*/
int
#ifdef _C89_INTMAX32		/* _C89_INTMAX32 version in 32-bit libc only */
_vsnprintf_c89(char *string, size_t n, const char *format, va_list ap)
#else
vsnprintf(char *string, size_t n, const char *format, va_list ap)
#endif
{
	ssize_t count;
	FILE siop;
	size_t	max = MAXINT;
	unsigned char tmpbuf[1]; /* dummy buffer for _doprnt() if NULL string */

	siop._cnt = n - 1;
	siop._base = siop._ptr = (unsigned char *)string;
	siop._flag = _IOREAD;	/* distinguish dummy file descriptor */
#ifdef _LP64
	/*
	 * _bufend() (_realbufend()) should return NULL for v/snprintf,
	 * so PUT() macro in _doprnt() will bounds check.  For 32-bit,
	 * there is no _end field, and _realbufend() will return NULL
	 * since it cannot find the dummy FILE structure in the linked
	 * list of FILE strucutres.  See bug 4274368.
	 */
	siop._end = NULL;
#endif  /* _LP64 */

	if (n == 0) {
		/*
		 * When n==0, string may be NULL, so always use tmpbuf to
		 * guard for this case.  No bytes should be transmitted to
		 * the buffer when n==0, so using tmpbuf instead of string
		 * (ie, if string is not NULL) should not matter.
		 */
		siop._base = siop._ptr = tmpbuf;
		siop._cnt = 0;
	} else if (n > max) {
		errno = EOVERFLOW;
		return (EOF);
	}


#ifdef _C89_INTMAX32
	count = _ndoprnt(format, ap, &siop, _F_INTMAX32);
#else
	count = _ndoprnt(format, ap, &siop, 0);
#endif  /* _C89_INTMAX32 */

	*siop._ptr = '\0';	/* plant terminating null character */

	/* overflow check */
	if ((size_t)count > max) {
		errno = EOVERFLOW;
		return (EOF);
	} else {
		return ((int)count);
	}
}
