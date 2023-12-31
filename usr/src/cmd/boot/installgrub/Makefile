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
# Copyright (c) 2005, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2015, Nexenta Systems, Inc.
# Copyright (c) 2018, Joyent, Inc.

PROG= installgrub

PCFS_SRC= $(SRC)/common/fs/pcfs.c
EINFO_SRC= ./../common/bblk_einfo.c
UTILS_SRC =./../common/boot_utils.c
EXTRA_SRC =./../common/mboot_extra.c

OBJS= installgrub.o pcfs_glue.o pcfs.o bblk_einfo.o boot_utils.o mboot_extra.o \
	getresponse.o
SBINLINKS= $(PROG)

include ../Makefile.com

CPPFLAGS += -I$(SRC)/uts/i86pc -I$(SRC)/uts/intel -I$(SRC)/uts/common
CPPFLAGS += -I$(SRC)/common/util

# not linted
SMATCH=off

LDLIBS += -lmd5

i386_CFLAGS += -D_LARGEFILE64_SOURCE
i386_CFLAGS += -D_FILE_OFFSET_BITS=64

LDLIBS += -lfdisk -lefi -lfstyp

CSTD=	$(CSTD_GNU99)

.KEEP_STATE:

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) -o $@ $(OBJS) $(LDLIBS)
	$(POST_PROCESS)


# pick pcfs from usr/src/common/fs
pcfs.o:	$(PCFS_SRC)
	$(COMPILE.c) -o $@ $(PCFS_SRC)

boot_utils.o:	$(UTILS_SRC)
		$(COMPILE.c) -o $@ $(UTILS_SRC)

mboot_extra.o:	$(EXTRA_SRC)
		$(COMPILE.c) -o $@ $(EXTRA_SRC)

bblk_einfo.o:	$(EINFO_SRC)
		$(COMPILE.c) -o $@ $(EINFO_SRC)

%.o:	$(SRC)/common/util/%.c
		$(COMPILE.c) $(OUTPUT_OPTION) $<

install: all $(ROOTSBINPROG) .WAIT $(ROOTUSRSBINLINKS)

clean:
	$(RM) $(OBJS)

include ../Makefile.targ
