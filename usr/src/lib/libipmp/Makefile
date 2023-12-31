#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright 2021 Tintri by DDN, Inc.  All rights reserved.
#

include $(SRC)/lib/Makefile.lib

HDRS =		ipmp.h ipmp_admin.h ipmp_mpathd.h ipmp_query.h ipmp_query_impl.h
HDRDIR =	common
SUBDIRS	=	$(MACH)
$(BUILD64)SUBDIRS += $(MACH64)

POFILE =	libipmp.po
MSGFILES =	common/ipmp.c
XGETFLAGS =	-a

TYPECHECK_LIB =	libipmp.so.1
TYPELIST = \
	ipmp_addrinfo_t \
	ipmp_addrlist_t \
	ipmp_groupinfo_xfer_t \
	ipmp_grouplist_t \
	ipmp_ifinfo_xfer_t \
	ipmp_iflist_t \
	ipmp_infotype_t \
	ipmp_targinfo_xfer_t \
	mi_query_t \
	mi_result_t

all :=		TARGET = all
clean :=	TARGET = clean
clobber :=	TARGET = clobber
install	:=	TARGET = install

.KEEP_STATE:

all clean clobber install: $(SUBDIRS)

install_h:	$(ROOTHDRS)

check:		$(CHECKHDRS) $(TYPECHECK)

$(POFILE):	$(MSGFILES)
		$(BUILDPO.msgfiles)

_msg:		$(MSGDOMAINPOFILE)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:

include $(SRC)/Makefile.msg.targ
include $(SRC)/lib/Makefile.targ
