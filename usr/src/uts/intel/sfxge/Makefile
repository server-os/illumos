#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright 2016 Garrett D'Amore <garrett@damore.org>
#
# Copyright (c) 2018, Joyent, Inc.

UTSBASE		= ../..

MODULE		= sfxge
OBJECTS		= $(SFXGE_OBJS:%=$(OBJS_DIR)/%)
OBJECTS		+= $(SFXGE_SF_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)

include $(UTSBASE)/intel/Makefile.intel

#
# Targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

#
# Overrides
#

INC_PATH += -I$(UTSBASE)/common/io/sfxge -I$(UTSBASE)/common/io/sfxge/common

#
# TODO:
# These are specific to this driver.  We will unidef these out later.
# Some of them need further cleanup as well (e.g. we shouldn't bother with
# supporting NDD directly.)
#
CPPFLAGS += -U_USE_MTU_UPDATE

CFLAGS += $(CCVERBOSE)

#
# Driver depends on GLDv3 (mac)
#
LDFLAGS		+= -N misc/mac

# needs work
$(OBJS_DIR)/sfxge_ev.o := SMOFF += index_overflow
SMOFF += all_func_returns

#
# Default build targets.
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
