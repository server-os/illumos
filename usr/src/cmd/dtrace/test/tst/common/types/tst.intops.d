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
 *	Verify relational operators with integers
 *
 * SECTION: Types, Operators, and Expressions/Relational Operators;
 * 	Types, Operators, and Expressions/Precedence
 *
 */

#pragma D option quiet


BEGIN
{
	int_1 = 0x100;
	int_2 = 0x101;
	int_3 = 0x99;
}

tick-1
/int_1 >= int_2 || int_2 <= int_1 || int_1 == int_2/
{
	printf("Shouldn't end up here (1)\n");
	printf("int_1 = %x int_2 = %x int_3 = %x\n",
		(int) int_1, (int) int_2, (int) int_3);
	exit(1);
}

tick-1
/int_3 > int_1 || int_1 < int_3 || int_3 == int_1/
{
	printf("Shouldn't end up here (2)\n");
	printf("int_1 = %x int_2 = %x int_3 = %x\n",
		(int) int_1, (int) int_2, (int) int_3);
	exit(1);
}

tick-1
/int_2 > int_3 && int_1 < int_2 ^^ int_3 == int_2 && !(int_1 != int_2)/
{
	exit(0);
}
