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
# Copyright 2018 Nexenta Systems, Inc.  All rights reserved.
# Copyright (c) 2018, Joyent, Inc.
#

#
# cmd/fs.d/smbclnt/smbutil/Makefile
#

PROG=		smbutil

OBJS=   smbutil.o discon.o info.o login.o lookup.o print.o status.o view.o \
	shares_rpc.o srvsvc1_clnt.o srvsvc1_ndr.o

include		$(SRC)/cmd/Makefile.cmd

POFILE=	smbutil_all.po
POFILES= $(OBJS:%.o=%.po)

CLEANFILES +=  srvsvc1_ndr.c
CLOBBERFILES+=  $(POFILE) $(POFILES)

CSTD= $(CSTD_GNU99)

LDLIBS += -lmlrpc -lsmbfs -lnsl

CPPFLAGS += -I$(SRC)/lib/libsmbfs \
	-I$(SRC)/uts/common/smbclnt -I$(SRC)/uts/common

# not linted
SMATCH=off

# Debugging
$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG

all:		$(PROG)

$(PROG):	$(OBJS)
	$(LINK.c) -o $@ $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

install:	all $(ROOTPROG)

$(ROOTPROG):	$(PROG)
	$(INS.file) $(PROG)

catalog: $(POFILE)

$(POFILE): $(POFILES)
	$(RM) $@
	$(CAT) $(POFILES) > $@

clean:
	$(RM) $(OBJS) $(CLEANFILES)

include ../../../Makefile.targ

srvsvc1_ndr.c : srvsvc1.ndl
	$(NDRGEN) -Y $(ANSI_CPP) $(CPPFLAGS) srvsvc1.ndl

.KEEP_STATE:
