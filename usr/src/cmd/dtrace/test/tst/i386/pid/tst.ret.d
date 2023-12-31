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
 * ASSERTION: Validated the emulation of various flavors of the return
 * instruction.
 */

#pragma D option destructive

pid$1:a.out:waiting:entry
{
	this->a = (char *)alloca(1);
	*this->a = 1;
	copyout(this->a, arg0, 1);
}

pid$1:a.out:ret*:
{
	printf("%%sp = %x", uregs[R_SP]);
}

pid$1:a.out:ret*:return
{
}

pid$1:a.out:done:entry
{
	exit(0);
}

pid$1:a.out:main:return
{
	printf("%%eax = %x", uregs[R_EAX]);
}

BEGIN
{
	/*
	 * Let's just do this for 5 seconds.
	 */
	timeout = timestamp + 5000000000;
}

profile:::tick-4
/timestamp > timeout/
{
	trace("test timed out");
	exit(1);
}

