/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright 2000 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
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
 *
 */
/*
 * Copyright 1993 by OpenVision Technologies, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of OpenVision not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. OpenVision makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * OPENVISION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OPENVISION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Copyright (C) 1998 by the FundsXpress, INC.
 *
 * All rights reserved.
 *
 * Export of this software from the United States of America may require
 * a specific license from the United States Government.  It is the
 * responsibility of any person or organization contemplating export to
 * obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of FundsXpress. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  FundsXpress makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "gssapiP_krb5.h"

size_t KRB5_CALLCONV krb5_encrypt_size(size_t, krb5_enctype);

 /* SUNW15resync - XXX find new home for this func */
#ifdef _KERNEL
size_t KRB5_CALLCONV
krb5_encrypt_size(size_t length, krb5_enctype crypto)
{
    size_t ret;

    if (krb5_c_encrypt_length(/* XXX */ 0, crypto, length, &ret))
	/*LINTED*/
        return(-1); /* XXX */

    return(ret);
}
#endif

/* V2 interface */
OM_uint32
krb5_gss_wrap_size_limit(minor_status, context_handle, conf_req_flag,
			 qop_req, req_output_size, max_input_size)
    OM_uint32		*minor_status;
    gss_ctx_id_t	context_handle;
    int			conf_req_flag;
    gss_qop_t		qop_req;
    OM_uint32		req_output_size;
    OM_uint32		*max_input_size;
{
    krb5_gss_ctx_id_rec	*ctx;
    OM_uint32		data_size, conflen;
    OM_uint32		ohlen;
    int			overhead;

    /* Solaris Kerb - check to make sure we aren't writing to a NULL pointer */
    if (!max_input_size)
 	return(GSS_S_CALL_INACCESSIBLE_WRITE);

    /* only default qop is allowed */
    /*
     * SUNW15resync
     * mit 1.2-6: if (qop_req != GSS_C_QOP_DEFAULT) {
     * Go with Solaris version here, though not sure which is
     * correct and RFC 2743 does not make it clear.
     */
    if ((qop_req & GSS_KRB5_CONF_C_QOP_MASK) != GSS_C_QOP_DEFAULT) {
	*minor_status = (OM_uint32) G_UNKNOWN_QOP;
	/* SUNW15resync - RFC 2743 is clear here but
          this is still GSS_S_FAILURE in MIT */
	return(GSS_S_BAD_QOP);
    }

    /* validate the context handle */
    if (! kg_validate_ctx_id(context_handle)) {
	*minor_status = (OM_uint32) G_VALIDATE_FAILED;
	return(GSS_S_NO_CONTEXT);
    }

    ctx = (krb5_gss_ctx_id_rec *) context_handle;
    if (! ctx->established) {
	*minor_status = KG_CTX_INCOMPLETE;
	return(GSS_S_NO_CONTEXT);
    }

    if (ctx->proto == 1) {
	/* No pseudo-ASN.1 wrapper overhead, so no sequence length and
	   OID.  */
	OM_uint32 sz = req_output_size;
	/* Token header: 16 octets.  */
	if (conf_req_flag) {
	    while (sz > 0 && krb5_encrypt_size(sz, ctx->enc->enctype) + 16 > req_output_size)
		sz--;
	    /* Allow for encrypted copy of header.  */
	    if (sz > 16)
		sz -= 16;
	    else
		sz = 0;
#ifdef CFX_EXERCISE
	    /* Allow for EC padding.  In the MIT implementation, only
	       added while testing.  */
	    if (sz > 65535)
		sz -= 65535;
	    else
		sz = 0;
#endif
	} else {
	    /* Allow for token header and checksum.  */
	    if (sz < 16 + ctx->cksum_size)
		sz = 0;
	    else
		sz -= (16 + ctx->cksum_size);
	}

	*max_input_size = sz;
	*minor_status = 0;
	return GSS_S_COMPLETE;
    }

    /* Calculate the token size and subtract that from the output size */
    overhead = 7 + ctx->mech_used->length;
    data_size = req_output_size;
    conflen = kg_confounder_size(ctx->k5_context, ctx->enc);
    data_size = (conflen + data_size + 8) & (~(OM_uint32)7);
    ohlen = g_token_size(ctx->mech_used,
			 (unsigned int) (data_size + ctx->cksum_size + 14))
      - req_output_size;

    if (ohlen+overhead < req_output_size)
      /*
       * Cannot have trailer length that will cause us to pad over our
       * length.
       */
      *max_input_size = (req_output_size - ohlen - overhead) & (~(OM_uint32)7);
    else
      *max_input_size = 0;

    *minor_status = 0;
    return(GSS_S_COMPLETE);
}
