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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma D option quiet

struct in6_addr *ip6a;
struct in6_addr *ip6b;
struct in6_addr *ip6c;
struct in6_addr *ip6d;
struct in6_addr *ip6e;
struct in6_addr *ip6f;
struct in6_addr *ip6g;

BEGIN
{
	this->buf6a = alloca(sizeof (struct in6_addr));
	this->buf6b = alloca(sizeof (struct in6_addr));
	this->buf6c = alloca(sizeof (struct in6_addr));
	this->buf6d = alloca(sizeof (struct in6_addr));
	this->buf6e = alloca(sizeof (struct in6_addr));
	this->buf6f = alloca(sizeof (struct in6_addr));
	this->buf6g = alloca(sizeof (struct in6_addr));
	ip6a = this->buf6a;
	ip6b = this->buf6b;
	ip6c = this->buf6c;
	ip6d = this->buf6d;
	ip6e = this->buf6e;
	ip6f = this->buf6f;
	ip6g = this->buf6g;

	ip6a->_S6_un._S6_u8[0] = 0xfe;
	ip6a->_S6_un._S6_u8[1] = 0x80;
	ip6a->_S6_un._S6_u8[8] = 0x02;
	ip6a->_S6_un._S6_u8[9] = 0x14;
	ip6a->_S6_un._S6_u8[10] = 0x4f;
	ip6a->_S6_un._S6_u8[11] = 0xff;
	ip6a->_S6_un._S6_u8[12] = 0xfe;
	ip6a->_S6_un._S6_u8[13] = 0x0b;
	ip6a->_S6_un._S6_u8[14] = 0x76;
	ip6a->_S6_un._S6_u8[15] = 0xc8;
	ip6b->_S6_un._S6_u8[0] = 0x10;
	ip6b->_S6_un._S6_u8[1] = 0x80;
	ip6b->_S6_un._S6_u8[10] = 0x08;
	ip6b->_S6_un._S6_u8[11] = 0x08;
	ip6b->_S6_un._S6_u8[13] = 0x20;
	ip6b->_S6_un._S6_u8[13] = 0x0c;
	ip6b->_S6_un._S6_u8[14] = 0x41;
	ip6b->_S6_un._S6_u8[15] = 0x7a;
	ip6c->_S6_un._S6_u8[15] = 0x01;
	ip6e->_S6_un._S6_u8[12] = 0x7f;
	ip6e->_S6_un._S6_u8[15] = 0x01;
	ip6f->_S6_un._S6_u8[10] = 0xff;
	ip6f->_S6_un._S6_u8[11] = 0xff;
	ip6f->_S6_un._S6_u8[12] = 0x7f;
	ip6f->_S6_un._S6_u8[15] = 0x01;
	ip6g->_S6_un._S6_u8[10] = 0xff;
	ip6g->_S6_un._S6_u8[11] = 0xfe;
	ip6g->_S6_un._S6_u8[12] = 0x7f;
	ip6g->_S6_un._S6_u8[15] = 0x01;

	printf("%s\n", inet_ntoa6(ip6a));
	printf("%s\n", inet_ntoa6(ip6b));
	printf("%s\n", inet_ntoa6(ip6c));
	printf("%s\n", inet_ntoa6(ip6d));
	printf("%s\n", inet_ntoa6(ip6e));
	printf("%s\n", inet_ntoa6(ip6f));
	printf("%s\n", inet_ntoa6(ip6g));

	exit(0);
}
