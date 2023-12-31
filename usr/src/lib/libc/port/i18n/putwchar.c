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
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1986 AT&T	*/
/*	  All Rights Reserved  	*/


/*	This module is created for NLS on Sep.03.86		*/

/*
 * A subroutine version of the macro putchar
 */

#include "lint.h"
#include "mse_int.h"
#include <stdio.h>
#include <stdlib.h>
#include <widec.h>
#include "libc.h"

#undef putwchar

wint_t
putwchar(wint_t c)
{
	return (_putwc(c, stdout));
}

wint_t
__putwchar_xpg5(wint_t c)
{
	return (__putwc_xpg5(c, stdout));
}
