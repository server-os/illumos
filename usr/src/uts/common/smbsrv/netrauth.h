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
 * Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 * Copyright 2020 Tintri by DDN, Inc. All rights reserved.
 * Copyright 2023 RackTop Systems, Inc.
 */

#ifndef _SMBSRV_NETRAUTH_H
#define	_SMBSRV_NETRAUTH_H

/*
 * NETR remote authentication and logon services.
 */

#include <sys/types.h>
#include <smb/wintypes.h>
#include <smbsrv/netbios.h>
#include <smbsrv/smbinfo.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * See also netlogon.ndl.
 */
#define	NETR_WKSTA_TRUST_ACCOUNT_TYPE		0x02
#define	NETR_DOMAIN_TRUST_ACCOUNT_TYPE		0x04

/*
 * Negotiation flags for challenge/response authentication.
 */
#define	NETR_NEGO_UNUSED_A_FLAG			0x00000001
#define	NETR_NEGO_BDC_UPDATE_FLAG		0x00000002
#define	NETR_NEGO_RC4_ENCRYPT_FLAG		0x00000004
#define	NETR_NEGO_UNUSED_D_FLAG			0x00000008

#define	NETR_NEGO_BDC_CHANGELOG_FLAG		0x00000010
#define	NETR_NEGO_DC_RESTART_SYNC_FLAG		0x00000020
#define	NETR_NEGO_VALID2_NOTREQUIRED_FLAG	0x00000040
#define	NETR_NEGO_OPNUM17_FLAG			0x00000080

#define	NETR_NEGO_PWCHANGE_REFUSE_FLAG		0x00000100
#define	NETR_NEGO_OPNUM32_FLAG			0x00000200
#define	NETR_NEGO_GENERIC_PASSTHRU_FLAG		0x00000400
#define	NETR_NEGO_CONCURRENT_RPC_FLAG		0x00000800

#define	NETR_NEGO_AVOID_USERDB_REPL_FLAG	0x00001000
#define	NETR_NEGO_AVOID_SECURITYDB_REPL_FLAG	0x00002000
#define	NETR_NEGO_STRONGKEY_FLAG		0x00004000
#define	NETR_NEGO_TRANSITIVE_TRUSTFLAG		0x00008000

#define	NETR_NEGO_UNUSED_Q_FLAG			0x00010000
#define	NETR_NEGO_PASSWORDSET2_FLAG		0x00020000
#define	NETR_NEGO_GETDOMAININFO_FLAG		0x00040000
#define	NETR_NEGO_CROSS_FOREST_TRUST_FLAG	0x00080000

#define	NETR_NEGO_IGNORE_NT4EMULATOR_FLAG	0x00100000
#define	NETR_NEGO_RODC_PASSTHRU_FLAG		0x00200000
#define	NETR_NEGO_UNDEFINED_9_FLAG		0x00400000
#define	NETR_NEGO_UNDEFINED_8_FLAG		0x00800000

#define	NETR_NEGO_AES_SHA2_FLAG			0x01000000
#define	NETR_NEGO_UNDEFINED_6_FLAG		0x02000000
#define	NETR_NEGO_UNDEFINED_5_FLAG		0x04000000
#define	NETR_NEGO_UNDEFINED_4_FLAG		0x08000000

#define	NETR_NEGO_UNDEFINED_3_FLAG		0x10000000
#define	NETR_NEGO_UNUSED_X_FLAG			0x20000000
#define	NETR_NEGO_SECURE_RPC_FLAG		0x40000000
#define	NETR_NEGO_UNDEFINED_0_FLAG		0x80000000

/*
 * TODO: This needs review - some of these are inappropriate.
 * I.E. BDC_UPDATE, BDC_CHANGELOG, and DC_RESTART_SYNC are
 * server-to-server only, but we implement a client.
 */
#define	NETR_NEGO_BASE_FLAGS	(		\
	NETR_NEGO_UNUSED_A_FLAG	|		\
	NETR_NEGO_BDC_UPDATE_FLAG |		\
	NETR_NEGO_RC4_ENCRYPT_FLAG |		\
	NETR_NEGO_UNUSED_D_FLAG |		\
	NETR_NEGO_BDC_CHANGELOG_FLAG |		\
	NETR_NEGO_DC_RESTART_SYNC_FLAG |	\
	NETR_NEGO_VALID2_NOTREQUIRED_FLAG |	\
	NETR_NEGO_OPNUM17_FLAG |		\
	NETR_NEGO_PWCHANGE_REFUSE_FLAG)		\

#define	NETR_SESSKEY64_SZ			8
#define	NETR_SESSKEY128_SZ			16
#define	NETR_SESSKEY_MAXSZ			NETR_SESSKEY128_SZ
#define	NETR_CRED_DATA_SZ			8
#define	NETR_OWF_PASSWORD_SZ			16

/*
 * SAM logon levels: interactive and network.
 */
#define	NETR_INTERACTIVE_LOGON			0x01
#define	NETR_NETWORK_LOGON			0x02

/*
 * SAM logon validation levels.
 */
#define	NETR_VALIDATION_LEVEL3			0x03

/*
 * Most of these are from: "MSV1_0_LM20_LOGON structure"
 * http://msdn.microsoft.com/en-us/library/windows/desktop/aa378762
 * and a few are from the ntddk (ntmsv1_0.h) found many places.
 */
#define	MSV1_0_CLEARTEXT_PASSWORD_ALLOWED	0x00000002
#define	MSV1_0_UPDATE_LOGON_STATISTICS		0x00000004
#define	MSV1_0_RETURN_USER_PARAMETERS		0x00000008
#define	MSV1_0_DONT_TRY_GUEST_ACCOUNT		0x00000010
#define	MSV1_0_ALLOW_SERVER_TRUST_ACCOUNT	0x00000020
#define	MSV1_0_RETURN_PASSWORD_EXPIRY		0x00000040
/*
 * MSV1_0_USE_CLIENT_CHALLENGE means the LM response field contains the
 * "client challenge" in the first 8 bytes instead of the LM response.
 */
#define	MSV1_0_USE_CLIENT_CHALLENGE		0x00000080
#define	MSV1_0_TRY_GUEST_ACCOUNT_ONLY		0x00000100
#define	MSV1_0_RETURN_PROFILE_PATH		0x00000200
#define	MSV1_0_TRY_SPECIFIED_DOMAIN_ONLY	0x00000400
#define	MSV1_0_ALLOW_WORKSTATION_TRUST_ACCOUNT	0x00000800
#define	MSV1_0_DISABLE_PERSONAL_FALLBACK	0x00001000
#define	MSV1_0_ALLOW_FORCE_GUEST		0x00002000
#define	MSV1_0_CLEARTEXT_PASSWORD_SUPPLIED	0x00004000
#define	MSV1_0_USE_DOMAIN_FOR_ROUTING_ONLY	0x00008000
#define	MSV1_0_SUBAUTHENTICATION_DLL_EX		0x00100000

/*
 * This is a duplicate of the netr_credential
 * from netlogon.ndl.
 */
typedef struct netr_cred {
	BYTE data[NETR_CRED_DATA_SZ];
} netr_cred_t;

typedef struct netr_session_key {
	BYTE key[NETR_SESSKEY_MAXSZ];
	short len;
} netr_session_key_t;

#define	NETR_FLG_NULL		0x00000001
#define	NETR_FLG_VALID		0x00000001
#define	NETR_FLG_INIT		0x00000002

/*
 * 120-byte machine account password (null-terminated)
 */
#define	NETR_MACHINE_ACCT_PASSWD_MAX	120 + 1

typedef struct netr_info {
	DWORD flags;
	char server[MAXHOSTNAMELEN];		/* Current DC, FQDN */
	char hostname[NETBIOS_NAME_SZ * 2];	/* local "flat" name */
	char nb_domain[NETBIOS_NAME_SZ * 2];	/* Current NetBios domain */
	char fqdn_domain[MAXHOSTNAMELEN];	/* Current Domain */
	netr_cred_t client_challenge;
	netr_cred_t server_challenge;
	netr_cred_t client_credential;
	netr_cred_t server_credential;
	netr_session_key_t session_key;
	netr_session_key_t rpc_seal_key;
	BYTE password[NETR_MACHINE_ACCT_PASSWD_MAX];
	time_t timestamp;
	uint64_t clh_seqnum; /* Client SequenceNumber for Netlogon SSP */
	DWORD nego_flags; /* Negotiated flags returned from ServerAuthenciate */
	boolean_t use_secure_rpc; /* Use "SecureRPC" (RPC-level auth) */
	boolean_t use_logon_ex; /* Use SamLogonEx (instead of SamLogon) */
} netr_info_t;

/*
 * NETLOGON private interface.
 */
int netr_gen_skey64(netr_info_t *);
int netr_gen_skey128(netr_info_t *);

int netr_gen_credentials(BYTE *, netr_cred_t *, DWORD, netr_cred_t *,
    boolean_t);

void netlogon_init_global(uint32_t);

#define	NETR_A2H(c) (isdigit(c)) ? ((c) - '0') : ((c) - 'A' + 10)

#ifdef __cplusplus
}
#endif

#endif /* _SMBSRV_NETRAUTH_H */
