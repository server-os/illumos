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

#pragma D option quiet

BEGIN
{
	a = 7;
	b = 13;
	val = (-a * b) + a;
}

tick-1ms
{
	incr = val % b;
	val += a;
}

tick-1ms
/val == 0/
{
	val += a;
}

tick-1ms
/incr != 0/
{
	i++;
	@quanty[i] = quantize(1, incr);
	@lquanty[i] = lquantize(1, -10, 10, 1, incr);
	@summy[i] = sum(incr);
	@maxxy[i] = max(incr);
	@minny[i] = min(incr);
}

tick-1ms
/incr == 0/
{
	printf("Ordering of quantize() with some negative weights:\n");
	printa(@quanty);
	printf("\n");

	printf("Ordering of lquantize() with some negative weights:\n");
	printa(@lquanty);
	printf("\n");

	printf("Ordering of sum() with some negative weights:\n");
	printa(@summy);
	printf("\n");

	printf("Ordering of max() with some negative weights:\n");
	printa(@maxxy);
	printf("\n");

	printf("Ordering of min() with some negative weights:\n");
	printa(@minny);
	printf("\n");

	exit(0);
}
