#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
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
# Copyright (c) 2000-2001 by Sun Microsystems, Inc.
# All rights reserved.
#
# lib/libfru/libfrureg/sparcv9/Makefile
#

include ../Makefile.com
include $(SRC)/lib/Makefile.lib.64

LDLIBS +=	-L$(SRC)/lib/libfruutils/$(MACH64)
LDLIBS +=	-L$(SRC)/lib/libfru/$(MACH64)
LDLIBS +=	-lc -lfruutils

.KEEP_STATE:

all: $(ROOTLIBDIR64) $(LIBS) $(LIBLINKS)

$(LIBLINKS):    FRC
	$(RM) $@; $(SYMLINK) $(DYNLIB) $@

$(ROOTLIBDIR64):
	$(INS.dir)

install: all $(ROOTLIBS64) $(ROOTLINKS64)

FRC:
