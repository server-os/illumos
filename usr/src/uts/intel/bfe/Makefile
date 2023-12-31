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

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE		= ../..

#
#	Define the module and object file sets.
#
MODULE		= bfe
OBJECTS		= $(BFE_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

#
#	GENERAL PURPOUSE GEM FLAGS: Tuning GEM for Solaris specific modes
#
VFLAGS		= -DVERSION='"2.6.1"'
AFLAGS		= -Di86pc
DFLAGS		= -D"__INLINE__="
CFGFLAGS	= -DGEM_CONFIG_POLLING -DGEM_CONFIG_GLDv3 -DGEM_CONFIG_VLAN \
	-DGEM_CONFIG_CKSUM_OFFLOAD -DGEM_CONFIG_ND \
	-DCONFIG_DP83815 -DCONFIG_SIS900 -DCONFIG_SIS7016 \
	-DCONFIG_MAC_ADDR_SIS630E -DCONFIG_OPT_IO -UCONFIG_OO \
	-DCONFIG_PATTERN_MATCH_DP83815
#
#	FAST PATH SECTION: Will activate usage of inlines as a regular functions
#	on fast data path

CPPFLAGS	+= $(VFLAGS) $(AFLAGS) $(DFLAGS) $(CFGFLAGS) $(CCVERBOSE) \
	-I$(UTSBASE)/common/io/bfe

CFLAGS		+= $(CPPFLAGS)

CERRWARN	+= -_gcc=-Wno-switch
CERRWARN	+= $(CNOWARN_UNINIT)

#
# Driver depends on MAC & IP
#
LDFLAGS		+= -N misc/mac -N drv/ip

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
