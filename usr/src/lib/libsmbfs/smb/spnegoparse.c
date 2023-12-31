// Copyright 2022 RackTop Systems, Inc.
// Copyright 2012 Nexenta Systems, Inc.  All rights reserved.
// Copyright (C) 2002 Microsoft Corporation
// All rights reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS"
// WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
// OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE IMPLIED WARRANTIES OF MERCHANTIBILITY
// AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//
// Date    - 10/08/2002
// Author  - Sanj Surati

/////////////////////////////////////////////////////////////
//
// SPNEGOPARSE.C
//
// SPNEGO Token Handler Source File
//
// Contains implementation of SPNEGO Token parsing functions.
//
/////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "spnego.h"
#include "derparse.h"
#include "spnegoparse.h"

//
// Defined in DERPARSE.C
//

extern MECH_OID g_stcMechOIDList [];

/**********************************************************************/
/**                                                                  **/
/**                                                                  **/
/**                                                                  **/
/**                                                                  **/
/**                 Local SPNEGO Helper definitions                  **/
/**                                                                  **/
/**                                                                  **/
/**                                                                  **/
/**                                                                  **/
/**********************************************************************/


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    CalculateMinSpnegoInitTokenSize
//
// Parameters:
//    [in]  nMechTokenLength        -  Length of the MechToken Element
//    [in]  nMechListMICLength      -  Length of the MechListMIC Element
//                                     (or negHints, if no MechToken)
//    [in]  mechOID                 -  OID for MechList
//    [in]  nReqFlagsAvailable      -  Is ContextFlags element available
//    [out] pnTokenSize             -  Filled out with total size of token
//    [out] pnInternalTokenLength   -  Filled out with length minus length
//                                     for initial token.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Calculates the required length for a SPNEGO NegTokenInit token based
//    on the supplied variable length values and which elements are present.
//    Note that because the lengths can be represented by an arbitrary
//    number of bytes in DER encodings, we actually calculate the lengths
//    backwards, so we always know how many bytes we will potentially be
//    writing out.
//
////////////////////////////////////////////////////////////////////////////

int CalculateMinSpnegoInitTokenSize( long nMechTokenLength,
      long nMechListMICLength, SPNEGO_MECH_OID *mechOidLst, int mechOidCnt,
                                 int nReqFlagsAvailable, long* pnTokenSize,
                                 long* pnInternalTokenLength )
{
   int   nReturn = SPNEGO_E_INVALID_LENGTH;

   // Start at 0.
   long  nTotalLength = 0;
   long  nTempLength= 0L;

   // We will calculate this by walking the token backwards

   // Start with MIC Element (or negHints)
   if ( nMechListMICLength > 0L )
   {
      nTempLength = ASNDerCalcElementLength( nMechListMICLength, NULL );

      // Check for rollover error
      if ( nTempLength < nMechListMICLength )
      {
         goto xEndTokenInitLength;
      }

      nTotalLength += nTempLength;
   }

   // Next is the MechToken
   if ( nMechTokenLength > 0L )
   {
      nTempLength += ASNDerCalcElementLength( nMechTokenLength, NULL );

      // Check for rollover error
      if ( nTempLength < nTotalLength )
      {
         goto xEndTokenInitLength;
      }

      nTotalLength = nTempLength;
   }

   // Next is the ReqFlags
   if ( nReqFlagsAvailable )
   {
      nTempLength += ASNDerCalcElementLength( SPNEGO_NEGINIT_MAXLEN_REQFLAGS, NULL );

      // Check for rollover error
      if ( nTempLength < nTotalLength )
      {
         goto xEndTokenInitLength;
      }

      nTotalLength = nTempLength;
   }

   // Next is the MechList - This is REQUIRED
   nTempLength += ASNDerCalcMechListLength( mechOidLst, mechOidCnt, NULL );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenInitLength;
   }

   nTotalLength = nTempLength;

   // Following four fields are the basic header tokens

   // Sequence Token
   nTempLength += ASNDerCalcTokenLength( nTotalLength, 0L );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenInitLength;
   }

   nTotalLength = nTempLength;

   // Neg Token Identifier Token
   nTempLength += ASNDerCalcTokenLength( nTotalLength, 0L );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenInitLength;
   }

   nTotalLength = nTempLength;

   // SPNEGO OID Token
   nTempLength += g_stcMechOIDList[spnego_mech_oid_Spnego].iLen;

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenInitLength;
   }

   nTotalLength = nTempLength;

   // App Constructed Token
   nTempLength += ASNDerCalcTokenLength( nTotalLength, 0L );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenInitLength;
   }

   // The internal length doesn't include the number of bytes
   // for the initial token
   *pnInternalTokenLength = nTotalLength;
   nTotalLength = nTempLength;

   // We're done
   *pnTokenSize = nTotalLength;
   nReturn = SPNEGO_E_SUCCESS;

xEndTokenInitLength:

   return nReturn;

}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    CreateSpnegoInitToken
//
// Parameters:
//    [in]  pMechTypeList           -  OID array
//    [in]  MechTypeCnt             -  OID array length
//    [in]  ucContextFlags          -  ContextFlags value
//    [in]  pbMechToken             -  Mech Token Binary Data
//    [in]  ulMechTokenLen          -  Length of Mech Token
//    [in]  pbMechListMIC           -  MechListMIC Binary Data (or negHints)
//    [in]  ulMechListMICn          -  Length of MechListMIC
//    [out] pbTokenData             -  Buffer to write token into.
//    [in]  nTokenLength            -  Length of pbTokenData buffer
//    [in]  nInternalTokenLength    -  Length of full token without leading
//                                     token bytes.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Uses DER to fill out pbTokenData with a SPNEGO NegTokenInit Token
//    Note that because the lengths can be represented by an arbitrary
//    number of bytes in DER encodings, we actually calculate the lengths
//    backwards, so we always know how many bytes we will potentially be
//    writing out.
//
//    This function is also used to create an SPNEGO "hint", as described in
//    [MS-SPNG] sec. 2.2.1 negTokenInit2.  The "hint" looks almost identical
//    to a NegTokenInit, but has a "negHints" field inserted before the MIC.
//    A normal SPNEGO negTokenInit2 contains only the mech list and the
//    negHints.  To avoid a giant copy/paste of this function, we pass the
//    negHints as the MIC arg, and pass NULL as the MechToken to indicate
//    that we're creating a Hint rather than an Init, and use the correct
//    type when writing out the MIC (or negHints) element.
//
////////////////////////////////////////////////////////////////////////////

int CreateSpnegoInitToken( SPNEGO_MECH_OID *pMechTypeList, long MechTypeCnt,
          unsigned char ucContextFlags, unsigned char* pbMechToken,
          unsigned long ulMechTokenLen, unsigned char* pbMechListMIC,
          unsigned long ulMechListMICLen, unsigned char* pbTokenData,
          long nTokenLength, long nInternalTokenLength )
{
   int   nReturn = SPNEGO_E_INVALID_LENGTH;

   // Start at 0.
   long  nTempLength= 0L;
   long  nTotalBytesWritten = 0L;
   long  nInternalLength = 0L;

   unsigned char* pbWriteTokenData = pbTokenData + nTokenLength;

   // Temporary buffer to hold the REQ Flags as BIT String Data
   unsigned char  abTempReqFlags[SPNEGO_NEGINIT_MAXLEN_REQFLAGS];


   // We will write the token out backwards to properly handle the cases
   // where the length bytes become adjustable

   // Start with MIC Element (or negHints)
   if ( ulMechListMICLen > 0L )
   {
      unsigned char ucType;
      nTempLength = ASNDerCalcElementLength( ulMechListMICLen, &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and write it out.
      // Note: When MechTokenLen == 0, we're writing a negTokenInit2 and the
      // MIC arg is really negHints, written as a constructed sequence.
      // Otherwise we're writing a negTokenInit, and the MIC is an OCTETSTRING.
      ucType = (ulMechTokenLen == 0) ?
         SPNEGO_CONSTRUCTED_SEQUENCE : OCTETSTRING;

      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGINIT_ELEMENT_MECHLISTMIC,
                              ucType, pbMechListMIC, ulMechListMICLen );

      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenInit;
      }

   }  // IF MechListMIC is present

   // Next is the MechToken
   if ( ulMechTokenLen > 0L )
   {
      nTempLength = ASNDerCalcElementLength( ulMechTokenLen, &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.
      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGINIT_ELEMENT_MECHTOKEN,
                              OCTETSTRING, pbMechToken, ulMechTokenLen );
      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenInit;
      }

   }  // IF MechToken Length is present

   // Next is the ReqFlags
   if ( ucContextFlags > 0L )
   {

      nTempLength = ASNDerCalcElementLength( SPNEGO_NEGINIT_MAXLEN_REQFLAGS, &nInternalLength );

      // We need a byte that indicates how many bits difference between the number
      // of bits used in final octet (we only have one) and the max (8)

      abTempReqFlags[0] = SPNEGO_NEGINIT_REQFLAGS_BITDIFF;
      abTempReqFlags[1] = ucContextFlags;

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.
      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGINIT_ELEMENT_REQFLAGS,
                              BITSTRING, abTempReqFlags, SPNEGO_NEGINIT_MAXLEN_REQFLAGS );

      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenInit;
      }

   }  // IF ContextFlags

   // Next is the MechList - This is REQUIRED
   nTempLength = ASNDerCalcMechListLength( pMechTypeList, MechTypeCnt, &nInternalLength );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteMechList( pbWriteTokenData, pMechTypeList, MechTypeCnt );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;
   nInternalTokenLength -= nTempLength;

   if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
   {
      goto xEndWriteNegTokenInit;
   }

   // The next tokens we're writing out reflect the total number of bytes
   // we have actually written out.

   // Sequence Token
   nTempLength = ASNDerCalcTokenLength( nTotalBytesWritten, 0L );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                    NULL, nTotalBytesWritten );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;
   nInternalTokenLength -= nTempLength;

   if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
   {
      goto xEndWriteNegTokenInit;
   }

   // Neg Init Token Identifier Token
   nTempLength = ASNDerCalcTokenLength( nTotalBytesWritten, 0L );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_NEGINIT_TOKEN_IDENTIFIER,
                                    NULL, nTotalBytesWritten );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;
   nInternalTokenLength -= nTempLength;

   if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
   {
      goto xEndWriteNegTokenInit;
   }

   // SPNEGO OID Token
   nTempLength = g_stcMechOIDList[spnego_mech_oid_Spnego].iLen;

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteOID( pbWriteTokenData, spnego_mech_oid_Spnego );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;
   nInternalTokenLength -= nTempLength;

   if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
   {
      goto xEndWriteNegTokenInit;
   }

   // App Constructed Token
   nTempLength = ASNDerCalcTokenLength( nTotalBytesWritten, 0L );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_NEGINIT_APP_CONSTRUCT,
                                    NULL, nTotalBytesWritten );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;

   // Don't adjust the internal token length here, it doesn't account
   // the initial bytes written out (we really don't need to keep
   // a running count here, but for debugging, it helps to be able
   // to see the total number of bytes written out as well as the
   // number of bytes left to write).

   if ( nTotalBytesWritten == nTokenLength && nInternalTokenLength == 0 &&
         pbWriteTokenData == pbTokenData )
   {
      nReturn = SPNEGO_E_SUCCESS;
   }

xEndWriteNegTokenInit:

   return nReturn;

}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    CalculateMinSpnegoTargTokenSize
//
// Parameters:
//    [in]  MechType                -  Supported MechType
//    [in]  spnegoNegResult         -  Neg Result
//    [in]  nMechTokenLength        -  Length of the MechToken Element
//    [in]  nMechListMICLength      -  Length of the MechListMIC Element
//    [out] pnTokenSize             -  Filled out with total size of token
//    [out] pnInternalTokenLength   -  Filled out with length minus length
//                                     for initial token.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Calculates the required length for a SPNEGO NegTokenTarg token based
//    on the supplied variable length values and which elements are present.
//    Note that because the lengths can be represented by an arbitrary
//    number of bytes in DER encodings, we actually calculate the lengths
//    backwards, so we always know how many bytes we will potentially be
//    writing out.
//
////////////////////////////////////////////////////////////////////////////

int CalculateMinSpnegoTargTokenSize( SPNEGO_MECH_OID MechType,
                                    SPNEGO_NEGRESULT spnegoNegResult, long nMechTokenLen,
                                    long nMechListMICLen, long* pnTokenSize,
                                    long* pnInternalTokenLength )
{
   int   nReturn = SPNEGO_E_INVALID_LENGTH;

   // Start at 0.
   long  nTotalLength = 0;
   long  nTempLength= 0L;

   // We will calculate this by walking the token backwards

   // Start with MIC Element
   if ( nMechListMICLen > 0L )
   {
      nTempLength = ASNDerCalcElementLength( nMechListMICLen, NULL );

      // Check for rollover error
      if ( nTempLength < nMechListMICLen )
      {
         goto xEndTokenTargLength;
      }

      nTotalLength += nTempLength;
   }

   // Next is the MechToken
   if ( nMechTokenLen > 0L )
   {
      nTempLength += ASNDerCalcElementLength( nMechTokenLen, NULL );

      // Check for rollover error
      if ( nTempLength < nTotalLength )
      {
         goto xEndTokenTargLength;
      }

      nTotalLength = nTempLength;
   }

   // Supported MechType
   if ( spnego_mech_oid_NotUsed != MechType )
   {
      // Supported MechOID element - we use the token function since
      // we already know the size of the OID token and value
      nTempLength += ASNDerCalcElementLength( g_stcMechOIDList[MechType].iActualDataLen,
                                             NULL );

      // Check for rollover error
      if ( nTempLength < nTotalLength )
      {
         goto xEndTokenTargLength;
      }

      nTotalLength = nTempLength;

   }  // IF MechType is available

   // NegResult Element
   if ( spnego_negresult_NotUsed != spnegoNegResult )
   {
      nTempLength += ASNDerCalcElementLength( SPNEGO_NEGTARG_MAXLEN_NEGRESULT, NULL );

      // Check for rollover error
      if ( nTempLength < nTotalLength )
      {
         goto xEndTokenTargLength;
      }

      nTotalLength = nTempLength;

   }  // IF negResult is available

   // Following two fields are the basic header tokens

   // Sequence Token
   nTempLength += ASNDerCalcTokenLength( nTotalLength, 0L );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenTargLength;
   }

   nTotalLength = nTempLength;

   // Neg Token Identifier Token
   nTempLength += ASNDerCalcTokenLength( nTotalLength, 0L );

   // Check for rollover error
   if ( nTempLength < nTotalLength )
   {
      goto xEndTokenTargLength;
   }

   // The internal length doesn't include the number of bytes
   // for the initial token
   *pnInternalTokenLength = nTotalLength;
   nTotalLength = nTempLength;

   // We're done
   *pnTokenSize = nTotalLength;
   nReturn = SPNEGO_E_SUCCESS;

xEndTokenTargLength:

   return nReturn;

}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    CreateSpnegoTargToken
//
// Parameters:
//    [in]  MechType                -  Supported MechType
//    [in]  eNegResult              -  NegResult value
//    [in]  pbMechToken             -  Mech Token Binary Data
//    [in]  ulMechTokenLen          -  Length of Mech Token
//    [in]  pbMechListMIC           -  MechListMIC Binary Data
//    [in]  ulMechListMICn          -  Length of MechListMIC
//    [out] pbTokenData             -  Buffer to write token into.
//    [in]  nTokenLength            -  Length of pbTokenData buffer
//    [in]  nInternalTokenLength    -  Length of full token without leading
//                                     token bytes.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Uses DER to fill out pbTokenData with a SPNEGO NegTokenTarg Token
//    Note that because the lengths can be represented by an arbitrary
//    number of bytes in DER encodings, we actually calculate the lengths
//    backwards, so we always know how many bytes we will potentially be
//    writing out.
//
////////////////////////////////////////////////////////////////////////////

int CreateSpnegoTargToken( SPNEGO_MECH_OID MechType,
          SPNEGO_NEGRESULT eNegResult, unsigned char* pbMechToken,
          unsigned long ulMechTokenLen, unsigned char* pbMechListMIC,
          unsigned long ulMechListMICLen, unsigned char* pbTokenData,
          long nTokenLength, long nInternalTokenLength )
{
   int   nReturn = SPNEGO_E_INVALID_LENGTH;

   // Start at 0.
   long  nTempLength= 0L;
   long  nTotalBytesWritten = 0L;
   long  nInternalLength = 0L;

   unsigned char  ucTemp = 0;

   // We will write the token out backwards to properly handle the cases
   // where the length bytes become adjustable, so the write location
   // is initialized to point *just* past the end of the buffer.

   unsigned char* pbWriteTokenData = pbTokenData + nTokenLength;


   // Start with MIC Element
   if ( ulMechListMICLen > 0L )
   {
      nTempLength = ASNDerCalcElementLength( ulMechListMICLen, &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.

      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGTARG_ELEMENT_MECHLISTMIC,
                              OCTETSTRING, pbMechListMIC, ulMechListMICLen );

      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenTarg;
      }

   }  // IF MechListMIC is present

   // Next is the MechToken
   if ( ulMechTokenLen > 0L )
   {
      nTempLength = ASNDerCalcElementLength( ulMechTokenLen, &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.
      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGTARG_ELEMENT_RESPONSETOKEN,
                              OCTETSTRING, pbMechToken, ulMechTokenLen );
      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenTarg;
      }

   }  // IF MechToken Length is present

   // Supported Mech Type
   if ( spnego_mech_oid_NotUsed != MechType )
   {

      nTempLength = ASNDerCalcElementLength( g_stcMechOIDList[MechType].iActualDataLen,
                                             &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.
      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_NEGTARG_ELEMENT_SUPPORTEDMECH,
                                       g_stcMechOIDList[MechType].ucOid,
                                       g_stcMechOIDList[MechType].iLen );

      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenTarg;
      }

   }  // IF MechType is present

   // Neg Result
   // NegResult Element
   if ( spnego_negresult_NotUsed != eNegResult )
   {
      ucTemp = (unsigned char) eNegResult;

      nTempLength = ASNDerCalcElementLength( SPNEGO_NEGTARG_MAXLEN_NEGRESULT, &nInternalLength );

      // Decrease the pbWriteTokenData, now we know the length and
      // write it out.
      pbWriteTokenData -= nTempLength;
      nTempLength = ASNDerWriteElement( pbWriteTokenData, SPNEGO_NEGTARG_ELEMENT_NEGRESULT,
                              ENUMERATED, &ucTemp, SPNEGO_NEGTARG_MAXLEN_NEGRESULT );

      // Adjust Values and sanity check
      nTotalBytesWritten += nTempLength;
      nInternalTokenLength -= nTempLength;

      if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
      {
         goto xEndWriteNegTokenTarg;
      }

   }  // If eNegResult is available

   // The next tokens we're writing out reflect the total number of bytes
   // we have actually written out.

   // Sequence Token
   nTempLength = ASNDerCalcTokenLength( nTotalBytesWritten, 0L );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                    NULL, nTotalBytesWritten );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;
   nInternalTokenLength -= nTempLength;

   if ( nTotalBytesWritten > nTokenLength || nInternalTokenLength < 0 )
   {
      goto xEndWriteNegTokenTarg;
   }

   // Neg Targ Token Identifier Token
   nTempLength = ASNDerCalcTokenLength( nTotalBytesWritten, 0L );

   // Decrease the pbWriteTokenData, now we know the length and
   // write it out.
   pbWriteTokenData -= nTempLength;
   nTempLength = ASNDerWriteToken( pbWriteTokenData, SPNEGO_NEGTARG_TOKEN_IDENTIFIER,
                                    NULL, nTotalBytesWritten );

   // Adjust Values and sanity check
   nTotalBytesWritten += nTempLength;

   // Don't adjust the internal token length here, it doesn't account
   // the initial bytes written out (we really don't need to keep
   // a running count here, but for debugging, it helps to be able
   // to see the total number of bytes written out as well as the
   // number of bytes left to write).

   if ( nTotalBytesWritten == nTokenLength && nInternalTokenLength == 0 &&
         pbWriteTokenData == pbTokenData )
   {
      nReturn = SPNEGO_E_SUCCESS;
   }


xEndWriteNegTokenTarg:

   return nReturn;


}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    AllocEmptySpnegoToken
//
// Parameters:
//    [in]  ucCopyData        -  Flag to copy data or pointer.
//    [in]  ulFlags           -  Flags for SPNEGO_TOKEN data member.
//    [in]  pbTokenData       -  Binary token data.
//    [in]  ulTokenSize       -  Size of pbTokenData.
//
// Returns:
//    SPNEGO_TOKEN*  Success - Pointer to initialized SPNEGO_TOKEN struct
//                   Failure - NULL
//
// Comments :
//    Allocates a SPNEGO_TOKEN data structure and initializes it.  Based on
//    the value of ucCopyData, if non-zero, we copy the data into a buffer
//    we allocate in this function, otherwise, we copy the data pointer
//    direcly.
//
////////////////////////////////////////////////////////////////////////////

SPNEGO_TOKEN* AllocEmptySpnegoToken( unsigned char ucCopyData, unsigned long ulFlags,
                                    unsigned char * pbTokenData, unsigned long ulTokenSize )
{
   SPNEGO_TOKEN*  pSpnegoToken = (SPNEGO_TOKEN*) calloc( 1, sizeof(SPNEGO_TOKEN) );

   if ( NULL != pSpnegoToken )
   {
      // Set the token size
      pSpnegoToken->nStructSize = SPNEGO_TOKEN_SIZE;

      // Initialize the element array
      InitSpnegoTokenElementArray( pSpnegoToken );

      // Assign the flags value
      pSpnegoToken->ulFlags = ulFlags;

      //
      // IF ucCopyData is TRUE, we will allocate a buffer and copy data into it.
      // Otherwise, we will just copy the pointer and the length.  This is so we
      // can cut out additional allocations for performance reasons
      //

      if ( SPNEGO_TOKEN_INTERNAL_FLAGS_FREEDATA == ucCopyData )
      {
         // Alloc the internal buffer.  Cleanup on failure.
         pSpnegoToken->pbBinaryData = (unsigned char*) calloc( ulTokenSize, sizeof(unsigned char) );

         if ( NULL != pSpnegoToken->pbBinaryData )
         {
            // We must ALWAYS free this buffer
            pSpnegoToken->ulFlags |= SPNEGO_TOKEN_INTERNAL_FLAGS_FREEDATA;

            // Copy the data locally
            memcpy( pSpnegoToken->pbBinaryData, pbTokenData, ulTokenSize );
            pSpnegoToken->ulBinaryDataLen = ulTokenSize;
         }
         else
         {
            free( pSpnegoToken );
            pSpnegoToken = NULL;
         }

      }  // IF ucCopyData
      else
      {
         // Copy the pointer and the length directly - ulFlags will control whether or not
         // we are allowed to free the value

         pSpnegoToken->pbBinaryData = pbTokenData;
         pSpnegoToken->ulBinaryDataLen = ulTokenSize;
      }

   }

   return pSpnegoToken;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    FreeSpnegoToken
//
// Parameters:
//    [in]  pSpnegoToken      -  Points to SPNEGO_TOKEN to free.
//
// Returns:
//    void
//
// Comments :
//    If non-NULL, interprets pSpnegoToken, freeing any internal allocations
//    and finally the actual structure.
//
////////////////////////////////////////////////////////////////////////////

void FreeSpnegoToken( SPNEGO_TOKEN* pSpnegoToken )
{
   if ( NULL != pSpnegoToken )
   {

      // Cleanup internal allocation per the flags
      if ( pSpnegoToken->ulFlags & SPNEGO_TOKEN_INTERNAL_FLAGS_FREEDATA &&
         NULL != pSpnegoToken->pbBinaryData )
      {
         free( pSpnegoToken->pbBinaryData );
         pSpnegoToken->pbBinaryData = NULL;
      }

      free ( pSpnegoToken );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitSpnegoTokenElementArray
//
// Parameters:
//    [in]  pSpnegoToken      -  Points to SPNEGO_TOKEN structure.
//
// Returns:
//    void
//
// Comments :
//    Initializes the element array data member of a SPNEGO_TOKEN data
//    structure.
//
////////////////////////////////////////////////////////////////////////////

void InitSpnegoTokenElementArray( SPNEGO_TOKEN* pSpnegoToken )
{
   int   nCtr;

   // Set the number of elemnts
   pSpnegoToken->nNumElements = MAX_NUM_TOKEN_ELEMENTS;

   //
   // Initially, all elements are unavailable
   //

   for ( nCtr = 0; nCtr < MAX_NUM_TOKEN_ELEMENTS; nCtr++ )
   {
      // Set the element size as well
      pSpnegoToken->aElementArray[ nCtr ].nStructSize = SPNEGO_ELEMENT_SIZE;
      pSpnegoToken->aElementArray[ nCtr ].iElementPresent = SPNEGO_TOKEN_ELEMENT_UNAVAILABLE;
   }

}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitSpnegoTokenType
//
// Parameters:
//    [in]  pSpnegoToken            -  Points to SPNEGO_TOKEN structure.
//    [out] pnTokenLength           -  Filled out with total token length
//    [out] pnRemainingTokenLength  -  Filled out with remaining length
//                                     after header is parsed
//    [out] ppbFirstElement         -  Filled out with pointer to first
//                                     element after header info.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Walks the underlying binary data for a SPNEGO_TOKEN data structure
//    and determines the type of the underlying token based on token header
//    information.
//
////////////////////////////////////////////////////////////////////////////

int InitSpnegoTokenType( SPNEGO_TOKEN* pSpnegoToken, long* pnTokenLength,
                           long* pnRemainingTokenLength, unsigned char** ppbFirstElement )
{
   int   nReturn = SPNEGO_E_INVALID_TOKEN;
   long  nActualTokenLength = 0L;
   long  nBoundaryLength = pSpnegoToken->ulBinaryDataLen;
   unsigned char* pbTokenData = pSpnegoToken->pbBinaryData;

   //
   // First byte MUST be either an APP_CONSTRUCT or the NEGTARG_TOKEN_TARG
   //

   if ( SPNEGO_NEGINIT_APP_CONSTRUCT == *pbTokenData )
   {
      // Validate the above token - this will tell us the actual length of the token
      // per the encoding (minus the actual token bytes)
      if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_NEGINIT_APP_CONSTRUCT, 0L, nBoundaryLength,
                                          pnTokenLength, &nActualTokenLength ) )
                       == SPNEGO_E_SUCCESS )
      {
         // Initialize the remaining token length value.  This will be used
         // to tell the caller how much token there is left once we've parsed
         // the header (they could calculate it from the other values, but this
         // is a bit friendlier)
         *pnRemainingTokenLength = *pnTokenLength;

         // Make adjustments to next token
         pbTokenData += nActualTokenLength;
         nBoundaryLength -= nActualTokenLength;

         // The next token should be an OID
         if ( ( nReturn = ASNDerCheckOID( pbTokenData, spnego_mech_oid_Spnego, nBoundaryLength,
                                          &nActualTokenLength ) ) == SPNEGO_E_SUCCESS )
         {
            // Make adjustments to next token
            pbTokenData += nActualTokenLength;
            nBoundaryLength -= nActualTokenLength;
            *pnRemainingTokenLength -= nActualTokenLength;

            // The next token should specify the NegTokenInit
            if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_NEGINIT_TOKEN_IDENTIFIER,
                                                *pnRemainingTokenLength, nBoundaryLength, pnTokenLength,
                                                &nActualTokenLength ) )
                             == SPNEGO_E_SUCCESS )
            {
               // Make adjustments to next token
               pbTokenData += nActualTokenLength;
               nBoundaryLength -= nActualTokenLength;
               *pnRemainingTokenLength -= nActualTokenLength;

               // The next token should specify the start of a sequence
               if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                                   *pnRemainingTokenLength, nBoundaryLength, pnTokenLength,
                                                   &nActualTokenLength ) )
                                == SPNEGO_E_SUCCESS )
               {
                  // NegTokenInit header is now checked out!

                  // Make adjustments to next token
                  *pnRemainingTokenLength -= nActualTokenLength;

                  // Store pointer to first element
                  *ppbFirstElement = pbTokenData + nActualTokenLength;
                  pSpnegoToken->ucTokenType = SPNEGO_TOKEN_INIT;
               }  // IF Check Sequence Token

            }  // IF Check NegTokenInit token


         }  // IF Check for SPNEGO OID


      }  // IF check app construct token

   }
   else if ( SPNEGO_NEGTARG_TOKEN_IDENTIFIER == *pbTokenData )
   {

      // The next token should specify the NegTokenInit
      if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_NEGTARG_TOKEN_IDENTIFIER,
                                          *pnRemainingTokenLength, nBoundaryLength, pnTokenLength,
                                          &nActualTokenLength ) )
                       == SPNEGO_E_SUCCESS )
      {
         // Initialize the remaining token length value.  This will be used
         // to tell the caller how much token there is left once we've parsed
         // the header (they could calculate it from the other values, but this
         // is a bit friendlier)
         *pnRemainingTokenLength = *pnTokenLength;

         // Make adjustments to next token
         pbTokenData += nActualTokenLength;
         nBoundaryLength -= nActualTokenLength;

         // The next token should specify the start of a sequence
         if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                             *pnRemainingTokenLength, nBoundaryLength, pnTokenLength,
                                             &nActualTokenLength ) )
                          == SPNEGO_E_SUCCESS )
         {
            // NegTokenInit header is now checked out!

            // Make adjustments to next token
            *pnRemainingTokenLength -= nActualTokenLength;

            // Store pointer to first element
            *ppbFirstElement = pbTokenData + nActualTokenLength;
            pSpnegoToken->ucTokenType = SPNEGO_TOKEN_TARG;
         }  // IF Check Sequence Token

      }  // IF Check NegTokenInit token

   }  // ELSE IF it's a NegTokenTarg

   return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    GetSpnegoInitTokenMechList
//
// Parameters:
//    [in]  pbTokenData             -  Points to binary MechList element
//                                     in NegTokenInit.
//    [in]  nMechListLength         -  Length of the MechList
//    [out] pSpnegoElement          -  Filled out with MechList Element
//                                     data.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Checks that pbTokenData is pointing at something that at least
//    *looks* like a MechList and then fills out the supplied
//    SPNEGO_ELEMENT structure.
//
////////////////////////////////////////////////////////////////////////////

int GetSpnegoInitTokenMechList( unsigned char* pbTokenData, int nMechListLength,
                                 SPNEGO_ELEMENT* pSpnegoElement )
{
   int   nReturn = SPNEGO_E_INVALID_TOKEN;
   long  nLength = 0L;
   long  nActualTokenLength = 0L;

   // Actual MechList is prepended by a Constructed Sequence Token
   if ( ( nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                       nMechListLength, nMechListLength,
                                       &nLength, &nActualTokenLength ) )
                             == SPNEGO_E_SUCCESS )
   {
      // Adjust for this token
      nMechListLength -= nActualTokenLength;
      pbTokenData += nActualTokenLength;

      // Perform simple validation of the actual MechList (i.e. ensure that
      // the OIDs in the MechList are reasonable).

      if ( ( nReturn = ValidateMechList( pbTokenData, nLength ) ) == SPNEGO_E_SUCCESS )
      {
         // Initialize the element now
         pSpnegoElement->eElementType = spnego_init_mechtypes;
         pSpnegoElement->iElementPresent = SPNEGO_TOKEN_ELEMENT_AVAILABLE;
         pSpnegoElement->type = SPNEGO_MECHLIST_TYPE;
         pSpnegoElement->nDatalength = nLength;
         pSpnegoElement->pbData = pbTokenData;
      }

   }  // IF Check Token

   return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitSpnegoTokenElementFromBasicType
//
// Parameters:
//    [in]  pbTokenData             -  Points to binary element data in
//                                     a SPNEGO token.
//    [in]  nElementLength          -  Length of the element
//    [in]  ucExpectedType          -  Expected DER type.
//    [in]  spnegoElementType       -  Which element is this?
//    [out] pSpnegoElement          -  Filled out with element data.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Checks that pbTokenData is pointing at the specified DER type.  If so,
//    then we verify that lengths are proper and then fill out the
//    SPNEGO_ELEMENT data structure.
//
////////////////////////////////////////////////////////////////////////////

int InitSpnegoTokenElementFromBasicType( unsigned char* pbTokenData, int nElementLength,
                                          unsigned char ucExpectedType,
                                          SPNEGO_ELEMENT_TYPE spnegoElementType,
                                          SPNEGO_ELEMENT* pSpnegoElement )
{
   int   nReturn = SPNEGO_E_UNEXPECTED_TYPE;
   long  nLength = 0L;
   long  nActualTokenLength = 0L;

   // The type BYTE must match our token data or something is badly wrong
   if ( *pbTokenData == ucExpectedType )
   {

      // Check that we are pointing at the specified type
      if ( ( nReturn = ASNDerCheckToken( pbTokenData, ucExpectedType,
                                          nElementLength, nElementLength,
                                          &nLength, &nActualTokenLength ) )
                                == SPNEGO_E_SUCCESS )
      {
         // Adjust for this token
         nElementLength -= nActualTokenLength;
         pbTokenData += nActualTokenLength;

         // Initialize the element now
         pSpnegoElement->eElementType = spnegoElementType;
         pSpnegoElement->iElementPresent = SPNEGO_TOKEN_ELEMENT_AVAILABLE;
         pSpnegoElement->type = ucExpectedType;
         pSpnegoElement->nDatalength = nLength;
         pSpnegoElement->pbData = pbTokenData;
      }

   }  // IF type makes sense

   return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitSpnegoTokenElementFromOID
//
// Parameters:
//    [in]  pbTokenData             -  Points to binary element data in
//                                     a SPNEGO token.
//    [in]  nElementLength          -  Length of the element
//    [in]  spnegoElementType       -  Which element is this?
//    [out] pSpnegoElement          -  Filled out with element data.
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Initializes a SpnegoElement from an OID - normally, this would have
//    used the Basic Type function above, but since we do binary compares
//    on the OIDs against the DER information as well as the OID, we need
//    to account for that.
//
////////////////////////////////////////////////////////////////////////////

int InitSpnegoTokenElementFromOID( unsigned char* pbTokenData, int nElementLength,
                                   SPNEGO_ELEMENT_TYPE spnegoElementType,
                                   SPNEGO_ELEMENT* pSpnegoElement )
{
   int   nReturn = SPNEGO_E_UNEXPECTED_TYPE;
   long  nLength = 0L;
   long  nActualTokenLength = 0L;

   // The type BYTE must match our token data or something is badly wrong
   if ( *pbTokenData == OID )
   {

      // Check that we are pointing at an OID type
      if ( ( nReturn = ASNDerCheckToken( pbTokenData, OID,
                                          nElementLength, nElementLength,
                                          &nLength, &nActualTokenLength ) )
                                == SPNEGO_E_SUCCESS )
      {
         // Don't adjust any values for this function

         // Initialize the element now
         pSpnegoElement->eElementType = spnegoElementType;
         pSpnegoElement->iElementPresent = SPNEGO_TOKEN_ELEMENT_AVAILABLE;
         pSpnegoElement->type = OID;
         pSpnegoElement->nDatalength = nElementLength;
         pSpnegoElement->pbData = pbTokenData;
      }

   }  // IF type makes sense

   return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitSpnegoTokenElements
//
// Parameters:
//    [in]  pSpnegoToken            -  Points to SPNEGO_TOKEN struct
//    [in]  pbTokenData             -  Points to initial binary element
//                                     data in a SPNEGO token.
//    [in]  nRemainingTokenLength   -  Length remaining past header
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Interprets the data at pbTokenData based on the TokenType in
//    pSpnegoToken.  Since some elements are optional (technically all are
//    but the token becomes quite useless if this is so), we check if
//    an element exists before filling out the element in the array.
//
////////////////////////////////////////////////////////////////////////////

int InitSpnegoTokenElements( SPNEGO_TOKEN* pSpnegoToken, unsigned char* pbTokenData,
                           long nRemainingTokenLength  )
{
   //
   // The following arrays contain the token identifiers for the elements
   // comprising the actual token.  All values are optional, and there are
   // no defaults.
   //

   static unsigned char abNegTokenInitElements[] =
      { SPNEGO_NEGINIT_ELEMENT_MECHTYPES, SPNEGO_NEGINIT_ELEMENT_REQFLAGS,
         SPNEGO_NEGINIT_ELEMENT_MECHTOKEN, SPNEGO_NEGINIT_ELEMENT_MECHLISTMIC };

   static unsigned char abNegTokenTargElements[] =
      { SPNEGO_NEGTARG_ELEMENT_NEGRESULT, SPNEGO_NEGTARG_ELEMENT_SUPPORTEDMECH,
         SPNEGO_NEGTARG_ELEMENT_RESPONSETOKEN, SPNEGO_NEGTARG_ELEMENT_MECHLISTMIC };

   int   nReturn = SPNEGO_E_SUCCESS;
   int   nCtr = 0L;
   long  nElementLength = 0L;
   long  nActualTokenLength = 0L;
   unsigned char* pbElements = NULL;

   // Point to the correct array
   switch( pSpnegoToken->ucTokenType )
   {
      case SPNEGO_TOKEN_INIT:
      {
         pbElements = abNegTokenInitElements;
      }
      break;

      case SPNEGO_TOKEN_TARG:
      {
         pbElements = abNegTokenTargElements;
      }
      break;

   }  // SWITCH tokentype

   //
   // Enumerate the element arrays and look for the tokens at our current location
   //

   for ( nCtr = 0L;
         SPNEGO_E_SUCCESS == nReturn &&
         nCtr < MAX_NUM_TOKEN_ELEMENTS &&
         nRemainingTokenLength > 0L;
         nCtr++ )
   {

      // Check if the token exists
      if ( ( nReturn = ASNDerCheckToken( pbTokenData, pbElements[nCtr],
                                          0L, nRemainingTokenLength,
                                          &nElementLength, &nActualTokenLength ) )
                                == SPNEGO_E_SUCCESS )
      {

         // Token data should skip over the sequence token and then
         // call the appropriate function to initialize the element
         pbTokenData += nActualTokenLength;

         // Lengths in the elements should NOT go beyond the element
         // length

         // Different tokens mean different elements
         if ( SPNEGO_TOKEN_INIT == pSpnegoToken->ucTokenType )
         {

            // Handle each element as appropriate
            switch( pbElements[nCtr] )
            {

               case SPNEGO_NEGINIT_ELEMENT_MECHTYPES:
               {
                  //
                  // This is a Mech List that specifies which OIDs the
                  // originator of the Init Token supports.
                  //

                  nReturn = GetSpnegoInitTokenMechList( pbTokenData, nElementLength,
                                                         &pSpnegoToken->aElementArray[nCtr] );

               }
               break;

               case SPNEGO_NEGINIT_ELEMENT_REQFLAGS:
               {
                  //
                  // This is a BITSTRING which specifies the flags that the receiver
                  // pass to the gss_accept_sec_context() function.
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                  BITSTRING, spnego_init_reqFlags,
                                                                  &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

               case SPNEGO_NEGINIT_ELEMENT_MECHTOKEN:
               {
                  //
                  // This is an OCTETSTRING which contains a GSSAPI token corresponding
                  // to the first OID in the MechList.
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                  OCTETSTRING, spnego_init_mechToken,
                                                                  &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

               case SPNEGO_NEGINIT_ELEMENT_MECHLISTMIC:  // xA3
               {
                  //
                  // Don't yet know if this is a negTokenInit, or negTokenInit2.
                  // Unfortunately, both have the same type: SPNEGO_TOKEN_INIT
                  // If it's negTokenInit, this element should be an OCTETSTRING
                  // containing the MIC.  If it's a negTokenInit2, this element
                  // should be an SPNEGO_CONSTRUCTED_SEQUENCE containing the
                  // negHints (GENERALSTR, ignored)
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                 OCTETSTRING, spnego_init_mechListMIC,
                                                                 &pSpnegoToken->aElementArray[nCtr] );

                  if (nReturn == SPNEGO_E_UNEXPECTED_TYPE) {
                     // This is really a negHints element.  Check the type and length,
                     // but otherwise just ignore it.
                     long  elen, tlen;
                     nReturn = ASNDerCheckToken( pbTokenData, SPNEGO_CONSTRUCTED_SEQUENCE,
                                          nElementLength, nElementLength,
                                          &elen, &tlen );
                  }
               }
               break;

            }  // SWITCH Element
         }
         else
         {
            /* pSpnegoToken->ucTokenType == SPNEGO_TOKEN_TARG */

            switch( pbElements[nCtr] )
            {

               case SPNEGO_NEGTARG_ELEMENT_NEGRESULT:
               {
                  //
                  // This is an ENUMERATION which specifies result of the last GSS
                  // token negotiation call.
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                  ENUMERATED, spnego_targ_negResult,
                                                                  &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

               case SPNEGO_NEGTARG_ELEMENT_SUPPORTEDMECH:
               {
                  //
                  // This is an OID which specifies a supported mechanism.
                  //

                  nReturn = InitSpnegoTokenElementFromOID( pbTokenData, nElementLength,
                                                           spnego_targ_mechListMIC,
                                                           &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

               case SPNEGO_NEGTARG_ELEMENT_RESPONSETOKEN:
               {
                  //
                  // This is an OCTETSTRING which specifies results of the last GSS
                  // token negotiation call.
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                  OCTETSTRING, spnego_targ_responseToken,
                                                                  &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

               case SPNEGO_NEGTARG_ELEMENT_MECHLISTMIC:
               {
                  //
                  // This is an OCTETSTRING, typically 16 bytes,
                  // which contains a message integrity BLOB.
                  //

                  nReturn = InitSpnegoTokenElementFromBasicType( pbTokenData, nElementLength,
                                                                 OCTETSTRING, spnego_targ_mechListMIC,
                                                                 &pSpnegoToken->aElementArray[nCtr] );
               }
               break;

            }  // SWITCH Element

         }  // ELSE !NegTokenInit

         // Account for the entire token and following data
         nRemainingTokenLength -= ( nActualTokenLength + nElementLength );

         // Token data should skip past the element length now
         pbTokenData += nElementLength;

      }  // IF Token found
      else if ( SPNEGO_E_TOKEN_NOT_FOUND == nReturn )
      {
         // For now, this is a benign error (remember, all elements are optional, so
         // if we don't find one, it's okay).

         nReturn = SPNEGO_E_SUCCESS;
      }

   }  // FOR enum elements

   //
   // We should always run down to 0 remaining bytes in the token.  If not, we've got
   // a bad token.
   //

   if ( SPNEGO_E_SUCCESS == nReturn && nRemainingTokenLength != 0L )
   {
      nReturn = SPNEGO_E_INVALID_TOKEN;
   }

   return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    FindMechOIDInMechList
//
// Parameters:
//    [in]  pSpnegoElement          -  SPNEGO_ELEMENT for MechList
//    [in]  MechOID                 -  OID we're looking for.
//    [out] piMechTypeIndex         -  Index in the list where OID was
//                                     found
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Walks the MechList for MechOID.  When it is found, the index in the
//    list is written to piMechTypeIndex.
//
////////////////////////////////////////////////////////////////////////////

int FindMechOIDInMechList( SPNEGO_ELEMENT* pSpnegoElement, SPNEGO_MECH_OID MechOID,
                          int * piMechTypeIndex )
{
   int   nReturn = SPNEGO_E_NOT_FOUND;
   int   nCtr = 0;
   long  nLength = 0L;
   long  nBoundaryLength = pSpnegoElement->nDatalength;
   unsigned char* pbMechListData = pSpnegoElement->pbData;

   while( SPNEGO_E_SUCCESS != nReturn && nBoundaryLength > 0L )
   {

      // Use the helper function to check the OID
      if ( ( nReturn = ASNDerCheckOID( pbMechListData, MechOID, nBoundaryLength, &nLength ) )
                     == SPNEGO_E_SUCCESS )
      {
         *piMechTypeIndex = nCtr;
      }

      // Adjust for the current OID
      pbMechListData += nLength;
      nBoundaryLength -= nLength;
      nCtr++;

   }  // WHILE enuming OIDs

   return nReturn;

}


/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    ValidateMechList
//
// Parameters:
//    [in]  pbMechListData          -  Pointer to binary MechList data
//    [in]  nBoundaryLength         -  Length we must not exceed
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Checks the data at pbMechListData to see if it looks like a MechList.
//    As part of this, we walk the list and ensure that none of the OIDs
//    have a length that takes us outside of nBoundaryLength.
//
////////////////////////////////////////////////////////////////////////////

int ValidateMechList( unsigned char* pbMechListData, long nBoundaryLength )
{
   int   nReturn = SPNEGO_E_SUCCESS;
   long  nLength = 0L;
   long  nTokenLength = 0L;

   while( SPNEGO_E_SUCCESS == nReturn && nBoundaryLength > 0L )
   {
      // Verify that we have something that at least *looks* like an OID - in other
      // words it has an OID identifier and specifies a length that doesn't go beyond
      // the size of the list.
      nReturn = ASNDerCheckToken( pbMechListData, OID, 0L, nBoundaryLength,
                                  &nLength, &nTokenLength );

      // Adjust for the current OID
      pbMechListData += ( nLength + nTokenLength );
      nBoundaryLength -= ( nLength + nTokenLength );

   }  // WHILE enuming OIDs

   return nReturn;

}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    IsValidMechOid
//
// Parameters:
//    [in]  mechOid  -  mechOID id enumeration
//
// Returns:
//    int   Success - 1
//          Failure - 0
//
// Comments :
//    Checks for a valid mechOid value.
//
////////////////////////////////////////////////////////////////////////////

int IsValidMechOid( SPNEGO_MECH_OID mechOid )
{
   return ( mechOid >= spnego_mech_oid_Kerberos_V5_Legacy &&
            mechOid <= spnego_mech_oid_NTLMSSP );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    IsValidContextFlags
//
// Parameters:
//    [in]  ucContextFlags -  ContextFlags value
//
// Returns:
//    int   Success - 1
//          Failure - 0
//
// Comments :
//    Checks for a valid ContextFlags value.
//
////////////////////////////////////////////////////////////////////////////

int IsValidContextFlags( unsigned char ucContextFlags )
{
   // Mask out our valid bits.  If there is anything leftover, this
   // is not a valid value for Context Flags
   return ( ( ucContextFlags & ~SPNEGO_NEGINIT_CONTEXT_MASK ) == 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    IsValidNegResult
//
// Parameters:
//    [in]  negResult   -  NegResult value
//
// Returns:
//    int   Success - 1
//          Failure - 0
//
// Comments :
//    Checks for a valid NegResult value.
//
////////////////////////////////////////////////////////////////////////////

int IsValidNegResult( SPNEGO_NEGRESULT negResult )
{
   return ( negResult >= spnego_negresult_success &&
            negResult <= spnego_negresult_request_mic );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    IsValidSpnegoToken
//
// Parameters:
//    [in]  pSpnegoToken   -  Points to SPNEGO_TOKEN data structure
//
// Returns:
//    int   Success - 1
//          Failure - 0
//
// Comments :
//    Performs simple heuristic on location pointed to by pSpnegoToken.
//
////////////////////////////////////////////////////////////////////////////

int IsValidSpnegoToken( SPNEGO_TOKEN* pSpnegoToken )
{
   int   nReturn = 0;

   // Parameter should be non-NULL
   if ( NULL != pSpnegoToken )
   {
      // Length should be at least the size defined in the header
      if ( pSpnegoToken->nStructSize >= SPNEGO_TOKEN_SIZE )
      {
         // Number of elements should be >= our maximum - if it's greater, that's
         // okay, since we'll only be accessing the elements up to MAX_NUM_TOKEN_ELEMENTS
         if ( pSpnegoToken->nNumElements >= MAX_NUM_TOKEN_ELEMENTS )
         {
            // Check for proper token type
            if ( SPNEGO_TOKEN_INIT == pSpnegoToken->ucTokenType ||
               SPNEGO_TOKEN_TARG == pSpnegoToken->ucTokenType )
            {
               nReturn = 1;
            }
         }

      }  // IF struct size makes sense

   }  // IF non-NULL spnego Token

   return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    IsValidSpnegoElement
//
// Parameters:
//    [in]  pSpnegoToken   -  Points to SPNEGO_TOKEN data structure
//    [in]  spnegoElement  -  spnegoElement Type from enumeration
//
// Returns:
//    int   Success - 1
//          Failure - 0
//
// Comments :
//    Checks that spnegoElement has a valid value and is appropriate for
//    the SPNEGO token encapsulated by pSpnegoToken.
//
////////////////////////////////////////////////////////////////////////////

int IsValidSpnegoElement( SPNEGO_TOKEN* pSpnegoToken,SPNEGO_ELEMENT_TYPE spnegoElement )
{
   int   nReturn = 0;

   // Check boundaries
   if ( spnegoElement > spnego_element_min &&
      spnegoElement < spnego_element_max )
   {

      // Check for appropriateness to token type
      if ( SPNEGO_TOKEN_INIT == pSpnegoToken->ucTokenType )
      {
         nReturn = ( spnegoElement >= spnego_init_mechtypes &&
                     spnegoElement <= spnego_init_mechListMIC );
      }
      else
      {
         nReturn = ( spnegoElement >= spnego_targ_negResult &&
                     spnegoElement <= spnego_targ_mechListMIC );
      }

   }  // IF boundary conditions are met

   return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    CalculateElementArrayIndex
//
// Parameters:
//    [in]  pSpnegoToken   -  Points to SPNEGO_TOKEN data structure
//    [in]  spnegoElement  -  spnegoElement Type from enumeration
//
// Returns:
//    int   index in the SPNEGO_TOKEN element array that the element can
//          can be found
//
// Comments :
//    Based on the Token Type, calculates the index in the element array
//    at which the specified element can be found.
//
////////////////////////////////////////////////////////////////////////////

int CalculateElementArrayIndex( SPNEGO_TOKEN* pSpnegoToken,SPNEGO_ELEMENT_TYPE spnegoElement )
{
   int   nReturn = 0;

   // Offset is difference between value and initial element identifier
   // (these differ based on ucTokenType)

   if ( SPNEGO_TOKEN_INIT == pSpnegoToken->ucTokenType )
   {
      nReturn = spnegoElement - spnego_init_mechtypes;
   }
   else
   {
      nReturn = spnegoElement - spnego_targ_negResult;
   }

   return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:
//    InitTokenFromBinary
//
// Parameters:
//    [in]  ucCopyData     -  Flag indicating if data should be copied
//    [in]  ulFlags        -  Flags value for structure
//    [in]  pnTokenData    -  Binary Token Data
//    [in]  ulLength       -  Length of the data
//    [out] ppSpnegoToken  -  Pointer to call allocated SPNEGO Token
//                            data structure
//
// Returns:
//    int   Success - SPNEGO_E_SUCCESS
//          Failure - SPNEGO API Error code
//
// Comments :
//    Allocates a SPNEGO_TOKEN data structure and fills it out as
//    appropriate based in the flags passed into the function.
//
////////////////////////////////////////////////////////////////////////////


// Initializes SPNEGO_TOKEN structure from DER encoded binary data
int InitTokenFromBinary( unsigned char ucCopyData, unsigned long ulFlags,
                        unsigned char* pbTokenData, unsigned long ulLength,
                        SPNEGO_TOKEN** ppSpnegoToken )
{
   int            nReturn = SPNEGO_E_INVALID_PARAMETER;
   SPNEGO_TOKEN*  pSpnegoToken = NULL;
   unsigned char* pbFirstElement = NULL;
   long           nTokenLength = 0L;
   long           nRemainingTokenLength = 0L;

   // Basic Parameter Validation

   if (  NULL != pbTokenData &&
         NULL != ppSpnegoToken &&
         0L != ulLength )
   {

      //
      // Allocate the empty token, then initialize the data structure.
      //

      pSpnegoToken = AllocEmptySpnegoToken( ucCopyData, ulFlags, pbTokenData, ulLength );

      if ( NULL != pSpnegoToken )
      {

         // Copy the binary data locally


         // Initialize the token type
         if ( ( nReturn = InitSpnegoTokenType( pSpnegoToken, &nTokenLength,
                                                &nRemainingTokenLength, &pbFirstElement ) )
                        == SPNEGO_E_SUCCESS )
         {

            // Initialize the element array
            if ( ( nReturn = InitSpnegoTokenElements( pSpnegoToken, pbFirstElement,
                                                      nRemainingTokenLength ) )
                           == SPNEGO_E_SUCCESS )
            {
               *ppSpnegoToken = pSpnegoToken;
            }

         }  // IF Init Token Type

         // Cleanup on error condition
         if ( SPNEGO_E_SUCCESS != nReturn )
         {
            spnegoFreeData( pSpnegoToken );
         }

      }
      else
      {
         nReturn = SPNEGO_E_OUT_OF_MEMORY;
      }

   }  // IF Valid parameters


   return nReturn;
}
