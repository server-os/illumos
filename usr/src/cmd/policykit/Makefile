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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright 2019 OmniOS Community Edition (OmniOSce) Association.
#

PROG =	polkit-is-privileged

SRCS =	$(PROG:%=%.c)

include ../Makefile.cmd
include $(SRC)/lib/policykit/Makefile.policykit

LDLIBS +=	-lpolkit $(POLICYKIT_GLIB_LDLIBS)
LDFLAGS +=	$(POLICYKIT_GLIB_LDFLAGS)

CPPFLAGS +=	-I$(ROOT)/usr/include/libpolkit
CPPFLAGS +=	$(POLICYKIT_DBUS_CPPFLAGS) $(POLICYKIT_GLIB_CPPFLAGS)
CPPFLAGS +=	-DPACKAGE_VERSION=\"$(POLICYKIT_VERSION)\"
CSTD =	$(CSTD_GNU99)

CERRWARN +=	-_gcc=-Wno-unused-variable
CERRWARN +=	-_gcc=-Wno-unused-function

# smatch has problems parsing the glib header files
SMATCH=off

ROOTUSRSBINPROG =	$(PROG:%=$(ROOTUSRSBIN)/%)

.KEEP_STATE:

all: $(PROG)

install: all $(ROOTUSRSBINPROG)

clean:
	$(RM) $(CLEANFILES)

FRC:

include ../Makefile.targ
