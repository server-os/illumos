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
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2017 Jason King
# Copyright (c) 2019, Joyent, Inc.
#

LIBRARY =	libsoftcrypto.a
VERS =		.1

include		../Makefile.com

AES_PSM_OBJS =	aes_amd64.o aes_intel.o aeskey.o
ARCFOUR_PSM_OBJS = arcfour-x86_64.o
BIGNUM_PSM_OBJS = bignum_amd64.o bignum_amd64_asm.o
MODES_PSM_OBJS = gcm_intel.o

include		$(SRC)/lib/Makefile.lib
include		$(SRC)/lib/Makefile.lib.64

CERRWARN +=	-_gcc=-Wno-type-limits
CERRWARN +=	-_gcc=-Wno-parentheses
CERRWARN +=	$(CNOWARN_UNINIT)

# not linted
SMATCH=off

AES_PSM_SRC =	$(AES_DIR)/$(MACH64)/aes_amd64.S \
		$(AES_DIR)/$(MACH64)/aes_intel.S \
		$(AES_DIR)/$(MACH64)/aeskey.c
ARCFOUR_PSM_SRC = arcfour-x86_64.S
BIGNUM_PSM_SRC = $(BIGNUM_DIR)/$(MACH64)/bignum_amd64.c \
		$(BIGNUM_DIR)/$(MACH64)/bignum_amd64_asm.S

MODES_PSM_SRC = $(MODES_DIR)/$(MACH64)/gcm_intel.S

# Sources need to be redefined after Makefile.lib inclusion.
SRCS =		$(AES_SRC) $(ARCFOUR_SRC) $(BIGNUM_SRC) $(BLOWFISH_SRC) \
		$(DES_SRC) $(MODES_SRC) $(DH_SRC) $(DSA_SRC) $(RSA_SRC) \
		$(PAD_SRC)

SRCDIR =	$(SRC)/lib/pkcs11/libsoftcrypto/common

LIBS =		$(DYNLIB)
MAPFILEDIR =	../common

CFLAGS +=	-xO4
CPPFLAGS +=	-I$(CRYPTODIR) -I$(UTSDIR) -D_POSIX_PTHREAD_SEMANTICS
ASFLAGS64 +=	$(AS_PICFLAGS) -D_ASM
BIGNUM_FLAGS +=	-DPSR_MUL
CLEANFILES +=	arcfour-x86_64.S arcfour-x86_64.s
LDLIBS +=	-lcryptoutil -lc

.KEEP_STATE:

all:		$(LIBS)

install:	all $(ROOTLIBS64) $(ROOTLINKS64)

pics/%.o: $(AES_DIR)/$(MACH64)/%.c
		$(COMPILE.c) $(AES_FLAGS) -o $@ $<
		$(POST_PROCESS_O)

pics/%.o: $(AES_DIR)/$(MACH64)/%.S
		$(COMPILE.s) $(AES_FLAGS) -o $@ $<
		$(POST_PROCESS_S_O)

pics/%.o: $(BIGNUM_DIR)/$(MACH64)/%.c
		$(COMPILE.c) $(BIGNUM_FLAGS) -o $@ $<
		$(POST_PROCESS_O)

pics/%.o: $(BIGNUM_DIR)/$(MACH64)/%.S
		$(COMPILE64.s) $(BIGNUM_FLAGS) -o $@ $<
		$(POST_PROCESS_S_O)

pics/%.o: $(MODES_DIR)/$(MACH64)/%.S
		$(COMPILE64.s) $(MODES_FLAGS) -o $@ $<
		$(POST_PROCESS_S_O)

include		../Makefile.targ

arcfour-x86_64.S: $(ARCFOUR_DIR)/amd64/arcfour-x86_64.pl
		$(PERL) $? $@

pics/%.o: arcfour-x86_64.S
		$(COMPILE64.s) $(ARCFOUR_FLAGS) -o $@ $<
		$(POST_PROCESS_S_O)
