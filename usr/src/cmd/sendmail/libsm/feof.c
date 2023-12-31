/*
 * Copyright (c) 2000-2001 Sendmail, Inc. and its suppliers.
 *      All rights reserved.
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the sendmail distribution.
 */

#include <sm/gen.h>
SM_RCSID("@(#)$Id: feof.c,v 1.11 2001/04/03 01:46:40 ca Exp $")
#include <sm/io.h>
#include <sm/assert.h>
#include "local.h"

/*
**  SM_IO_EOF -- subroutine version of the macro sm_io_eof.
**
**  Tests if the file for 'fp' has reached the end.
**
**	Parameters:
**		fp -- file pointer.
**
**	Returns:
**		0 (zero) when the file is not at end
**		non-zero when EOF has been found
*/
#undef sm_io_eof

int
sm_io_eof(fp)
	SM_FILE_T *fp;
{
	SM_REQUIRE_ISA(fp, SmFileMagic);

	return sm_eof(fp);
}
