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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright 2018 Joyent, Inc.
#

#
#	This makefile drives the production of the Chelsio N110
#	10G Ethernet (CH) driver module in x86 systems
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE   = ../..

#
#	Define the module and object file sets.
#
MODULE		= chxge
OBJECTS		= $(CH_OBJS:%=$(OBJS_DIR)/%) $(CH_COM_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/sparc/Makefile.sparc

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

CFLAGS		+= -DC99_NOT_SUPPORTED -DCONFIG_CHELSIO_T1_1G \
		-I$(UTSBASE)/common/io/chxge/ \
		-I$(UTSBASE)/common/io/chxge/com

CFLAGS		+= -DCONFIG_SMP

CFLAGS		+= -DRX_MAX_COUNT=256

CFLAGS		+= -DCONFIG_CHELSIO_T1_OFFLOAD

CFLAGS		+= -DSUN_KSTATS -DHOST_PAUSE -DTX_CKSUM_FIX -DTX_THREAD_RECLAIM

#	Debug flags
# CFLAGS		+= -DCH_DEBUG=1 -DPE_DBGOUT_ENABLED=1

#
#	Driver depends on GLD, IP, and MAC
#
LDFLAGS		+= -N misc/gld -N drv/ip -N misc/mac

#
# For now, disable these warnings; maintainers should endeavor
# to investigate and remove these for maximum coverage.
# Please do not carry these forward to new Makefiles.
#

CERRWARN	+= -_gcc=-Wno-unused-label
CERRWARN	+= -_gcc=-Wno-unused-function
CERRWARN	+= -_gcc=-Wno-unused-variable
CERRWARN	+= -_gcc=-Wno-parentheses

#
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
include $(UTSBASE)/sparc/Makefile.targ
