/*
 * Copyright (C) 2002 Microsoft Corporation
 * All rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS"
 * WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTIBILITY
 * AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Date    - 10/08/2002
 * Author  - Sanj Surati
 */

/*
 * Copyright 2012 Nexenta Systems, Inc.  All rights reserved.
 * Copyright 2022 RackTop Systems, Inc.
 */

/*
 * spnego.h
 *
 * SPNEGO Token Handler Header File
 *
 * Contains the definitions required to interpret and create
 * SPNEGO tokens so that Kerberos GSS tokens can be
 * Unpackaged/packaged.
 */

#ifndef _SPNEGO_H
#define	_SPNEGO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type Definitions
 */

/*
 * Users of SPNEGO Token Handler API will request
 * these as well as free them,
 */
typedef void*  SPNEGO_TOKEN_HANDLE;

/*
 * Defines the element types that are found
 * in each of the tokens.
 */

typedef enum spnego_element_type
{
	spnego_element_min,  /* Lower bound */

	/* Init token elements */
	spnego_init_mechtypes,
	spnego_init_reqFlags,
	spnego_init_mechToken,
	spnego_init_mechListMIC,

	/* Targ token elements */
	spnego_targ_negResult,
	spnego_targ_supportedMech,
	spnego_targ_responseToken,
	spnego_targ_mechListMIC,

	spnego_element_max   /* Upper bound */

} SPNEGO_ELEMENT_TYPE;

/*
 * Token Element Availability.  Elements in both
 * token types are optional.  Since there are only
 * 4 elements in each Token, we will allocate space
 * to hold the information, but we need a way to
 * indicate whether or not an element is available
 */

#define	SPNEGO_TOKEN_ELEMENT_UNAVAILABLE 0
#define	SPNEGO_TOKEN_ELEMENT_AVAILABLE 1

/*
 * Token type values.  SPNEGO has 2 token types:
 * NegTokenInit and NegTokenTarg
 */

#define	SPNEGO_TOKEN_INIT 0
#define	SPNEGO_TOKEN_TARG 1

/*
 * GSS Mechanism OID enumeration.  We only really handle
 * 3 different OIDs.  These are stored in an array structure
 * defined in the parsing code.
 */

typedef enum spnego_mech_oid
{
	/* Init token elements */
	spnego_mech_oid_Kerberos_V5_Legacy, /* Really V5, but OID off by 1 */
	spnego_mech_oid_Kerberos_V5,
	spnego_mech_oid_Spnego,
	spnego_mech_oid_NTLMSSP,
	spnego_mech_oid_NotUsed = -1

} SPNEGO_MECH_OID;

/*
 * Defines the negResult values.
 */

typedef enum spnego_negResult
{
	spnego_negresult_success,
	spnego_negresult_incomplete,
	spnego_negresult_rejected,
	spnego_negresult_request_mic,
	spnego_negresult_NotUsed = -1
} SPNEGO_NEGRESULT;

/*
 * Context Flags in NegTokenInit
 */

/*
 * ContextFlags values MUST be zero or a combination
 * of the below
 */

#define	SPNEGO_NEGINIT_CONTEXT_DELEG_FLAG	0x80
#define	SPNEGO_NEGINIT_CONTEXT_MUTUAL_FLAG	0x40
#define	SPNEGO_NEGINIT_CONTEXT_REPLAY_FLAG	0x20
#define	SPNEGO_NEGINIT_CONTEXT_SEQUENCE_FLAG	0x10
#define	SPNEGO_NEGINIT_CONTEXT_ANON_FLAG	0x8
#define	SPNEGO_NEGINIT_CONTEXT_CONF_FLAG	0x4
#define	SPNEGO_NEGINIT_CONTEXT_INTEG_FLAG	0x2

/*
 * Mask to retrieve valid values.
 */

#define	SPNEGO_NEGINIT_CONTEXT_MASK	0xFE

/*
 * SPNEGO API return codes.
 */

/* API function was successful */
#define	SPNEGO_E_SUCCESS		0

/* The supplied Token was invalid */
#define	SPNEGO_E_INVALID_TOKEN		-1

/* An invalid length was encountered */
#define	SPNEGO_E_INVALID_LENGTH		-2

/* The Token Parse failed */
#define	SPNEGO_E_PARSE_FAILED		-3

/* The requested value was not found */
#define	SPNEGO_E_NOT_FOUND		-4

/* The requested element is not available */
#define	SPNEGO_E_ELEMENT_UNAVAILABLE	-5

/* Out of Memory */
#define	SPNEGO_E_OUT_OF_MEMORY		-6

/* Not Implemented */
#define	SPNEGO_E_NOT_IMPLEMENTED	-7

/* Invalid Parameter */
#define	SPNEGO_E_INVALID_PARAMETER	-8

/* Token Handler encountered an unexpected OID */
#define	SPNEGO_E_UNEXPECTED_OID		-9

/* The requested token was not found */
#define	SPNEGO_E_TOKEN_NOT_FOUND	-10

/* An unexpected type was encountered in the encoding */
#define	SPNEGO_E_UNEXPECTED_TYPE	-11

/* The buffer was too small */
#define	SPNEGO_E_BUFFER_TOO_SMALL	-12

/* A Token Element was invalid (e.g. improper length or value) */
#define	SPNEGO_E_INVALID_ELEMENT	-13

/* Miscelaneous API Functions */

/* Frees opaque data */
void spnegoFreeData(SPNEGO_TOKEN_HANDLE hSpnegoToken);

/* Initializes SPNEGO_TOKEN structure from DER encoded binary data */
int spnegoInitFromBinary(unsigned char *pbTokenData, unsigned long ulLength,
	SPNEGO_TOKEN_HANDLE* phSpnegoToken);

/* Initializes SPNEGO_TOKEN structure for a NegTokenInit type */
int spnegoCreateNegTokenHint(SPNEGO_MECH_OID *pMechTypeList, int MechTypeCnt,
	unsigned char *pbPrincipal, SPNEGO_TOKEN_HANDLE* phSpnegoToken);

/* Initializes SPNEGO_TOKEN structure for a NegTokenInit type */
int spnegoCreateNegTokenInit(SPNEGO_MECH_OID MechType,
	unsigned char ucContextFlags, unsigned char *pbMechToken,
	unsigned long ulMechTokenLen, unsigned char *pbMechTokenMIC,
	unsigned long ulMechTokenMIC, SPNEGO_TOKEN_HANDLE *phSpnegoToken);

/* Initializes SPNEGO_TOKEN structure for a NegTokenTarg type */
int spnegoCreateNegTokenTarg(SPNEGO_MECH_OID MechType,
	SPNEGO_NEGRESULT spnegoNegResult, unsigned char *pbMechToken,
	unsigned long ulMechTokenLen, unsigned char *pbMechListMIC,
	unsigned long ulMechListMICLen, SPNEGO_TOKEN_HANDLE* phSpnegoToken);

/* Copies binary representation of SPNEGO Data into user supplied buffer */
int spnegoTokenGetBinary(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	unsigned char *pbTokenData, unsigned long *pulDataLen);

/* Returns SPNEGO Token Type */
int spnegoGetTokenType(SPNEGO_TOKEN_HANDLE hSpnegoToken, int *piTokenType);

/* Reading an Init Token */

/* Returns the Initial Mech Type in the MechList element in the NegInitToken. */
int spnegoIsMechTypeAvailable(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	SPNEGO_MECH_OID MechOID, int *piMechTypeIndex);

/* Returns the value from the context flags element in the NegInitToken */
int spnegoGetContextFlags(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	unsigned char *pucContextFlags);

/* Reading a Response Token */

/*
 * Returns the value from the negResult element
 * (Status code of GSS call - 0,1,2)
 */
int spnegoGetNegotiationResult(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	SPNEGO_NEGRESULT* pnegResult);

/* Returns the Supported Mech Type from the NegTokenTarg. */
int spnegoGetSupportedMechType(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	SPNEGO_MECH_OID* pMechOID);

/* Reading either Token Type */

/*
 * Returns the actual Mechanism data from the token
 * (this is what is passed into GSS-API functions
 */
int spnegoGetMechToken(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	unsigned char *pbTokenData, unsigned long *pulDataLen);

/* Returns the Message Integrity BLOB in the token */
int spnegoGetMechListMIC(SPNEGO_TOKEN_HANDLE hSpnegoToken,
	unsigned char *pbMICData, unsigned long *pulDataLen);

#ifdef __cplusplus
}
#endif

#endif /* _SPNEGO_H */
