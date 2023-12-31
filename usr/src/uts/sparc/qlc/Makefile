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
# This makefile drives the production of qlc driver kernel module.
#
# Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= qlc
OBJECTS		= $(QLC_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
CONF_SRCDIR	= $(UTSBASE)/common/io/fibre-channel/fca/qlc

#
#	Include common rules.
#
include $(UTSBASE)/Makefile.uts
include $(UTSBASE)/sparc/Makefile.sparc

#
#	Define targets
#
ALL_TARGET	= $(BINARY) $(CONFMOD) $(ITUMOD)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE) $(ROOT_CONFFILE)

#
#	header file directories
#
INC_PATH	+= -I$(ROOT)/usr/include
INC_PATH	+= -I$(UTSBASE)/common/sys/fibre-channel
INC_PATH	+= -I$(UTSBASE)/common/sys/fibre-channel/ulp
INC_PATH	+= -I$(UTSBASE)/common/sys/fibre-channel/fca/qlc
INC_PATH	+= -I$(UTSBASE)/common/sys/fibre-channel/impl

LDFLAGS		+= -Nmisc/fctl

FWIMAGES         = 2200
FWIMAGES        += 2300
FWIMAGES        += 2400
FWIMAGES        += 2500
FWIMAGES        += 6322
FWIMAGES        += 8100

FWMODULES        = $(FWIMAGES:%=$(MODULE)_fw_%)
FWMODULES_SRC    = $(FWIMAGES:%=$(CONF_SRCDIR)/ql_fw_%.c)

CERRWARN	+= $(CNOWARN_UNINIT)
CERRWARN	+= -_gcc=-Wno-type-limits
CERRWARN	+= -_gcc=-Wno-parentheses

#
#	Default build targets.
#
.KEEP_STATE:

all:		$(ALL_DEPS)

def:		$(DEF_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/sparc/Makefile.targ
