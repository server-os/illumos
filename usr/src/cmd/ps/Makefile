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
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# cmd/ps/Makefile
#

PROG= ps
OBJS=ps.o ucbps.o
SRCS=$(OBJS:%.o=%.c)

include ../Makefile.cmd
include ../Makefile.cmd.64
include ../Makefile.ctf

LDLIBS += -lproject

XGETFLAGS += -a -x ps.xcl

DCFILE= $(PROG).dc

ROOTUCBPROG = $(ROOT)/usr/ucb/$(PROG)

.KEEP_STATE:

all:	$(PROG)

$(PROG): $(OBJS)
	$(LINK.c) -o $@ $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

install:	$(PROG) $(ROOTPROG)
	-$(RM) $(ROOTUCBPROG)
	-$(LN) $(ROOTPROG) $(ROOTUCBPROG)

clean:
	$(RM) $(OBJS)

$(DCFILE): $(PROG).c
	$(RM) $(DCFILE)
	$(COMPILE.cpp) $(PROG).c  | \
	$(XGETTEXT) -c TRANSLATION_NOTE -t -
	$(SED) -e '/^domain/d' messages.po > $@
	$(RM) messages.po

include ../Makefile.targ
