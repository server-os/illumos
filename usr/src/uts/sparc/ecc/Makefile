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
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
#
#	This makefile drives the production of the ECC KEF provider.
#
#	sparc implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..
COM1_DIR = $(COMMONBASE)/mpi
COM2_DIR = $(COMMONBASE)/crypto

#
#	Define the module and object file sets.
#
MODULE		= ecc
OBJECTS		= $(ECCPROV_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_CRYPTO_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/sparc/Makefile.sparc

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

#
# Linkage dependencies
#
LDFLAGS += -Nmisc/kcf

CFLAGS		+= -I$(COM1_DIR) -I$(COM2_DIR)
CFLAGS		+= -DMP_API_COMPATIBLE -DNSS_ECC_MORE_THAN_SUITE_B

#
# For now, disable these warnings; maintainers should endeavor
# to investigate and remove these for maximum coverage.
# Please do not carry these forward to new Makefiles.
#
CFLAGS		+= $(CCVERBOSE)
CERRWARN	+= -_gcc=-Wno-switch
CERRWARN	+= -_gcc=-Wno-unused-label
CERRWARN	+= $(CNOWARN_UNINIT)
CERRWARN	+= -_gcc=-Wno-type-limits
CERRWARN	+= -_gcc=-Wno-empty-body
CERRWARN	+= -_gcc=-Wno-unused-variable
CERRWARN	+= -_gcc=-Wno-parentheses

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
