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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright 2019 Joyent, Inc.

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE		= ../..

#
#	Path to where brand common sources live
#
LX_CMN		= $(SRC)/common/brand/lx

#
#	Define the module and object file sets.
#
MODULE		= lx_brand
OBJECTS		= $(LX_BRAND_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(USR_BRAND_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

INC_PATH	+= -I$(UTSBASE)/common/brand/lx -I$(LX_CMN) -I$(SRC)/common
INC_PATH	+= -I$(UTSBASE)/common/inet/sockmods -I$(UTSBASE)/common/io/bpf
INC_PATH	+= -I$(UTSBASE)/common/fs/sockfs
INC_PATH	+= -I$(UTSBASE)/common/fs/zfs
AS_INC_PATH	+= -I$(UTSBASE)/i86pc/genassym/$(OBJS_DIR)

CFLAGS		+= $(CCVERBOSE)

LDFLAGS		+= -Nexec/elfexec -Nfs/fifofs -Nfs/sockfs -Ndrv/ip \
		    -Nfs/zfs -Nmisc/klmmod -Nsys/sysacct

# needs work
SMATCH=off

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/intel/Makefile.targ

#
#	Include brand-specific rules
#

include $(UTSBASE)/intel/lx_brand/Makefile.rules
