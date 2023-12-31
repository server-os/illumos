/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation. Portions created by Netscape are
 * Copyright (C) 1998-1999 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 */

#ifndef LDAP_PR_H
#define LDAP_PR_H

#include "nspr.h"

/*
 * ldappr.h - prototypes for functions that tie libldap into NSPR (Netscape
 *	Portable Runtime).
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function: prldap_init().
 *
 * Create a new LDAP session handle, but with NSPR I/O, threading, and DNS
 * functions installed.
 *
 * Pass a non-zero value for the 'shared' parameter if you plan to use
 * this LDAP * handle from more than one thread.
 *
 * Returns an LDAP session handle (or NULL if an error occurs).
 */
LDAP * LDAP_CALL prldap_init( const char *defhost, int defport, int shared );


/*
 * Function: prldap_install_routines().
 *
 * Install NSPR I/O, threading, and DNS functions so they will be used by
 * 'ld'.
 *
 * If 'ld' is NULL, the functions are installed as the default functions
 * for all new LDAP * handles).
 *
 * Pass a non-zero value for the 'shared' parameter if you plan to use
 * this LDAP * handle from more than one thread.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well).
 */
int LDAP_CALL prldap_install_routines( LDAP *ld, int shared );


/*
 * Function: prldap_set_session_option().
 *
 * Given an LDAP session handle or a session argument such is passed to
 * CONNECT, POLL, NEWHANDLE, or DISPOSEHANDLE extended I/O callbacks, set
 * an option that affects the prldap layer.
 *
 * If 'ld' and 'session" are both NULL, the option is set as the default
 * for all new prldap sessions.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well).
 */
int LDAP_CALL prldap_set_session_option( LDAP *ld, void *sessionarg,
	int option, ... );


/*
 * Function: prldap_get_session_option().
 *
 * Given an LDAP session handle or a session argument such is passed to
 * CONNECT, POLL, NEWHANDLE, or DISPOSEHANDLE extended I/O callbacks, retrieve
 * the setting for an option that affects the prldap layer.
 *
 * If 'ld' and 'session" are both NULL, the default option value for all new
 * new prldap sessions is retrieved.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well).
 */
int LDAP_CALL prldap_get_session_option( LDAP *ld, void *sessionarg,
	int option, ... );



/*
 * Available options.
 */
/*
 * PRLDAP_OPT_IO_MAX_TIMEOUT: set the maximum time in milliseconds to
 * block waiting for a network I/O operation to complete.
 *
 * Data type: int.
 *
 * These two special values from ldap-extension.h can also be used;
 *
 *    LDAP_X_IO_TIMEOUT_NO_TIMEOUT
 *    LDAP_X_IO_TIMEOUT_NO_WAIT
 */
#define PRLDAP_OPT_IO_MAX_TIMEOUT		1


/**
 ** Note: the types and functions below are only useful for developers
 ** who need to layer one or more custom extended I/O functions on top of
 ** the standard NSPR I/O functions installed by a call to prldap_init()
 ** or prldap_install_routines().  Layering can be accomplished after
 ** prldap_init() or prldap_install_routines() has completed successfully
 ** by:
 **
 **   1) Calling ldap_get_option( ..., LDAP_X_OPT_EXTIO_FN_PTRS, ... ).
 **
 **   2) Saving the function pointer of one or more of the standard functions.
 **
 **   3) Replacing one or more standard functions in the ldap_x_ext_io_fns
 **      struct	with new functions that optionally do some preliminary work,
 **      call the standard function (via the function pointer saved in step 2),
 **      and optionally do some followup work.
 */

/*
 * Data structure for session information.
 * seinfo_size should be set to PRLDAP_SESSIONINFO_SIZE before use.
 */
struct prldap_session_private;

typedef struct prldap_session_info {
	int				seinfo_size;
	struct prldap_session_private	*seinfo_appdata;
} PRLDAPSessionInfo;
#define PRLDAP_SESSIONINFO_SIZE	sizeof( PRLDAPSessionInfo )


/*
 * Function: prldap_set_session_info().
 *
 * Given an LDAP session handle or a session argument such is passed to
 * CONNECT, POLL, NEWHANDLE, or DISPOSEHANDLE extended I/O callbacks,
 * set some application-specific data.  If ld is NULL, arg is used.  If
 * both ld and arg are NULL, LDAP_PARAM_ERROR is returned.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well).
 */
int LDAP_CALL prldap_set_session_info( LDAP *ld, void *sessionarg,
	PRLDAPSessionInfo *seip );


/*
 * Function: prldap_get_session_info().
 *
 * Given an LDAP session handle or a session argument such is passed to
 * CONNECT, POLL, NEWHANDLE, or DISPOSEHANDLE extended I/O callbacks,
 * retrieve some application-specific data.  If ld is NULL, arg is used.  If
 * both ld and arg are NULL, LDAP_PARAM_ERROR is returned.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well, in
 * which case the fields in the structure that seip points to are filled in).
 */
int LDAP_CALL prldap_get_session_info( LDAP *ld, void *sessionarg,
	PRLDAPSessionInfo *seip );


/*
 * Data structure for socket specific information.
 * Note: soinfo_size should be set to PRLDAP_SOCKETINFO_SIZE before use.
 */
struct prldap_socket_private;
typedef struct prldap_socket_info {
	int				soinfo_size;
	PRFileDesc			*soinfo_prfd;
	struct prldap_socket_private	*soinfo_appdata;
} PRLDAPSocketInfo;
#define PRLDAP_SOCKETINFO_SIZE	sizeof( PRLDAPSocketInfo )


/*
 * Function: prldap_set_socket_info().
 *
 * Given an integer fd and a socket argument such as those passed to the
 * extended I/O callback functions, set socket specific information.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well).
 *
 * Note: it is only safe to change soinfo_prfd from within the SOCKET
 * extended I/O callback function.
 */
int LDAP_CALL prldap_set_socket_info( int fd, void *socketarg,
					PRLDAPSocketInfo *soip );

/*
 * Function: prldap_get_socket_info().
 *
 * Given an integer fd and a socket argument such as those passed to the
 * extended I/O callback functions, retrieve socket specific information.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well, in
 * which case the fields in the structure that soip points to are filled in).
 */
int LDAP_CALL prldap_get_socket_info( int fd, void *socketarg,
					PRLDAPSocketInfo *soip );

/*
 * Function: prldap_get_default_socket_info().
 *
 * Given an LDAP session handle, retrieve socket specific information.
 * If ld is NULL, LDAP_PARAM_ERROR is returned.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well, in
 * which case the fields in the structure that soip points to are filled in).
 */
int LDAP_CALL prldap_get_default_socket_info( LDAP *ld, PRLDAPSocketInfo *soip );

/*
 * Function: prldap_set_default_socket_info().
 *
 * Given an LDAP session handle, set socket specific information.
 * If ld is NULL, LDAP_PARAM_ERROR is returned.
 *
 * Returns an LDAP API error code (LDAP_SUCCESS if all goes well, in
 * which case the fields in the structure that soip points to are filled in).
 */
int LDAP_CALL prldap_set_default_socket_info( LDAP *ld, PRLDAPSocketInfo *soip );

#ifdef __cplusplus
}
#endif
#endif /* !defined(LDAP_PR_H) */
