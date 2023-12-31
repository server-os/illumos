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

/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/* Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T */
/* All Rights Reserved */
/*
 * Portions of this source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 */

/*
 * XDR routines for DES authentication
 */

#include "mt.h"
#include <sys/types.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/auth_des.h>

bool_t
xdr_authdes_cred(XDR *xdrs, struct authdes_cred *cred)
{
	/*
	 * Unrolled xdr
	 */
	if (!xdr_enum(xdrs, (enum_t *)&cred->adc_namekind))
		return (FALSE);
	switch (cred->adc_namekind) {
	case ADN_FULLNAME:
		if (!xdr_string(xdrs, &cred->adc_fullname.name,
				MAXNETNAMELEN))
			return (FALSE);
		if (!xdr_opaque(xdrs, (caddr_t)&cred->adc_fullname.key,
				(uint_t)sizeof (des_block)))
			return (FALSE);
		return (xdr_opaque(xdrs, (caddr_t)&cred->adc_fullname.window,
				(uint_t)sizeof (cred->adc_fullname.window)));
	case ADN_NICKNAME:
		return (xdr_opaque(xdrs, (caddr_t)&cred->adc_nickname,
				(uint_t)sizeof (cred->adc_nickname)));
	}
	return (FALSE);
}

bool_t
xdr_authdes_verf(XDR *xdrs, struct authdes_verf *verf)
{
	/*
	 * Unrolled xdr
	 */
	if (!xdr_opaque(xdrs, (caddr_t)&verf->adv_xtimestamp,
				(uint_t)sizeof (des_block)))
		return (FALSE);
	return (xdr_opaque(xdrs, (caddr_t)&verf->adv_int_u,
				(uint_t)sizeof (verf->adv_int_u)));
}
