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
/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*
	Set process group ID to this process ID and exec the command line
	that is the argument list.
*/


#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	char	*cmd;

	cmd = *argv;
	if (argc <= 1)  {
		(void) fprintf(stderr, "Usage:  %s command [ arg ... ]\n", cmd);
		exit(1);
	}
	argv++;
	argc--;
	(void) setpgrp();
	(void) execvp(*argv, argv);
	(void) fprintf(stderr, "%s: %s not executed.  ", cmd, *argv);
	perror("");
	return (1);
}
