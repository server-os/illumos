/*
 * Copyright 2002 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)encrypt.h	8.1 (Berkeley) 6/4/93
 */

/*
 * Copyright (C) 1990 by the Massachusetts Institute of Technology
 *
 * Export of this software from the United States of America may
 * require a specific license from the United States Government.
 * It is the responsibility of any person or organization contemplating
 * export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */

#ifndef	_ENCRYPT_H
#define	_ENCRYPT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	ENCR_LBUF_BUFSIZ	32	  /* short temporary buffer */
#define	SAMEKEY(k1, k2)	(!memcmp((void *)k1, (void *)k2, sizeof (Block))

#include <arpa/telnet.h>

typedef struct {
	char	*name;
	int	type;
	void	(*output)(unsigned char *, int);
	int	(*input)(int);
	void	(*init)();
	int	(*start)(int);
	int	(*is)(unsigned char *, int);
	int	(*reply)(unsigned char *, int);
	void	(*session)(Session_Key *);
	int	(*keyid)(int, unsigned char *, int *);
	void	(*printsub)(unsigned char *, int, unsigned char *, int);
} Encryptions;

#define	SK_DES		1	/* Matched Kerberos v5 ENCTYPE_DES */

void	encrypt_support(uchar_t *, int);
void	encrypt_init(char *);
void	encrypt_send_supprt(void);
void	encrypt_auto(int);
void	decrypt_auto(int);
void	encrypt_is(unsigned char *, int);
void	encrypt_reply(unsigned char *, int);
void	encrypt_start(uchar_t *, int);
void	encrypt_start_input(int);
void	encrypt_session_key(Session_Key *);
void	encrypt_end(void);
void	encrypt_request_end(void);
void	encrypt_request_start(uchar_t *, int);
void	encrypt_enc_keyid(uchar_t *, int);
void	encrypt_dec_keyid(uchar_t *, int);
void	encrypt_end_input(void);
void	encrypt_end_output(void);
boolean_t	encrypt_is_encrypting(void);
void	encrypt_send_support(void);
void	encrypt_send_keyid(int, unsigned char *, int, int);
void	encrypt_display(void);
void	encrypt_printsub(uchar_t *, int,  uchar_t *, int);

void	cfb64_encrypt(unsigned char *, int);
int	cfb64_decrypt(int);
void	cfb64_init(void);
int	cfb64_start(int);
int	cfb64_is(unsigned char *, int);
int	cfb64_reply(unsigned char *, int);
void	cfb64_session(Session_Key *);
int	cfb64_keyid(int, unsigned char *, int *);
void	cfb64_printsub(unsigned char *, int, unsigned char *, int);

extern boolean_t	encrypt_debug_mode;
extern int	(*decrypt_input)(int);
extern void	(*encrypt_output)(unsigned char *, int);

int	key_file_exists(void);
void	key_lookup(unsigned char *, Block);
void	key_stream_init(Block, Block, int);
unsigned char	key_stream(int, int);

int	EncryptStatus(void);
int	EncryptEnable(char *, char *);
int	EncryptDisable(char *, char *);
int	EncryptType(char *, char *);
int	EncryptStart(char *);
int	EncryptStartInput(void);
int	EncryptStartOutput(void);
int	EncryptStop(char *);
int	EncryptStopInput(void);
int	EncryptStopOutput(void);
int	EncryptDebug(int);
int	EncryptVerbose(int);
int	EncryptAutoEnc(int);
int	EncryptAutoDec(int);

#ifdef	__cplusplus
}
#endif

#endif	/* _ENCRYPT_H */
