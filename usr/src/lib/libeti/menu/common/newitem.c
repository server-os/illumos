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
#include <stdlib.h>
#include <strings.h>
#include "private.h"

ITEM *
new_item(char *name, char *desc)
{
	ITEM *item;

	if (item = (ITEM *) calloc(1, sizeof (ITEM))) {
		/* Set all default values */
		*item = *Dfl_Item;

		/* And set user values */
		Name(item) = name;
		Description(item) = desc;

		if (name && *name != '\0') {
			NameLen(item) = strlen(name);
		} else {
			free(item);		/* Can't have a null name */
			return ((ITEM *) NULL);
		}
		if (desc && *desc != '\0') {
			DescriptionLen(item) = strlen(desc);
		} else {
			DescriptionLen(item) = 0;
		}
	}
	return (item);
}

int
free_item(ITEM *i)
{
	if (!i) {
		return (E_BAD_ARGUMENT);
	}
	/* Make sure none of the items have pointers to menus. */
	if (Imenu(i)) {
		return (E_CONNECTED);
	}
	free(i);
	return (E_OK);
}

char *
item_name(ITEM *i)
{
	if (i) {
		return (Name(i));
	}
	return (NULL);
}

char *
item_description(ITEM *i)
{
	if (i) {
		return (Description(i));
	}
	return (NULL);
}
