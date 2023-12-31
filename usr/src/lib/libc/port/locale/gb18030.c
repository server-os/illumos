/*
 * Copyright 2013 Garrett D'Amore <garrett@damore.org>
 * Copyright 2010 Nexenta Systems, Inc.  All rights reserved.
 * Copyright (c) 2002-2004 Tim J. Robbins
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * PRC National Standard GB 18030-2000 encoding of Chinese text.
 *
 * See gb18030(7) for details.
 */

#include "lint.h"
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "mblocal.h"
#include "lctype.h"


static size_t	_GB18030_mbrtowc(wchar_t *_RESTRICT_KYWD,
		    const char *_RESTRICT_KYWD,
		    size_t, mbstate_t *_RESTRICT_KYWD, boolean_t);
static int	_GB18030_mbsinit(const mbstate_t *);
static size_t	_GB18030_wcrtomb(char *_RESTRICT_KYWD, wchar_t,
		    mbstate_t *_RESTRICT_KYWD);
static size_t	_GB18030_mbsnrtowcs(wchar_t *_RESTRICT_KYWD,
		    const char **_RESTRICT_KYWD, size_t, size_t,
		    mbstate_t *_RESTRICT_KYWD);
static size_t	_GB18030_wcsnrtombs(char *_RESTRICT_KYWD,
		    const wchar_t **_RESTRICT_KYWD, size_t, size_t,
		    mbstate_t *_RESTRICT_KYWD);

void
_GB18030_init(struct lc_ctype *lct)
{

	lct->lc_mbrtowc = _GB18030_mbrtowc;
	lct->lc_wcrtomb = _GB18030_wcrtomb;
	lct->lc_mbsinit = _GB18030_mbsinit;
	lct->lc_mbsnrtowcs = _GB18030_mbsnrtowcs;
	lct->lc_wcsnrtombs = _GB18030_wcsnrtombs;
	lct->lc_max_mblen = 4;
	lct->lc_is_ascii = 0;
}

static int
_GB18030_mbsinit(const mbstate_t *ps)
{

	return (ps == NULL || ((const _GB18030State *)ps)->count == 0);
}

static size_t
_GB18030_mbrtowc(wchar_t *_RESTRICT_KYWD pwc, const char *_RESTRICT_KYWD s,
    size_t n, mbstate_t *_RESTRICT_KYWD ps, boolean_t zero)
{
	_GB18030State *gs;
	wchar_t wch;
	int ch, len, ocount;
	size_t ncopy;

	gs = (_GB18030State *)ps;

	if (gs->count < 0 || gs->count > sizeof (gs->bytes)) {
		errno = EINVAL;
		return ((size_t)-1);
	}

	if (s == NULL) {
		s = "";
		n = 1;
		pwc = NULL;
	}

	ncopy = MIN(MIN(n, MB_CUR_MAX), sizeof (gs->bytes) - gs->count);
	(void) memcpy(gs->bytes + gs->count, s, ncopy);
	ocount = gs->count;
	gs->count += ncopy;
	s = (char *)gs->bytes;
	n = gs->count;

	if (n == 0)
		/* Incomplete multibyte sequence */
		return ((size_t)-2);

	/*
	 * Single byte:		[00-7f]
	 * Two byte:		[81-fe][40-7e,80-fe]
	 * Four byte:		[81-fe][30-39][81-fe][30-39]
	 */
	ch = (unsigned char)*s++;
	if (ch <= 0x7f) {
		len = 1;
		wch = ch;
	} else if (ch >= 0x81 && ch <= 0xfe) {
		wch = ch;
		if (n < 2)
			return ((size_t)-2);
		ch = (unsigned char)*s++;
		if ((ch >= 0x40 && ch <= 0x7e) || (ch >= 0x80 && ch <= 0xfe)) {
			wch = (wch << 8) | ch;
			len = 2;
		} else if (ch >= 0x30 && ch <= 0x39) {
			/*
			 * Strip high bit off the wide character we will
			 * eventually output so that it is positive when
			 * cast to wint_t on 32-bit twos-complement machines.
			 */
			wch = ((wch & 0x7f) << 8) | ch;
			if (n < 3)
				return ((size_t)-2);
			ch = (unsigned char)*s++;
			if (ch < 0x81 || ch > 0xfe)
				goto ilseq;
			wch = (wch << 8) | ch;
			if (n < 4)
				return ((size_t)-2);
			ch = (unsigned char)*s++;
			if (ch < 0x30 || ch > 0x39)
				goto ilseq;
			wch = (wch << 8) | ch;
			len = 4;
		} else
			goto ilseq;
	} else
		goto ilseq;

	if (pwc != NULL)
		*pwc = wch;
	gs->count = 0;
	if (zero || wch != L'\0') {
		return (len - ocount);
	} else {
		return (0);
	}
ilseq:
	errno = EILSEQ;
	return ((size_t)-1);
}

static size_t
_GB18030_wcrtomb(char *_RESTRICT_KYWD s, wchar_t wc,
    mbstate_t *_RESTRICT_KYWD ps)
{
	_GB18030State *gs;
	size_t len;
	int c;

	gs = (_GB18030State *)ps;

	if (gs->count != 0) {
		errno = EINVAL;
		return ((size_t)-1);
	}

	if (s == NULL)
		/* Reset to initial shift state (no-op) */
		return (1);
	if ((wc & ~0x7fffffff) != 0)
		goto ilseq;
	if (wc & 0x7f000000) {
		/* Replace high bit that mbrtowc() removed. */
		wc |= 0x80000000;
		c = (wc >> 24) & 0xff;
		if (c < 0x81 || c > 0xfe)
			goto ilseq;
		*s++ = c;
		c = (wc >> 16) & 0xff;
		if (c < 0x30 || c > 0x39)
			goto ilseq;
		*s++ = c;
		c = (wc >> 8) & 0xff;
		if (c < 0x81 || c > 0xfe)
			goto ilseq;
		*s++ = c;
		c = wc & 0xff;
		if (c < 0x30 || c > 0x39)
			goto ilseq;
		*s++ = c;
		len = 4;
	} else if (wc & 0x00ff0000)
		goto ilseq;
	else if (wc & 0x0000ff00) {
		c = (wc >> 8) & 0xff;
		if (c < 0x81 || c > 0xfe)
			goto ilseq;
		*s++ = c;
		c = wc & 0xff;
		if (c < 0x40 || c == 0x7f || c == 0xff)
			goto ilseq;
		*s++ = c;
		len = 2;
	} else if (wc <= 0x7f) {
		*s++ = wc;
		len = 1;
	} else
		goto ilseq;

	return (len);
ilseq:
	errno = EILSEQ;
	return ((size_t)-1);
}

static size_t
_GB18030_mbsnrtowcs(wchar_t *_RESTRICT_KYWD dst,
    const char **_RESTRICT_KYWD src, size_t nms, size_t len,
    mbstate_t *_RESTRICT_KYWD ps)
{
	return (__mbsnrtowcs_std(dst, src, nms, len, ps, _GB18030_mbrtowc));
}

static size_t
_GB18030_wcsnrtombs(char *_RESTRICT_KYWD dst,
    const wchar_t **_RESTRICT_KYWD src, size_t nwc, size_t len,
    mbstate_t *_RESTRICT_KYWD ps)
{
	return (__wcsnrtombs_std(dst, src, nwc, len, ps, _GB18030_wcrtomb));
}
