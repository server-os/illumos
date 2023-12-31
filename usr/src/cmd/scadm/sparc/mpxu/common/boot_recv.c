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
 * Copyright 2002 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * boot_recv.c: retrieve a BP (Boot Protocol) message from the receive queue
 * (BP messages received from the service processor are queued in
 * ADM_bpMsgQueue). BP messages are received ONLY in a firmware download
 * context
 */

#include <stdio.h>

#include "adm.h"
#include "smq.h"


extern smq_t  ADM_bpMsgQueue;


int
ADM_Boot_recv(bp_msg_t *MessagePtr, struct timespec *Timeout)
{
	void *msgp = (void *)MessagePtr;

	if (smq_xreceive(&ADM_bpMsgQueue, Timeout,
	    (smq_msg_t *)msgp) != 0) {

		return (-1);
	}

	return (0);
}
