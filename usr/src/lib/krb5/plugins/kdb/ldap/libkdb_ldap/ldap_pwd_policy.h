/*
 * lib/kdb/kdb_ldap/ldap_pwd_policy.h
 *
 * Copyright (c) 2004-2005, Novell, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *   * The copyright holder's name is not used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LDAP_KRBPWDPOLICY_H_
#define _LDAP_KRBPWDPOLICY_H_

krb5_error_code
krb5_ldap_get_password_policy (krb5_context , char *, osa_policy_ent_t *, int *);

krb5_error_code
krb5_ldap_create_password_policy (krb5_context , osa_policy_ent_t );

krb5_error_code
krb5_ldap_put_password_policy ( krb5_context kcontext, osa_policy_ent_t policy );

krb5_error_code
krb5_ldap_delete_password_policy ( krb5_context kcontext, char *policy );

krb5_error_code
krb5_ldap_iterate_password_policy(krb5_context, char *,
				  void (*) (krb5_pointer, osa_policy_ent_t ),
				  krb5_pointer);

void
krb5_ldap_free_password_policy( krb5_context kcontext, osa_policy_ent_t entry );

#endif
