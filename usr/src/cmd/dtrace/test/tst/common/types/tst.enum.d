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

/*
 * ASSERTION:
 *  Positive enumeration test
 *
 * SECTION: Type and Constant Definitions/Enumerations
 */

#pragma D option quiet

enum my_enum {
	zero,
	one = 1,
	two,
	three,
	four = 4,
	minimum = -2147483648,
	maximum = 2147483647
};

enum my_enum2 {
	zero2,
	one2 = 1,
	two2,
	three2,
	four2 = 4,
	minimum2 = -2147483648,
	maximum2 = 2147483647,
};

BEGIN
{
	exit(0);
}
