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
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 1991-2006 Unicode, Inc. All rights reserved. Distributed under
 * the Terms of Use in http://www.unicode.org/copyright.html.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of the Unicode data files and any associated documentation (the
 * "Data Files") or Unicode software and any associated documentation (the
 * "Software") to deal in the Data Files or Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of the Data Files or Software, and
 * to permit persons to whom the Data Files or Software are furnished to do so,
 * provided that (a) the above copyright notice(s) and this permission notice
 * appear with all copies of the Data Files or Software, (b) both the above
 * copyright notice(s) and this permission notice appear in associated
 * documentation, and (c) there is clear notice in each modified Data File or
 * in the Software as well as in the documentation associated with the Data
 * File(s) or Software that the data or software has been modified.
 *
 * THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR
 * CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE DATA FILES OR SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder shall not
 * be used in advertising or otherwise to promote the sale, use or other
 * dealings in these Data Files or Software without prior written authorization
 * of the copyright holder.
 *
 * Unicode and the Unicode logo are trademarks of Unicode, Inc., and may be
 * registered in some jurisdictions. All other trademarks and registered
 * trademarks mentioned herein are the property of their respective owners.
 */
/*
 * This file has been modified by Sun Microsystems, Inc.
 */

#ifndef _SYS_KICONV_LATIN1_H
#define	_SYS_KICONV_LATIN1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/kiconv.h>

#ifdef	_KERNEL

/*
 * Mapping tables from CP1252, ISO 8859-1, ISO 8859-15, and CP850 to UTF-8.
 *
 * Indices are calculated by subtracting 0x80 from single byte character code
 * values.
 *
 * Each component has up to three UTF-8 character bytes; the first byte will
 * be used to figure out on how many bytes should be taken. If size figured
 * out has a value less than zero, that means that component isn't valid and
 * has no assigned character.
 */
static const kiconv_to_utf8_tbl_comp_t to_u8_tbl[4][128] = {
	{	/* CP1252 -> UTF-8 */
		{ 0xE2, 0x82, 0xAC },	/* 0x80 */
		{ 0xFE, 0xFE, 0xFE },	/* 0x81 (illegal character) */
		{ 0xE2, 0x80, 0x9A },	/* 0x82 */
		{ 0xC6, 0x92, 0x00 },	/* 0x83 */
		{ 0xE2, 0x80, 0x9E },	/* 0x84 */
		{ 0xE2, 0x80, 0xA6 },	/* 0x85 */
		{ 0xE2, 0x80, 0xA0 },	/* 0x86 */
		{ 0xE2, 0x80, 0xA1 },	/* 0x87 */
		{ 0xCB, 0x86, 0x00 },	/* 0x88 */
		{ 0xE2, 0x80, 0xB0 },	/* 0x89 */
		{ 0xC5, 0xA0, 0x00 },	/* 0x8A */
		{ 0xE2, 0x80, 0xB9 },	/* 0x8B */
		{ 0xC5, 0x92, 0x00 },	/* 0x8C */
		{ 0xFE, 0xFE, 0xFE },	/* 0x8D (illegal character) */
		{ 0xC5, 0xBD, 0x00 },	/* 0x8E */
		{ 0xFE, 0xFE, 0xFE },	/* 0x8F (illegal character) */
		{ 0xFE, 0xFE, 0xFE },	/* 0x90 (illegal character) */
		{ 0xE2, 0x80, 0x98 },	/* 0x91 */
		{ 0xE2, 0x80, 0x99 },	/* 0x92 */
		{ 0xE2, 0x80, 0x9C },	/* 0x93 */
		{ 0xE2, 0x80, 0x9D },	/* 0x94 */
		{ 0xE2, 0x80, 0xA2 },	/* 0x95 */
		{ 0xE2, 0x80, 0x93 },	/* 0x96 */
		{ 0xE2, 0x80, 0x94 },	/* 0x97 */
		{ 0xCB, 0x9C, 0x00 },	/* 0x98 */
		{ 0xE2, 0x84, 0xA2 },	/* 0x99 */
		{ 0xC5, 0xA1, 0x00 },	/* 0x9A */
		{ 0xE2, 0x80, 0xBA },	/* 0x9B */
		{ 0xC5, 0x93, 0x00 },	/* 0x9C */
		{ 0xFE, 0xFE, 0xFE },	/* 0x9D (illegal character) */
		{ 0xC5, 0xBE, 0x00 },	/* 0x9E */
		{ 0xC5, 0xB8, 0x00 },	/* 0x9F */
		{ 0xC2, 0xA0, 0x00 },	/* 0xA0 */
		{ 0xC2, 0xA1, 0x00 },	/* 0xA1 */
		{ 0xC2, 0xA2, 0x00 },	/* 0xA2 */
		{ 0xC2, 0xA3, 0x00 },	/* 0xA3 */
		{ 0xC2, 0xA4, 0x00 },	/* 0xA4 */
		{ 0xC2, 0xA5, 0x00 },	/* 0xA5 */
		{ 0xC2, 0xA6, 0x00 },	/* 0xA6 */
		{ 0xC2, 0xA7, 0x00 },	/* 0xA7 */
		{ 0xC2, 0xA8, 0x00 },	/* 0xA8 */
		{ 0xC2, 0xA9, 0x00 },	/* 0xA9 */
		{ 0xC2, 0xAA, 0x00 },	/* 0xAA */
		{ 0xC2, 0xAB, 0x00 },	/* 0xAB */
		{ 0xC2, 0xAC, 0x00 },	/* 0xAC */
		{ 0xC2, 0xAD, 0x00 },	/* 0xAD */
		{ 0xC2, 0xAE, 0x00 },	/* 0xAE */
		{ 0xC2, 0xAF, 0x00 },	/* 0xAF */
		{ 0xC2, 0xB0, 0x00 },	/* 0xB0 */
		{ 0xC2, 0xB1, 0x00 },	/* 0xB1 */
		{ 0xC2, 0xB2, 0x00 },	/* 0xB2 */
		{ 0xC2, 0xB3, 0x00 },	/* 0xB3 */
		{ 0xC2, 0xB4, 0x00 },	/* 0xB4 */
		{ 0xC2, 0xB5, 0x00 },	/* 0xB5 */
		{ 0xC2, 0xB6, 0x00 },	/* 0xB6 */
		{ 0xC2, 0xB7, 0x00 },	/* 0xB7 */
		{ 0xC2, 0xB8, 0x00 },	/* 0xB8 */
		{ 0xC2, 0xB9, 0x00 },	/* 0xB9 */
		{ 0xC2, 0xBA, 0x00 },	/* 0xBA */
		{ 0xC2, 0xBB, 0x00 },	/* 0xBB */
		{ 0xC2, 0xBC, 0x00 },	/* 0xBC */
		{ 0xC2, 0xBD, 0x00 },	/* 0xBD */
		{ 0xC2, 0xBE, 0x00 },	/* 0xBE */
		{ 0xC2, 0xBF, 0x00 },	/* 0xBF */
		{ 0xC3, 0x80, 0x00 },	/* 0xC0 */
		{ 0xC3, 0x81, 0x00 },	/* 0xC1 */
		{ 0xC3, 0x82, 0x00 },	/* 0xC2 */
		{ 0xC3, 0x83, 0x00 },	/* 0xC3 */
		{ 0xC3, 0x84, 0x00 },	/* 0xC4 */
		{ 0xC3, 0x85, 0x00 },	/* 0xC5 */
		{ 0xC3, 0x86, 0x00 },	/* 0xC6 */
		{ 0xC3, 0x87, 0x00 },	/* 0xC7 */
		{ 0xC3, 0x88, 0x00 },	/* 0xC8 */
		{ 0xC3, 0x89, 0x00 },	/* 0xC9 */
		{ 0xC3, 0x8A, 0x00 },	/* 0xCA */
		{ 0xC3, 0x8B, 0x00 },	/* 0xCB */
		{ 0xC3, 0x8C, 0x00 },	/* 0xCC */
		{ 0xC3, 0x8D, 0x00 },	/* 0xCD */
		{ 0xC3, 0x8E, 0x00 },	/* 0xCE */
		{ 0xC3, 0x8F, 0x00 },	/* 0xCF */
		{ 0xC3, 0x90, 0x00 },	/* 0xD0 */
		{ 0xC3, 0x91, 0x00 },	/* 0xD1 */
		{ 0xC3, 0x92, 0x00 },	/* 0xD2 */
		{ 0xC3, 0x93, 0x00 },	/* 0xD3 */
		{ 0xC3, 0x94, 0x00 },	/* 0xD4 */
		{ 0xC3, 0x95, 0x00 },	/* 0xD5 */
		{ 0xC3, 0x96, 0x00 },	/* 0xD6 */
		{ 0xC3, 0x97, 0x00 },	/* 0xD7 */
		{ 0xC3, 0x98, 0x00 },	/* 0xD8 */
		{ 0xC3, 0x99, 0x00 },	/* 0xD9 */
		{ 0xC3, 0x9A, 0x00 },	/* 0xDA */
		{ 0xC3, 0x9B, 0x00 },	/* 0xDB */
		{ 0xC3, 0x9C, 0x00 },	/* 0xDC */
		{ 0xC3, 0x9D, 0x00 },	/* 0xDD */
		{ 0xC3, 0x9E, 0x00 },	/* 0xDE */
		{ 0xC3, 0x9F, 0x00 },	/* 0xDF */
		{ 0xC3, 0xA0, 0x00 },	/* 0xE0 */
		{ 0xC3, 0xA1, 0x00 },	/* 0xE1 */
		{ 0xC3, 0xA2, 0x00 },	/* 0xE2 */
		{ 0xC3, 0xA3, 0x00 },	/* 0xE3 */
		{ 0xC3, 0xA4, 0x00 },	/* 0xE4 */
		{ 0xC3, 0xA5, 0x00 },	/* 0xE5 */
		{ 0xC3, 0xA6, 0x00 },	/* 0xE6 */
		{ 0xC3, 0xA7, 0x00 },	/* 0xE7 */
		{ 0xC3, 0xA8, 0x00 },	/* 0xE8 */
		{ 0xC3, 0xA9, 0x00 },	/* 0xE9 */
		{ 0xC3, 0xAA, 0x00 },	/* 0xEA */
		{ 0xC3, 0xAB, 0x00 },	/* 0xEB */
		{ 0xC3, 0xAC, 0x00 },	/* 0xEC */
		{ 0xC3, 0xAD, 0x00 },	/* 0xED */
		{ 0xC3, 0xAE, 0x00 },	/* 0xEE */
		{ 0xC3, 0xAF, 0x00 },	/* 0xEF */
		{ 0xC3, 0xB0, 0x00 },	/* 0xF0 */
		{ 0xC3, 0xB1, 0x00 },	/* 0xF1 */
		{ 0xC3, 0xB2, 0x00 },	/* 0xF2 */
		{ 0xC3, 0xB3, 0x00 },	/* 0xF3 */
		{ 0xC3, 0xB4, 0x00 },	/* 0xF4 */
		{ 0xC3, 0xB5, 0x00 },	/* 0xF5 */
		{ 0xC3, 0xB6, 0x00 },	/* 0xF6 */
		{ 0xC3, 0xB7, 0x00 },	/* 0xF7 */
		{ 0xC3, 0xB8, 0x00 },	/* 0xF8 */
		{ 0xC3, 0xB9, 0x00 },	/* 0xF9 */
		{ 0xC3, 0xBA, 0x00 },	/* 0xFA */
		{ 0xC3, 0xBB, 0x00 },	/* 0xFB */
		{ 0xC3, 0xBC, 0x00 },	/* 0xFC */
		{ 0xC3, 0xBD, 0x00 },	/* 0xFD */
		{ 0xC3, 0xBE, 0x00 },	/* 0xFE */
		{ 0xC3, 0xBF, 0x00 },	/* 0xFF */
	},
	{	/* ISO 8859-1 -> UTF-8 */
		{ 0xC2, 0x80, 0x00 },	/* 0x80 */
		{ 0xC2, 0x81, 0x00 },	/* 0x81 */
		{ 0xC2, 0x82, 0x00 },	/* 0x82 */
		{ 0xC2, 0x83, 0x00 },	/* 0x83 */
		{ 0xC2, 0x84, 0x00 },	/* 0x84 */
		{ 0xC2, 0x85, 0x00 },	/* 0x85 */
		{ 0xC2, 0x86, 0x00 },	/* 0x86 */
		{ 0xC2, 0x87, 0x00 },	/* 0x87 */
		{ 0xC2, 0x88, 0x00 },	/* 0x88 */
		{ 0xC2, 0x89, 0x00 },	/* 0x89 */
		{ 0xC2, 0x8A, 0x00 },	/* 0x8A */
		{ 0xC2, 0x8B, 0x00 },	/* 0x8B */
		{ 0xC2, 0x8C, 0x00 },	/* 0x8C */
		{ 0xC2, 0x8D, 0x00 },	/* 0x8D */
		{ 0xC2, 0x8E, 0x00 },	/* 0x8E */
		{ 0xC2, 0x8F, 0x00 },	/* 0x8F */
		{ 0xC2, 0x90, 0x00 },	/* 0x90 */
		{ 0xC2, 0x91, 0x00 },	/* 0x91 */
		{ 0xC2, 0x92, 0x00 },	/* 0x92 */
		{ 0xC2, 0x93, 0x00 },	/* 0x93 */
		{ 0xC2, 0x94, 0x00 },	/* 0x94 */
		{ 0xC2, 0x95, 0x00 },	/* 0x95 */
		{ 0xC2, 0x96, 0x00 },	/* 0x96 */
		{ 0xC2, 0x97, 0x00 },	/* 0x97 */
		{ 0xC2, 0x98, 0x00 },	/* 0x98 */
		{ 0xC2, 0x99, 0x00 },	/* 0x99 */
		{ 0xC2, 0x9A, 0x00 },	/* 0x9A */
		{ 0xC2, 0x9B, 0x00 },	/* 0x9B */
		{ 0xC2, 0x9C, 0x00 },	/* 0x9C */
		{ 0xC2, 0x9D, 0x00 },	/* 0x9D */
		{ 0xC2, 0x9E, 0x00 },	/* 0x9E */
		{ 0xC2, 0x9F, 0x00 },	/* 0x9F */
		{ 0xC2, 0xA0, 0x00 },	/* 0xA0 */
		{ 0xC2, 0xA1, 0x00 },	/* 0xA1 */
		{ 0xC2, 0xA2, 0x00 },	/* 0xA2 */
		{ 0xC2, 0xA3, 0x00 },	/* 0xA3 */
		{ 0xC2, 0xA4, 0x00 },	/* 0xA4 */
		{ 0xC2, 0xA5, 0x00 },	/* 0xA5 */
		{ 0xC2, 0xA6, 0x00 },	/* 0xA6 */
		{ 0xC2, 0xA7, 0x00 },	/* 0xA7 */
		{ 0xC2, 0xA8, 0x00 },	/* 0xA8 */
		{ 0xC2, 0xA9, 0x00 },	/* 0xA9 */
		{ 0xC2, 0xAA, 0x00 },	/* 0xAA */
		{ 0xC2, 0xAB, 0x00 },	/* 0xAB */
		{ 0xC2, 0xAC, 0x00 },	/* 0xAC */
		{ 0xC2, 0xAD, 0x00 },	/* 0xAD */
		{ 0xC2, 0xAE, 0x00 },	/* 0xAE */
		{ 0xC2, 0xAF, 0x00 },	/* 0xAF */
		{ 0xC2, 0xB0, 0x00 },	/* 0xB0 */
		{ 0xC2, 0xB1, 0x00 },	/* 0xB1 */
		{ 0xC2, 0xB2, 0x00 },	/* 0xB2 */
		{ 0xC2, 0xB3, 0x00 },	/* 0xB3 */
		{ 0xC2, 0xB4, 0x00 },	/* 0xB4 */
		{ 0xC2, 0xB5, 0x00 },	/* 0xB5 */
		{ 0xC2, 0xB6, 0x00 },	/* 0xB6 */
		{ 0xC2, 0xB7, 0x00 },	/* 0xB7 */
		{ 0xC2, 0xB8, 0x00 },	/* 0xB8 */
		{ 0xC2, 0xB9, 0x00 },	/* 0xB9 */
		{ 0xC2, 0xBA, 0x00 },	/* 0xBA */
		{ 0xC2, 0xBB, 0x00 },	/* 0xBB */
		{ 0xC2, 0xBC, 0x00 },	/* 0xBC */
		{ 0xC2, 0xBD, 0x00 },	/* 0xBD */
		{ 0xC2, 0xBE, 0x00 },	/* 0xBE */
		{ 0xC2, 0xBF, 0x00 },	/* 0xBF */
		{ 0xC3, 0x80, 0x00 },	/* 0xC0 */
		{ 0xC3, 0x81, 0x00 },	/* 0xC1 */
		{ 0xC3, 0x82, 0x00 },	/* 0xC2 */
		{ 0xC3, 0x83, 0x00 },	/* 0xC3 */
		{ 0xC3, 0x84, 0x00 },	/* 0xC4 */
		{ 0xC3, 0x85, 0x00 },	/* 0xC5 */
		{ 0xC3, 0x86, 0x00 },	/* 0xC6 */
		{ 0xC3, 0x87, 0x00 },	/* 0xC7 */
		{ 0xC3, 0x88, 0x00 },	/* 0xC8 */
		{ 0xC3, 0x89, 0x00 },	/* 0xC9 */
		{ 0xC3, 0x8A, 0x00 },	/* 0xCA */
		{ 0xC3, 0x8B, 0x00 },	/* 0xCB */
		{ 0xC3, 0x8C, 0x00 },	/* 0xCC */
		{ 0xC3, 0x8D, 0x00 },	/* 0xCD */
		{ 0xC3, 0x8E, 0x00 },	/* 0xCE */
		{ 0xC3, 0x8F, 0x00 },	/* 0xCF */
		{ 0xC3, 0x90, 0x00 },	/* 0xD0 */
		{ 0xC3, 0x91, 0x00 },	/* 0xD1 */
		{ 0xC3, 0x92, 0x00 },	/* 0xD2 */
		{ 0xC3, 0x93, 0x00 },	/* 0xD3 */
		{ 0xC3, 0x94, 0x00 },	/* 0xD4 */
		{ 0xC3, 0x95, 0x00 },	/* 0xD5 */
		{ 0xC3, 0x96, 0x00 },	/* 0xD6 */
		{ 0xC3, 0x97, 0x00 },	/* 0xD7 */
		{ 0xC3, 0x98, 0x00 },	/* 0xD8 */
		{ 0xC3, 0x99, 0x00 },	/* 0xD9 */
		{ 0xC3, 0x9A, 0x00 },	/* 0xDA */
		{ 0xC3, 0x9B, 0x00 },	/* 0xDB */
		{ 0xC3, 0x9C, 0x00 },	/* 0xDC */
		{ 0xC3, 0x9D, 0x00 },	/* 0xDD */
		{ 0xC3, 0x9E, 0x00 },	/* 0xDE */
		{ 0xC3, 0x9F, 0x00 },	/* 0xDF */
		{ 0xC3, 0xA0, 0x00 },	/* 0xE0 */
		{ 0xC3, 0xA1, 0x00 },	/* 0xE1 */
		{ 0xC3, 0xA2, 0x00 },	/* 0xE2 */
		{ 0xC3, 0xA3, 0x00 },	/* 0xE3 */
		{ 0xC3, 0xA4, 0x00 },	/* 0xE4 */
		{ 0xC3, 0xA5, 0x00 },	/* 0xE5 */
		{ 0xC3, 0xA6, 0x00 },	/* 0xE6 */
		{ 0xC3, 0xA7, 0x00 },	/* 0xE7 */
		{ 0xC3, 0xA8, 0x00 },	/* 0xE8 */
		{ 0xC3, 0xA9, 0x00 },	/* 0xE9 */
		{ 0xC3, 0xAA, 0x00 },	/* 0xEA */
		{ 0xC3, 0xAB, 0x00 },	/* 0xEB */
		{ 0xC3, 0xAC, 0x00 },	/* 0xEC */
		{ 0xC3, 0xAD, 0x00 },	/* 0xED */
		{ 0xC3, 0xAE, 0x00 },	/* 0xEE */
		{ 0xC3, 0xAF, 0x00 },	/* 0xEF */
		{ 0xC3, 0xB0, 0x00 },	/* 0xF0 */
		{ 0xC3, 0xB1, 0x00 },	/* 0xF1 */
		{ 0xC3, 0xB2, 0x00 },	/* 0xF2 */
		{ 0xC3, 0xB3, 0x00 },	/* 0xF3 */
		{ 0xC3, 0xB4, 0x00 },	/* 0xF4 */
		{ 0xC3, 0xB5, 0x00 },	/* 0xF5 */
		{ 0xC3, 0xB6, 0x00 },	/* 0xF6 */
		{ 0xC3, 0xB7, 0x00 },	/* 0xF7 */
		{ 0xC3, 0xB8, 0x00 },	/* 0xF8 */
		{ 0xC3, 0xB9, 0x00 },	/* 0xF9 */
		{ 0xC3, 0xBA, 0x00 },	/* 0xFA */
		{ 0xC3, 0xBB, 0x00 },	/* 0xFB */
		{ 0xC3, 0xBC, 0x00 },	/* 0xFC */
		{ 0xC3, 0xBD, 0x00 },	/* 0xFD */
		{ 0xC3, 0xBE, 0x00 },	/* 0xFE */
		{ 0xC3, 0xBF, 0x00 },	/* 0xFF */
	},
	{	/* ISO 8859-15 -> UTF-8 */
		{ 0xC2, 0x80, 0x00 },	/* 0x80 */
		{ 0xC2, 0x81, 0x00 },	/* 0x81 */
		{ 0xC2, 0x82, 0x00 },	/* 0x82 */
		{ 0xC2, 0x83, 0x00 },	/* 0x83 */
		{ 0xC2, 0x84, 0x00 },	/* 0x84 */
		{ 0xC2, 0x85, 0x00 },	/* 0x85 */
		{ 0xC2, 0x86, 0x00 },	/* 0x86 */
		{ 0xC2, 0x87, 0x00 },	/* 0x87 */
		{ 0xC2, 0x88, 0x00 },	/* 0x88 */
		{ 0xC2, 0x89, 0x00 },	/* 0x89 */
		{ 0xC2, 0x8A, 0x00 },	/* 0x8A */
		{ 0xC2, 0x8B, 0x00 },	/* 0x8B */
		{ 0xC2, 0x8C, 0x00 },	/* 0x8C */
		{ 0xC2, 0x8D, 0x00 },	/* 0x8D */
		{ 0xC2, 0x8E, 0x00 },	/* 0x8E */
		{ 0xC2, 0x8F, 0x00 },	/* 0x8F */
		{ 0xC2, 0x90, 0x00 },	/* 0x90 */
		{ 0xC2, 0x91, 0x00 },	/* 0x91 */
		{ 0xC2, 0x92, 0x00 },	/* 0x92 */
		{ 0xC2, 0x93, 0x00 },	/* 0x93 */
		{ 0xC2, 0x94, 0x00 },	/* 0x94 */
		{ 0xC2, 0x95, 0x00 },	/* 0x95 */
		{ 0xC2, 0x96, 0x00 },	/* 0x96 */
		{ 0xC2, 0x97, 0x00 },	/* 0x97 */
		{ 0xC2, 0x98, 0x00 },	/* 0x98 */
		{ 0xC2, 0x99, 0x00 },	/* 0x99 */
		{ 0xC2, 0x9A, 0x00 },	/* 0x9A */
		{ 0xC2, 0x9B, 0x00 },	/* 0x9B */
		{ 0xC2, 0x9C, 0x00 },	/* 0x9C */
		{ 0xC2, 0x9D, 0x00 },	/* 0x9D */
		{ 0xC2, 0x9E, 0x00 },	/* 0x9E */
		{ 0xC2, 0x9F, 0x00 },	/* 0x9F */
		{ 0xC2, 0xA0, 0x00 },	/* 0xA0 */
		{ 0xC2, 0xA1, 0x00 },	/* 0xA1 */
		{ 0xC2, 0xA2, 0x00 },	/* 0xA2 */
		{ 0xC2, 0xA3, 0x00 },	/* 0xA3 */
		{ 0xE2, 0x82, 0xAC },	/* 0xA4 */
		{ 0xC2, 0xA5, 0x00 },	/* 0xA5 */
		{ 0xC5, 0xA0, 0x00 },	/* 0xA6 */
		{ 0xC2, 0xA7, 0x00 },	/* 0xA7 */
		{ 0xC5, 0xA1, 0x00 },	/* 0xA8 */
		{ 0xC2, 0xA9, 0x00 },	/* 0xA9 */
		{ 0xC2, 0xAA, 0x00 },	/* 0xAA */
		{ 0xC2, 0xAB, 0x00 },	/* 0xAB */
		{ 0xC2, 0xAC, 0x00 },	/* 0xAC */
		{ 0xC2, 0xAD, 0x00 },	/* 0xAD */
		{ 0xC2, 0xAE, 0x00 },	/* 0xAE */
		{ 0xC2, 0xAF, 0x00 },	/* 0xAF */
		{ 0xC2, 0xB0, 0x00 },	/* 0xB0 */
		{ 0xC2, 0xB1, 0x00 },	/* 0xB1 */
		{ 0xC2, 0xB2, 0x00 },	/* 0xB2 */
		{ 0xC2, 0xB3, 0x00 },	/* 0xB3 */
		{ 0xC5, 0xBD, 0x00 },	/* 0xB4 */
		{ 0xC2, 0xB5, 0x00 },	/* 0xB5 */
		{ 0xC2, 0xB6, 0x00 },	/* 0xB6 */
		{ 0xC2, 0xB7, 0x00 },	/* 0xB7 */
		{ 0xC5, 0xBE, 0x00 },	/* 0xB8 */
		{ 0xC2, 0xB9, 0x00 },	/* 0xB9 */
		{ 0xC2, 0xBA, 0x00 },	/* 0xBA */
		{ 0xC2, 0xBB, 0x00 },	/* 0xBB */
		{ 0xC5, 0x92, 0x00 },	/* 0xBC */
		{ 0xC5, 0x93, 0x00 },	/* 0xBD */
		{ 0xC5, 0xB8, 0x00 },	/* 0xBE */
		{ 0xC2, 0xBF, 0x00 },	/* 0xBF */
		{ 0xC3, 0x80, 0x00 },	/* 0xC0 */
		{ 0xC3, 0x81, 0x00 },	/* 0xC1 */
		{ 0xC3, 0x82, 0x00 },	/* 0xC2 */
		{ 0xC3, 0x83, 0x00 },	/* 0xC3 */
		{ 0xC3, 0x84, 0x00 },	/* 0xC4 */
		{ 0xC3, 0x85, 0x00 },	/* 0xC5 */
		{ 0xC3, 0x86, 0x00 },	/* 0xC6 */
		{ 0xC3, 0x87, 0x00 },	/* 0xC7 */
		{ 0xC3, 0x88, 0x00 },	/* 0xC8 */
		{ 0xC3, 0x89, 0x00 },	/* 0xC9 */
		{ 0xC3, 0x8A, 0x00 },	/* 0xCA */
		{ 0xC3, 0x8B, 0x00 },	/* 0xCB */
		{ 0xC3, 0x8C, 0x00 },	/* 0xCC */
		{ 0xC3, 0x8D, 0x00 },	/* 0xCD */
		{ 0xC3, 0x8E, 0x00 },	/* 0xCE */
		{ 0xC3, 0x8F, 0x00 },	/* 0xCF */
		{ 0xC3, 0x90, 0x00 },	/* 0xD0 */
		{ 0xC3, 0x91, 0x00 },	/* 0xD1 */
		{ 0xC3, 0x92, 0x00 },	/* 0xD2 */
		{ 0xC3, 0x93, 0x00 },	/* 0xD3 */
		{ 0xC3, 0x94, 0x00 },	/* 0xD4 */
		{ 0xC3, 0x95, 0x00 },	/* 0xD5 */
		{ 0xC3, 0x96, 0x00 },	/* 0xD6 */
		{ 0xC3, 0x97, 0x00 },	/* 0xD7 */
		{ 0xC3, 0x98, 0x00 },	/* 0xD8 */
		{ 0xC3, 0x99, 0x00 },	/* 0xD9 */
		{ 0xC3, 0x9A, 0x00 },	/* 0xDA */
		{ 0xC3, 0x9B, 0x00 },	/* 0xDB */
		{ 0xC3, 0x9C, 0x00 },	/* 0xDC */
		{ 0xC3, 0x9D, 0x00 },	/* 0xDD */
		{ 0xC3, 0x9E, 0x00 },	/* 0xDE */
		{ 0xC3, 0x9F, 0x00 },	/* 0xDF */
		{ 0xC3, 0xA0, 0x00 },	/* 0xE0 */
		{ 0xC3, 0xA1, 0x00 },	/* 0xE1 */
		{ 0xC3, 0xA2, 0x00 },	/* 0xE2 */
		{ 0xC3, 0xA3, 0x00 },	/* 0xE3 */
		{ 0xC3, 0xA4, 0x00 },	/* 0xE4 */
		{ 0xC3, 0xA5, 0x00 },	/* 0xE5 */
		{ 0xC3, 0xA6, 0x00 },	/* 0xE6 */
		{ 0xC3, 0xA7, 0x00 },	/* 0xE7 */
		{ 0xC3, 0xA8, 0x00 },	/* 0xE8 */
		{ 0xC3, 0xA9, 0x00 },	/* 0xE9 */
		{ 0xC3, 0xAA, 0x00 },	/* 0xEA */
		{ 0xC3, 0xAB, 0x00 },	/* 0xEB */
		{ 0xC3, 0xAC, 0x00 },	/* 0xEC */
		{ 0xC3, 0xAD, 0x00 },	/* 0xED */
		{ 0xC3, 0xAE, 0x00 },	/* 0xEE */
		{ 0xC3, 0xAF, 0x00 },	/* 0xEF */
		{ 0xC3, 0xB0, 0x00 },	/* 0xF0 */
		{ 0xC3, 0xB1, 0x00 },	/* 0xF1 */
		{ 0xC3, 0xB2, 0x00 },	/* 0xF2 */
		{ 0xC3, 0xB3, 0x00 },	/* 0xF3 */
		{ 0xC3, 0xB4, 0x00 },	/* 0xF4 */
		{ 0xC3, 0xB5, 0x00 },	/* 0xF5 */
		{ 0xC3, 0xB6, 0x00 },	/* 0xF6 */
		{ 0xC3, 0xB7, 0x00 },	/* 0xF7 */
		{ 0xC3, 0xB8, 0x00 },	/* 0xF8 */
		{ 0xC3, 0xB9, 0x00 },	/* 0xF9 */
		{ 0xC3, 0xBA, 0x00 },	/* 0xFA */
		{ 0xC3, 0xBB, 0x00 },	/* 0xFB */
		{ 0xC3, 0xBC, 0x00 },	/* 0xFC */
		{ 0xC3, 0xBD, 0x00 },	/* 0xFD */
		{ 0xC3, 0xBE, 0x00 },	/* 0xFE */
		{ 0xC3, 0xBF, 0x00 },	/* 0xFF */
	},
	{	/* CP850 -> UTF-8 */
		{ 0xC3, 0x87, 0x00 },	/* 0x80 */
		{ 0xC3, 0xBC, 0x00 },	/* 0x81 */
		{ 0xC3, 0xA9, 0x00 },	/* 0x82 */
		{ 0xC3, 0xA2, 0x00 },	/* 0x83 */
		{ 0xC3, 0xA4, 0x00 },	/* 0x84 */
		{ 0xC3, 0xA0, 0x00 },	/* 0x85 */
		{ 0xC3, 0xA5, 0x00 },	/* 0x86 */
		{ 0xC3, 0xA7, 0x00 },	/* 0x87 */
		{ 0xC3, 0xAA, 0x00 },	/* 0x88 */
		{ 0xC3, 0xAB, 0x00 },	/* 0x89 */
		{ 0xC3, 0xA8, 0x00 },	/* 0x8A */
		{ 0xC3, 0xAF, 0x00 },	/* 0x8B */
		{ 0xC3, 0xAE, 0x00 },	/* 0x8C */
		{ 0xC3, 0xAC, 0x00 },	/* 0x8D */
		{ 0xC3, 0x84, 0x00 },	/* 0x8E */
		{ 0xC3, 0x85, 0x00 },	/* 0x8F */
		{ 0xC3, 0x89, 0x00 },	/* 0x90 */
		{ 0xC3, 0xA6, 0x00 },	/* 0x91 */
		{ 0xC3, 0x86, 0x00 },	/* 0x92 */
		{ 0xC3, 0xB4, 0x00 },	/* 0x93 */
		{ 0xC3, 0xB6, 0x00 },	/* 0x94 */
		{ 0xC3, 0xB2, 0x00 },	/* 0x95 */
		{ 0xC3, 0xBB, 0x00 },	/* 0x96 */
		{ 0xC3, 0xB9, 0x00 },	/* 0x97 */
		{ 0xC3, 0xBF, 0x00 },	/* 0x98 */
		{ 0xC3, 0x96, 0x00 },	/* 0x99 */
		{ 0xC3, 0x9C, 0x00 },	/* 0x9A */
		{ 0xC3, 0xB8, 0x00 },	/* 0x9B */
		{ 0xC2, 0xA3, 0x00 },	/* 0x9C */
		{ 0xC3, 0x98, 0x00 },	/* 0x9D */
		{ 0xC3, 0x97, 0x00 },	/* 0x9E */
		{ 0xC6, 0x92, 0x00 },	/* 0x9F */
		{ 0xC3, 0xA1, 0x00 },	/* 0xA0 */
		{ 0xC3, 0xAD, 0x00 },	/* 0xA1 */
		{ 0xC3, 0xB3, 0x00 },	/* 0xA2 */
		{ 0xC3, 0xBA, 0x00 },	/* 0xA3 */
		{ 0xC3, 0xB1, 0x00 },	/* 0xA4 */
		{ 0xC3, 0x91, 0x00 },	/* 0xA5 */
		{ 0xC2, 0xAA, 0x00 },	/* 0xA6 */
		{ 0xC2, 0xBA, 0x00 },	/* 0xA7 */
		{ 0xC2, 0xBF, 0x00 },	/* 0xA8 */
		{ 0xC2, 0xAE, 0x00 },	/* 0xA9 */
		{ 0xC2, 0xAC, 0x00 },	/* 0xAA */
		{ 0xC2, 0xBD, 0x00 },	/* 0xAB */
		{ 0xC2, 0xBC, 0x00 },	/* 0xAC */
		{ 0xC2, 0xA1, 0x00 },	/* 0xAD */
		{ 0xC2, 0xAB, 0x00 },	/* 0xAE */
		{ 0xC2, 0xBB, 0x00 },	/* 0xAF */
		{ 0xE2, 0x96, 0x91 },	/* 0xB0 */
		{ 0xE2, 0x96, 0x92 },	/* 0xB1 */
		{ 0xE2, 0x96, 0x93 },	/* 0xB2 */
		{ 0xE2, 0x94, 0x82 },	/* 0xB3 */
		{ 0xE2, 0x94, 0xA4 },	/* 0xB4 */
		{ 0xC3, 0x81, 0x00 },	/* 0xB5 */
		{ 0xC3, 0x82, 0x00 },	/* 0xB6 */
		{ 0xC3, 0x80, 0x00 },	/* 0xB7 */
		{ 0xC2, 0xA9, 0x00 },	/* 0xB8 */
		{ 0xE2, 0x95, 0xA3 },	/* 0xB9 */
		{ 0xE2, 0x95, 0x91 },	/* 0xBA */
		{ 0xE2, 0x95, 0x97 },	/* 0xBB */
		{ 0xE2, 0x95, 0x9D },	/* 0xBC */
		{ 0xC2, 0xA2, 0x00 },	/* 0xBD */
		{ 0xC2, 0xA5, 0x00 },	/* 0xBE */
		{ 0xE2, 0x94, 0x90 },	/* 0xBF */
		{ 0xE2, 0x94, 0x94 },	/* 0xC0 */
		{ 0xE2, 0x94, 0xB4 },	/* 0xC1 */
		{ 0xE2, 0x94, 0xAC },	/* 0xC2 */
		{ 0xE2, 0x94, 0x9C },	/* 0xC3 */
		{ 0xE2, 0x94, 0x80 },	/* 0xC4 */
		{ 0xE2, 0x94, 0xBC },	/* 0xC5 */
		{ 0xC3, 0xA3, 0x00 },	/* 0xC6 */
		{ 0xC3, 0x83, 0x00 },	/* 0xC7 */
		{ 0xE2, 0x95, 0x9A },	/* 0xC8 */
		{ 0xE2, 0x95, 0x94 },	/* 0xC9 */
		{ 0xE2, 0x95, 0xA9 },	/* 0xCA */
		{ 0xE2, 0x95, 0xA6 },	/* 0xCB */
		{ 0xE2, 0x95, 0xA0 },	/* 0xCC */
		{ 0xE2, 0x95, 0x90 },	/* 0xCD */
		{ 0xE2, 0x95, 0xAC },	/* 0xCE */
		{ 0xC2, 0xA4, 0x00 },	/* 0xCF */
		{ 0xC3, 0xB0, 0x00 },	/* 0xD0 */
		{ 0xC3, 0x90, 0x00 },	/* 0xD1 */
		{ 0xC3, 0x8A, 0x00 },	/* 0xD2 */
		{ 0xC3, 0x8B, 0x00 },	/* 0xD3 */
		{ 0xC3, 0x88, 0x00 },	/* 0xD4 */
		{ 0xC4, 0xB1, 0x00 },	/* 0xD5 */
		{ 0xC3, 0x8D, 0x00 },	/* 0xD6 */
		{ 0xC3, 0x8E, 0x00 },	/* 0xD7 */
		{ 0xC3, 0x8F, 0x00 },	/* 0xD8 */
		{ 0xE2, 0x94, 0x98 },	/* 0xD9 */
		{ 0xE2, 0x94, 0x8C },	/* 0xDA */
		{ 0xE2, 0x96, 0x88 },	/* 0xDB */
		{ 0xE2, 0x96, 0x84 },	/* 0xDC */
		{ 0xC2, 0xA6, 0x00 },	/* 0xDD */
		{ 0xC3, 0x8C, 0x00 },	/* 0xDE */
		{ 0xE2, 0x96, 0x80 },	/* 0xDF */
		{ 0xC3, 0x93, 0x00 },	/* 0xE0 */
		{ 0xC3, 0x9F, 0x00 },	/* 0xE1 */
		{ 0xC3, 0x94, 0x00 },	/* 0xE2 */
		{ 0xC3, 0x92, 0x00 },	/* 0xE3 */
		{ 0xC3, 0xB5, 0x00 },	/* 0xE4 */
		{ 0xC3, 0x95, 0x00 },	/* 0xE5 */
		{ 0xC2, 0xB5, 0x00 },	/* 0xE6 */
		{ 0xC3, 0xBE, 0x00 },	/* 0xE7 */
		{ 0xC3, 0x9E, 0x00 },	/* 0xE8 */
		{ 0xC3, 0x9A, 0x00 },	/* 0xE9 */
		{ 0xC3, 0x9B, 0x00 },	/* 0xEA */
		{ 0xC3, 0x99, 0x00 },	/* 0xEB */
		{ 0xC3, 0xBD, 0x00 },	/* 0xEC */
		{ 0xC3, 0x9D, 0x00 },	/* 0xED */
		{ 0xC2, 0xAF, 0x00 },	/* 0xEE */
		{ 0xC2, 0xB4, 0x00 },	/* 0xEF */
		{ 0xC2, 0xAD, 0x00 },	/* 0xF0 */
		{ 0xC2, 0xB1, 0x00 },	/* 0xF1 */
		{ 0xE2, 0x80, 0x97 },	/* 0xF2 */
		{ 0xC2, 0xBE, 0x00 },	/* 0xF3 */
		{ 0xC2, 0xB6, 0x00 },	/* 0xF4 */
		{ 0xC2, 0xA7, 0x00 },	/* 0xF5 */
		{ 0xC3, 0xB7, 0x00 },	/* 0xF6 */
		{ 0xC2, 0xB8, 0x00 },	/* 0xF7 */
		{ 0xC2, 0xB0, 0x00 },	/* 0xF8 */
		{ 0xC2, 0xA8, 0x00 },	/* 0xF9 */
		{ 0xC2, 0xB7, 0x00 },	/* 0xFA */
		{ 0xC2, 0xB9, 0x00 },	/* 0xFB */
		{ 0xC2, 0xB3, 0x00 },	/* 0xFC */
		{ 0xC2, 0xB2, 0x00 },	/* 0xFD */
		{ 0xE2, 0x96, 0xA0 },	/* 0xFE */
		{ 0xC2, 0xA0, 0x00 },	/* 0xFF */
	}
};

/*
 * Mapping tables from UTF-8 to CP1252, ISO 8859-1, ISO 8859-15, and CP850.
 *
 * Each table is sorted with UTF-8 values to be used with binary search.
 * UTF-8 character byte values are saved at uint32_t:24 contiguously as
 * scalar values.
 */
static const kiconv_to_sb_tbl_comp_t to_sb_tbl[4][128] = {
	{	/* UTF-8 -> CP1252 */
		{ 0x00C2A0, 0xA0 },
		{ 0x00C2A1, 0xA1 },
		{ 0x00C2A2, 0xA2 },
		{ 0x00C2A3, 0xA3 },
		{ 0x00C2A4, 0xA4 },
		{ 0x00C2A5, 0xA5 },
		{ 0x00C2A6, 0xA6 },
		{ 0x00C2A7, 0xA7 },
		{ 0x00C2A8, 0xA8 },
		{ 0x00C2A9, 0xA9 },
		{ 0x00C2AA, 0xAA },
		{ 0x00C2AB, 0xAB },
		{ 0x00C2AC, 0xAC },
		{ 0x00C2AD, 0xAD },
		{ 0x00C2AE, 0xAE },
		{ 0x00C2AF, 0xAF },
		{ 0x00C2B0, 0xB0 },
		{ 0x00C2B1, 0xB1 },
		{ 0x00C2B2, 0xB2 },
		{ 0x00C2B3, 0xB3 },
		{ 0x00C2B4, 0xB4 },
		{ 0x00C2B5, 0xB5 },
		{ 0x00C2B6, 0xB6 },
		{ 0x00C2B7, 0xB7 },
		{ 0x00C2B8, 0xB8 },
		{ 0x00C2B9, 0xB9 },
		{ 0x00C2BA, 0xBA },
		{ 0x00C2BB, 0xBB },
		{ 0x00C2BC, 0xBC },
		{ 0x00C2BD, 0xBD },
		{ 0x00C2BE, 0xBE },
		{ 0x00C2BF, 0xBF },
		{ 0x00C380, 0xC0 },
		{ 0x00C381, 0xC1 },
		{ 0x00C382, 0xC2 },
		{ 0x00C383, 0xC3 },
		{ 0x00C384, 0xC4 },
		{ 0x00C385, 0xC5 },
		{ 0x00C386, 0xC6 },
		{ 0x00C387, 0xC7 },
		{ 0x00C388, 0xC8 },
		{ 0x00C389, 0xC9 },
		{ 0x00C38A, 0xCA },
		{ 0x00C38B, 0xCB },
		{ 0x00C38C, 0xCC },
		{ 0x00C38D, 0xCD },
		{ 0x00C38E, 0xCE },
		{ 0x00C38F, 0xCF },
		{ 0x00C390, 0xD0 },
		{ 0x00C391, 0xD1 },
		{ 0x00C392, 0xD2 },
		{ 0x00C393, 0xD3 },
		{ 0x00C394, 0xD4 },
		{ 0x00C395, 0xD5 },
		{ 0x00C396, 0xD6 },
		{ 0x00C397, 0xD7 },
		{ 0x00C398, 0xD8 },
		{ 0x00C399, 0xD9 },
		{ 0x00C39A, 0xDA },
		{ 0x00C39B, 0xDB },
		{ 0x00C39C, 0xDC },
		{ 0x00C39D, 0xDD },
		{ 0x00C39E, 0xDE },
		{ 0x00C39F, 0xDF },
		{ 0x00C3A0, 0xE0 },
		{ 0x00C3A1, 0xE1 },
		{ 0x00C3A2, 0xE2 },
		{ 0x00C3A3, 0xE3 },
		{ 0x00C3A4, 0xE4 },
		{ 0x00C3A5, 0xE5 },
		{ 0x00C3A6, 0xE6 },
		{ 0x00C3A7, 0xE7 },
		{ 0x00C3A8, 0xE8 },
		{ 0x00C3A9, 0xE9 },
		{ 0x00C3AA, 0xEA },
		{ 0x00C3AB, 0xEB },
		{ 0x00C3AC, 0xEC },
		{ 0x00C3AD, 0xED },
		{ 0x00C3AE, 0xEE },
		{ 0x00C3AF, 0xEF },
		{ 0x00C3B0, 0xF0 },
		{ 0x00C3B1, 0xF1 },
		{ 0x00C3B2, 0xF2 },
		{ 0x00C3B3, 0xF3 },
		{ 0x00C3B4, 0xF4 },
		{ 0x00C3B5, 0xF5 },
		{ 0x00C3B6, 0xF6 },
		{ 0x00C3B7, 0xF7 },
		{ 0x00C3B8, 0xF8 },
		{ 0x00C3B9, 0xF9 },
		{ 0x00C3BA, 0xFA },
		{ 0x00C3BB, 0xFB },
		{ 0x00C3BC, 0xFC },
		{ 0x00C3BD, 0xFD },
		{ 0x00C3BE, 0xFE },
		{ 0x00C3BF, 0xFF },
		{ 0x00C592, 0x8C },
		{ 0x00C593, 0x9C },
		{ 0x00C5A0, 0x8A },
		{ 0x00C5A1, 0x9A },
		{ 0x00C5B8, 0x9F },
		{ 0x00C5BD, 0x8E },
		{ 0x00C5BE, 0x9E },
		{ 0x00C692, 0x83 },
		{ 0x00CB86, 0x88 },
		{ 0x00CB9C, 0x98 },
		{ 0xE28093, 0x96 },
		{ 0xE28094, 0x97 },
		{ 0xE28098, 0x91 },
		{ 0xE28099, 0x92 },
		{ 0xE2809A, 0x82 },
		{ 0xE2809C, 0x93 },
		{ 0xE2809D, 0x94 },
		{ 0xE2809E, 0x84 },
		{ 0xE280A0, 0x86 },
		{ 0xE280A1, 0x87 },
		{ 0xE280A2, 0x95 },
		{ 0xE280A6, 0x85 },
		{ 0xE280B0, 0x89 },
		{ 0xE280B9, 0x8B },
		{ 0xE280BA, 0x9B },
		{ 0xE282AC, 0x80 },
		{ 0xE284A2, 0x99 },
		{ 0xFFFFFF, 0x00 },
		{ 0xFFFFFF, 0x00 },
		{ 0xFFFFFF, 0x00 },
		{ 0xFFFFFF, 0x00 },
		{ 0xFFFFFF, 0x00 },
	},
	{	/* UTF-8 -> ISO 8859-1 */
		{ 0x00C280, 0x80 },
		{ 0x00C281, 0x81 },
		{ 0x00C282, 0x82 },
		{ 0x00C283, 0x83 },
		{ 0x00C284, 0x84 },
		{ 0x00C285, 0x85 },
		{ 0x00C286, 0x86 },
		{ 0x00C287, 0x87 },
		{ 0x00C288, 0x88 },
		{ 0x00C289, 0x89 },
		{ 0x00C28A, 0x8A },
		{ 0x00C28B, 0x8B },
		{ 0x00C28C, 0x8C },
		{ 0x00C28D, 0x8D },
		{ 0x00C28E, 0x8E },
		{ 0x00C28F, 0x8F },
		{ 0x00C290, 0x90 },
		{ 0x00C291, 0x91 },
		{ 0x00C292, 0x92 },
		{ 0x00C293, 0x93 },
		{ 0x00C294, 0x94 },
		{ 0x00C295, 0x95 },
		{ 0x00C296, 0x96 },
		{ 0x00C297, 0x97 },
		{ 0x00C298, 0x98 },
		{ 0x00C299, 0x99 },
		{ 0x00C29A, 0x9A },
		{ 0x00C29B, 0x9B },
		{ 0x00C29C, 0x9C },
		{ 0x00C29D, 0x9D },
		{ 0x00C29E, 0x9E },
		{ 0x00C29F, 0x9F },
		{ 0x00C2A0, 0xA0 },
		{ 0x00C2A1, 0xA1 },
		{ 0x00C2A2, 0xA2 },
		{ 0x00C2A3, 0xA3 },
		{ 0x00C2A4, 0xA4 },
		{ 0x00C2A5, 0xA5 },
		{ 0x00C2A6, 0xA6 },
		{ 0x00C2A7, 0xA7 },
		{ 0x00C2A8, 0xA8 },
		{ 0x00C2A9, 0xA9 },
		{ 0x00C2AA, 0xAA },
		{ 0x00C2AB, 0xAB },
		{ 0x00C2AC, 0xAC },
		{ 0x00C2AD, 0xAD },
		{ 0x00C2AE, 0xAE },
		{ 0x00C2AF, 0xAF },
		{ 0x00C2B0, 0xB0 },
		{ 0x00C2B1, 0xB1 },
		{ 0x00C2B2, 0xB2 },
		{ 0x00C2B3, 0xB3 },
		{ 0x00C2B4, 0xB4 },
		{ 0x00C2B5, 0xB5 },
		{ 0x00C2B6, 0xB6 },
		{ 0x00C2B7, 0xB7 },
		{ 0x00C2B8, 0xB8 },
		{ 0x00C2B9, 0xB9 },
		{ 0x00C2BA, 0xBA },
		{ 0x00C2BB, 0xBB },
		{ 0x00C2BC, 0xBC },
		{ 0x00C2BD, 0xBD },
		{ 0x00C2BE, 0xBE },
		{ 0x00C2BF, 0xBF },
		{ 0x00C380, 0xC0 },
		{ 0x00C381, 0xC1 },
		{ 0x00C382, 0xC2 },
		{ 0x00C383, 0xC3 },
		{ 0x00C384, 0xC4 },
		{ 0x00C385, 0xC5 },
		{ 0x00C386, 0xC6 },
		{ 0x00C387, 0xC7 },
		{ 0x00C388, 0xC8 },
		{ 0x00C389, 0xC9 },
		{ 0x00C38A, 0xCA },
		{ 0x00C38B, 0xCB },
		{ 0x00C38C, 0xCC },
		{ 0x00C38D, 0xCD },
		{ 0x00C38E, 0xCE },
		{ 0x00C38F, 0xCF },
		{ 0x00C390, 0xD0 },
		{ 0x00C391, 0xD1 },
		{ 0x00C392, 0xD2 },
		{ 0x00C393, 0xD3 },
		{ 0x00C394, 0xD4 },
		{ 0x00C395, 0xD5 },
		{ 0x00C396, 0xD6 },
		{ 0x00C397, 0xD7 },
		{ 0x00C398, 0xD8 },
		{ 0x00C399, 0xD9 },
		{ 0x00C39A, 0xDA },
		{ 0x00C39B, 0xDB },
		{ 0x00C39C, 0xDC },
		{ 0x00C39D, 0xDD },
		{ 0x00C39E, 0xDE },
		{ 0x00C39F, 0xDF },
		{ 0x00C3A0, 0xE0 },
		{ 0x00C3A1, 0xE1 },
		{ 0x00C3A2, 0xE2 },
		{ 0x00C3A3, 0xE3 },
		{ 0x00C3A4, 0xE4 },
		{ 0x00C3A5, 0xE5 },
		{ 0x00C3A6, 0xE6 },
		{ 0x00C3A7, 0xE7 },
		{ 0x00C3A8, 0xE8 },
		{ 0x00C3A9, 0xE9 },
		{ 0x00C3AA, 0xEA },
		{ 0x00C3AB, 0xEB },
		{ 0x00C3AC, 0xEC },
		{ 0x00C3AD, 0xED },
		{ 0x00C3AE, 0xEE },
		{ 0x00C3AF, 0xEF },
		{ 0x00C3B0, 0xF0 },
		{ 0x00C3B1, 0xF1 },
		{ 0x00C3B2, 0xF2 },
		{ 0x00C3B3, 0xF3 },
		{ 0x00C3B4, 0xF4 },
		{ 0x00C3B5, 0xF5 },
		{ 0x00C3B6, 0xF6 },
		{ 0x00C3B7, 0xF7 },
		{ 0x00C3B8, 0xF8 },
		{ 0x00C3B9, 0xF9 },
		{ 0x00C3BA, 0xFA },
		{ 0x00C3BB, 0xFB },
		{ 0x00C3BC, 0xFC },
		{ 0x00C3BD, 0xFD },
		{ 0x00C3BE, 0xFE },
		{ 0x00C3BF, 0xFF },
	},
	{	/* UTF-8 -> ISO 8859-15 */
		{ 0x00C280, 0x80 },
		{ 0x00C281, 0x81 },
		{ 0x00C282, 0x82 },
		{ 0x00C283, 0x83 },
		{ 0x00C284, 0x84 },
		{ 0x00C285, 0x85 },
		{ 0x00C286, 0x86 },
		{ 0x00C287, 0x87 },
		{ 0x00C288, 0x88 },
		{ 0x00C289, 0x89 },
		{ 0x00C28A, 0x8A },
		{ 0x00C28B, 0x8B },
		{ 0x00C28C, 0x8C },
		{ 0x00C28D, 0x8D },
		{ 0x00C28E, 0x8E },
		{ 0x00C28F, 0x8F },
		{ 0x00C290, 0x90 },
		{ 0x00C291, 0x91 },
		{ 0x00C292, 0x92 },
		{ 0x00C293, 0x93 },
		{ 0x00C294, 0x94 },
		{ 0x00C295, 0x95 },
		{ 0x00C296, 0x96 },
		{ 0x00C297, 0x97 },
		{ 0x00C298, 0x98 },
		{ 0x00C299, 0x99 },
		{ 0x00C29A, 0x9A },
		{ 0x00C29B, 0x9B },
		{ 0x00C29C, 0x9C },
		{ 0x00C29D, 0x9D },
		{ 0x00C29E, 0x9E },
		{ 0x00C29F, 0x9F },
		{ 0x00C2A0, 0xA0 },
		{ 0x00C2A1, 0xA1 },
		{ 0x00C2A2, 0xA2 },
		{ 0x00C2A3, 0xA3 },
		{ 0x00C2A5, 0xA5 },
		{ 0x00C2A7, 0xA7 },
		{ 0x00C2A9, 0xA9 },
		{ 0x00C2AA, 0xAA },
		{ 0x00C2AB, 0xAB },
		{ 0x00C2AC, 0xAC },
		{ 0x00C2AD, 0xAD },
		{ 0x00C2AE, 0xAE },
		{ 0x00C2AF, 0xAF },
		{ 0x00C2B0, 0xB0 },
		{ 0x00C2B1, 0xB1 },
		{ 0x00C2B2, 0xB2 },
		{ 0x00C2B3, 0xB3 },
		{ 0x00C2B5, 0xB5 },
		{ 0x00C2B6, 0xB6 },
		{ 0x00C2B7, 0xB7 },
		{ 0x00C2B9, 0xB9 },
		{ 0x00C2BA, 0xBA },
		{ 0x00C2BB, 0xBB },
		{ 0x00C2BF, 0xBF },
		{ 0x00C380, 0xC0 },
		{ 0x00C381, 0xC1 },
		{ 0x00C382, 0xC2 },
		{ 0x00C383, 0xC3 },
		{ 0x00C384, 0xC4 },
		{ 0x00C385, 0xC5 },
		{ 0x00C386, 0xC6 },
		{ 0x00C387, 0xC7 },
		{ 0x00C388, 0xC8 },
		{ 0x00C389, 0xC9 },
		{ 0x00C38A, 0xCA },
		{ 0x00C38B, 0xCB },
		{ 0x00C38C, 0xCC },
		{ 0x00C38D, 0xCD },
		{ 0x00C38E, 0xCE },
		{ 0x00C38F, 0xCF },
		{ 0x00C390, 0xD0 },
		{ 0x00C391, 0xD1 },
		{ 0x00C392, 0xD2 },
		{ 0x00C393, 0xD3 },
		{ 0x00C394, 0xD4 },
		{ 0x00C395, 0xD5 },
		{ 0x00C396, 0xD6 },
		{ 0x00C397, 0xD7 },
		{ 0x00C398, 0xD8 },
		{ 0x00C399, 0xD9 },
		{ 0x00C39A, 0xDA },
		{ 0x00C39B, 0xDB },
		{ 0x00C39C, 0xDC },
		{ 0x00C39D, 0xDD },
		{ 0x00C39E, 0xDE },
		{ 0x00C39F, 0xDF },
		{ 0x00C3A0, 0xE0 },
		{ 0x00C3A1, 0xE1 },
		{ 0x00C3A2, 0xE2 },
		{ 0x00C3A3, 0xE3 },
		{ 0x00C3A4, 0xE4 },
		{ 0x00C3A5, 0xE5 },
		{ 0x00C3A6, 0xE6 },
		{ 0x00C3A7, 0xE7 },
		{ 0x00C3A8, 0xE8 },
		{ 0x00C3A9, 0xE9 },
		{ 0x00C3AA, 0xEA },
		{ 0x00C3AB, 0xEB },
		{ 0x00C3AC, 0xEC },
		{ 0x00C3AD, 0xED },
		{ 0x00C3AE, 0xEE },
		{ 0x00C3AF, 0xEF },
		{ 0x00C3B0, 0xF0 },
		{ 0x00C3B1, 0xF1 },
		{ 0x00C3B2, 0xF2 },
		{ 0x00C3B3, 0xF3 },
		{ 0x00C3B4, 0xF4 },
		{ 0x00C3B5, 0xF5 },
		{ 0x00C3B6, 0xF6 },
		{ 0x00C3B7, 0xF7 },
		{ 0x00C3B8, 0xF8 },
		{ 0x00C3B9, 0xF9 },
		{ 0x00C3BA, 0xFA },
		{ 0x00C3BB, 0xFB },
		{ 0x00C3BC, 0xFC },
		{ 0x00C3BD, 0xFD },
		{ 0x00C3BE, 0xFE },
		{ 0x00C3BF, 0xFF },
		{ 0x00C592, 0xBC },
		{ 0x00C593, 0xBD },
		{ 0x00C5A0, 0xA6 },
		{ 0x00C5A1, 0xA8 },
		{ 0x00C5B8, 0xBE },
		{ 0x00C5BD, 0xB4 },
		{ 0x00C5BE, 0xB8 },
		{ 0xE282AC, 0xA4 },
	},
	{	/* UTF-8 -> CP850 */
		{ 0x00C2A0, 0xFF },
		{ 0x00C2A1, 0xAD },
		{ 0x00C2A2, 0xBD },
		{ 0x00C2A3, 0x9C },
		{ 0x00C2A4, 0xCF },
		{ 0x00C2A5, 0xBE },
		{ 0x00C2A6, 0xDD },
		{ 0x00C2A7, 0xF5 },
		{ 0x00C2A8, 0xF9 },
		{ 0x00C2A9, 0xB8 },
		{ 0x00C2AA, 0xA6 },
		{ 0x00C2AB, 0xAE },
		{ 0x00C2AC, 0xAA },
		{ 0x00C2AD, 0xF0 },
		{ 0x00C2AE, 0xA9 },
		{ 0x00C2AF, 0xEE },
		{ 0x00C2B0, 0xF8 },
		{ 0x00C2B1, 0xF1 },
		{ 0x00C2B2, 0xFD },
		{ 0x00C2B3, 0xFC },
		{ 0x00C2B4, 0xEF },
		{ 0x00C2B5, 0xE6 },
		{ 0x00C2B6, 0xF4 },
		{ 0x00C2B7, 0xFA },
		{ 0x00C2B8, 0xF7 },
		{ 0x00C2B9, 0xFB },
		{ 0x00C2BA, 0xA7 },
		{ 0x00C2BB, 0xAF },
		{ 0x00C2BC, 0xAC },
		{ 0x00C2BD, 0xAB },
		{ 0x00C2BE, 0xF3 },
		{ 0x00C2BF, 0xA8 },
		{ 0x00C380, 0xB7 },
		{ 0x00C381, 0xB5 },
		{ 0x00C382, 0xB6 },
		{ 0x00C383, 0xC7 },
		{ 0x00C384, 0x8E },
		{ 0x00C385, 0x8F },
		{ 0x00C386, 0x92 },
		{ 0x00C387, 0x80 },
		{ 0x00C388, 0xD4 },
		{ 0x00C389, 0x90 },
		{ 0x00C38A, 0xD2 },
		{ 0x00C38B, 0xD3 },
		{ 0x00C38C, 0xDE },
		{ 0x00C38D, 0xD6 },
		{ 0x00C38E, 0xD7 },
		{ 0x00C38F, 0xD8 },
		{ 0x00C390, 0xD1 },
		{ 0x00C391, 0xA5 },
		{ 0x00C392, 0xE3 },
		{ 0x00C393, 0xE0 },
		{ 0x00C394, 0xE2 },
		{ 0x00C395, 0xE5 },
		{ 0x00C396, 0x99 },
		{ 0x00C397, 0x9E },
		{ 0x00C398, 0x9D },
		{ 0x00C399, 0xEB },
		{ 0x00C39A, 0xE9 },
		{ 0x00C39B, 0xEA },
		{ 0x00C39C, 0x9A },
		{ 0x00C39D, 0xED },
		{ 0x00C39E, 0xE8 },
		{ 0x00C39F, 0xE1 },
		{ 0x00C3A0, 0x85 },
		{ 0x00C3A1, 0xA0 },
		{ 0x00C3A2, 0x83 },
		{ 0x00C3A3, 0xC6 },
		{ 0x00C3A4, 0x84 },
		{ 0x00C3A5, 0x86 },
		{ 0x00C3A6, 0x91 },
		{ 0x00C3A7, 0x87 },
		{ 0x00C3A8, 0x8A },
		{ 0x00C3A9, 0x82 },
		{ 0x00C3AA, 0x88 },
		{ 0x00C3AB, 0x89 },
		{ 0x00C3AC, 0x8D },
		{ 0x00C3AD, 0xA1 },
		{ 0x00C3AE, 0x8C },
		{ 0x00C3AF, 0x8B },
		{ 0x00C3B0, 0xD0 },
		{ 0x00C3B1, 0xA4 },
		{ 0x00C3B2, 0x95 },
		{ 0x00C3B3, 0xA2 },
		{ 0x00C3B4, 0x93 },
		{ 0x00C3B5, 0xE4 },
		{ 0x00C3B6, 0x94 },
		{ 0x00C3B7, 0xF6 },
		{ 0x00C3B8, 0x9B },
		{ 0x00C3B9, 0x97 },
		{ 0x00C3BA, 0xA3 },
		{ 0x00C3BB, 0x96 },
		{ 0x00C3BC, 0x81 },
		{ 0x00C3BD, 0xEC },
		{ 0x00C3BE, 0xE7 },
		{ 0x00C3BF, 0x98 },
		{ 0x00C4B1, 0xD5 },
		{ 0x00C692, 0x9F },
		{ 0xE28097, 0xF2 },
		{ 0xE29480, 0xC4 },
		{ 0xE29482, 0xB3 },
		{ 0xE2948C, 0xDA },
		{ 0xE29490, 0xBF },
		{ 0xE29494, 0xC0 },
		{ 0xE29498, 0xD9 },
		{ 0xE2949C, 0xC3 },
		{ 0xE294A4, 0xB4 },
		{ 0xE294AC, 0xC2 },
		{ 0xE294B4, 0xC1 },
		{ 0xE294BC, 0xC5 },
		{ 0xE29590, 0xCD },
		{ 0xE29591, 0xBA },
		{ 0xE29594, 0xC9 },
		{ 0xE29597, 0xBB },
		{ 0xE2959A, 0xC8 },
		{ 0xE2959D, 0xBC },
		{ 0xE295A0, 0xCC },
		{ 0xE295A3, 0xB9 },
		{ 0xE295A6, 0xCB },
		{ 0xE295A9, 0xCA },
		{ 0xE295AC, 0xCE },
		{ 0xE29680, 0xDF },
		{ 0xE29684, 0xDC },
		{ 0xE29688, 0xDB },
		{ 0xE29691, 0xB0 },
		{ 0xE29692, 0xB1 },
		{ 0xE29693, 0xB2 },
		{ 0xE296A0, 0xFE },
	}
};

#endif	/* _KERNEL */

#ifdef __cplusplus
}
#endif

#endif /* _SYS_KICONV_LATIN1_H */
