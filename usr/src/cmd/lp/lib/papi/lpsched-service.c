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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*LINTLIBRARY*/

#include <stdlib.h>
#include <libintl.h>
#include <papi_impl.h>

#include "form.h"


void
lpsched_service_information(papi_attribute_t ***attrs)
{
	FORM	form;

	while (getform(NAME_ALL, &form, (FALERT *)0, (FILE **)0) != -1) {
		papiAttributeListAddString(attrs, PAPI_ATTR_APPEND,
				"form-supported", form.name);
		freeform(&form);
	}
}
