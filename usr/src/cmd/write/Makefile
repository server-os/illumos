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
# Copyright (c) 2018, Joyent, Inc.

PROG= write

include ../Makefile.cmd

DCFILE= $(PROG).dc

FILEMODE = 02555
XGETFLAGS += -a -x write.xcl

.KEEP_STATE:

CFLAGS += $(CCVERBOSE)
CERRWARN += -_gcc=-Wno-unused-variable

SMOFF += all_func_returns

all: $(PROG)

install: all $(ROOTPROG)

clean:

lint:	lint_PROG

$(DCFILE):
	$(RM) messages.po
	$(COMPILE.cpp) $(PROG).c | \
	$(XGETTEXT) -c TRANSLATION_NOTE -t -
	$(SED) -e '/^domain/d' messages.po > $@
	$(RM) messages.po

include ../Makefile.targ
