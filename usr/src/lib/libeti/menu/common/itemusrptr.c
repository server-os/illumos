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
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/


/*
 * Copyright (c) 1997, by Sun Mircrosystems, Inc.
 * All rights reserved.
 */

/*LINTLIBRARY*/

#include <sys/types.h>
#include "private.h"

int
set_item_userptr(ITEM *i, char *u)
{
	if (i) {
		Iuserptr(i) = u;
	} else {
		Iuserptr(Dfl_Item) = u;
	}
	return (E_OK);
}

char *
item_userptr(ITEM *i)
{
	return (Iuserptr(i ? i : Dfl_Item));
}
