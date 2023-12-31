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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright (c) 2011 Bayard G. Bell. All rights reserved.
#
# Copyright (c) 2018, Joyent, Inc.

#	This makefile drives the production of the gl_kmech_krb5 kernel module.
#
#	intel implementation architecture dependent
#
#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= kmech_krb5
OBJECTS		= $(KGSS_KRB5_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_KGSS_DIR)/$(MODULE)

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
# Defined kgssapi and md5 as depdencies
#
LDFLAGS += -N misc/kgssapi -N misc/md5

#
# For now, disable these warnings; maintainers should endeavor
# to investigate and remove these for maximum coverage.
# Please do not carry these forward to new Makefiles.
#

CERRWARN	+= -_gcc=-Wno-unused-function
CERRWARN	+= -_gcc=-Wno-unused-label
CERRWARN	+= $(CNOWARN_UNINIT)
CERRWARN	+= -_gcc=-Wno-parentheses

# needs work
SMOFF += indenting

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:        $(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

KMECHKRB5_BASE=$(UTSBASE)/common/gssapi/mechs/krb5
KGSSDFLAGS=-I $(UTSBASE)/common/gssapi/include
KGSSDFLAGS += $(KRB5_DEFS)

$(OBJS_DIR)/%.o:		$(KMECHKRB5_BASE)/crypto/crc32/%.c
	$(COMPILE.c) $(KGSSDFLAGS) -o $@ $<
	$(CTFCONVERT_O)

#
#	Include common targets.
#
include $(UTSBASE)/intel/Makefile.targ

INC_PATH += \
	-I$(UTSBASE)/common/gssapi \
	-I$(UTSBASE)/common/gssapi/include \
	-I$(UTSBASE)/common/gssapi/mechs/krb5/include \
	-I$(SRC)/lib/gss_mechs/mech_krb5/include \
	-I$(SRC)/lib/gss_mechs/mech_krb5/krb5/krb
