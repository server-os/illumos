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
 * String conversion routine for relocation types.
 */
#include	<stdio.h>
#include	<sys/elf_amd64.h>
#include	"relocate_amd64_msg.h"
#include	"_conv.h"

/*
 * AMD64 specific relocations.
 */
static const Msg rels[R_AMD64_NUM] = {
	[R_AMD64_NONE]		= MSG_R_AMD64_NONE,
	[R_AMD64_64]		= MSG_R_AMD64_64,
	[R_AMD64_PC32]		= MSG_R_AMD64_PC32,
	[R_AMD64_GOT32]		= MSG_R_AMD64_GOT32,
	[R_AMD64_PLT32]		= MSG_R_AMD64_PLT32,
	[R_AMD64_COPY]		= MSG_R_AMD64_COPY,
	[R_AMD64_GLOB_DAT]	= MSG_R_AMD64_GLOB_DAT,
	[R_AMD64_JUMP_SLOT]	= MSG_R_AMD64_JUMP_SLOT,
	[R_AMD64_RELATIVE]	= MSG_R_AMD64_RELATIVE,
	[R_AMD64_GOTPCREL]	= MSG_R_AMD64_GOTPCREL,
	[R_AMD64_32]		= MSG_R_AMD64_32,
	[R_AMD64_32S]		= MSG_R_AMD64_32S,
	[R_AMD64_16]		= MSG_R_AMD64_16,
	[R_AMD64_PC16]		= MSG_R_AMD64_PC16,
	[R_AMD64_8]		= MSG_R_AMD64_8,
	[R_AMD64_PC8]		= MSG_R_AMD64_PC8,
	[R_AMD64_DTPMOD64]	= MSG_R_AMD64_DTPMOD64,
	[R_AMD64_DTPOFF64]	= MSG_R_AMD64_DTPOFF64,
	[R_AMD64_TPOFF64]	= MSG_R_AMD64_TPOFF64,
	[R_AMD64_TLSGD]		= MSG_R_AMD64_TLSGD,
	[R_AMD64_TLSLD]		= MSG_R_AMD64_TLSLD,
	[R_AMD64_DTPOFF32]	= MSG_R_AMD64_DTPOFF32,
	[R_AMD64_GOTTPOFF]	= MSG_R_AMD64_GOTTPOFF,
	[R_AMD64_TPOFF32]	= MSG_R_AMD64_TPOFF32,
	[R_AMD64_PC64]		= MSG_R_AMD64_PC64,
	[R_AMD64_GOTOFF64]	= MSG_R_AMD64_GOTOFF64,
	[R_AMD64_GOTPC32]	= MSG_R_AMD64_GOTPC32,
	[R_AMD64_GOT64]		= MSG_R_AMD64_GOT64,
	[R_AMD64_GOTPCREL64]	= MSG_R_AMD64_GOTPCREL64,
	[R_AMD64_GOTPC64]	= MSG_R_AMD64_GOTPC64,
	[R_AMD64_GOTPLT64]	= MSG_R_AMD64_GOTPLT64,
	[R_AMD64_PLTOFF64]	= MSG_R_AMD64_PLTOFF64,
	[R_AMD64_SIZE32]	= MSG_R_AMD64_SIZE32,
	[R_AMD64_SIZE64]	= MSG_R_AMD64_SIZE64,
	[R_AMD64_GOTPC32_TLSDESC] = MSG_R_AMD64_GOTPC32_TLSDESC,
	[R_AMD64_TLSDESC_CALL]	= MSG_R_AMD64_TLSDESC_CALL,
	[R_AMD64_TLSDESC]	= MSG_R_AMD64_TLSDESC,
	[R_AMD64_IRELATIVE]	= MSG_R_AMD64_IRELATIVE,
	[R_AMD64_RELATIVE64]	= MSG_R_AMD64_RELATIVE64,
	[R_AMD64_UNKNOWN39]	= MSG_R_AMD64_UNKNOWN39,
	[R_AMD64_UNKNOWN40]	= MSG_R_AMD64_UNKNOWN40,
	[R_AMD64_GOTPCRELX]	= MSG_R_AMD64_GOTPCRELX,
	[R_AMD64_REX_GOTPCRELX] = MSG_R_AMD64_REX_GOTPCRELX
};

#if	(R_AMD64_NUM != (R_AMD64_REX_GOTPCRELX + 1))
#error	"R_AMD64_NUM has grown"
#endif

const char *
conv_reloc_amd64_type(Word type, Conv_fmt_flags_t fmt_flags,
    Conv_inv_buf_t *inv_buf)
{
	if (type >= R_AMD64_NUM)
		return (conv_invalid_val(inv_buf, type, fmt_flags));
	return (MSG_ORIG(rels[type]));
}
