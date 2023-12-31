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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * ASSERTION: Test the normal behavior of speculate() and commit().
 *
 * SECTION: Speculative Tracing/Committing a Speculation;
 *	Actions and Subroutines/speculation();
 *	Options and Tunables/cleanrate
 *
 */
#pragma D option quiet
#pragma D option cleanrate=2000hz

BEGIN
{
	self->var = speculation();
	printf("Speculation ID: %d\n", self->var);
	self->speculate = 0;
	self->commit = 0;
}

BEGIN
/1 > self->speculate/
{
	speculate(self->var);
	self->speculate++;
	printf("Called speculate on id: %d\n", self->var);
}

BEGIN
/1 <= self->speculate/
{
	commit(self->var);
	self->commit++;
}

BEGIN
/(1 == self->commit)/
{
	printf("Succesfully tested buffer commit\n");
	exit(0);
}

BEGIN
/(0 == self->commit)/
{
	printf("Failed to commit buffer\n");
	exit(1);
}
