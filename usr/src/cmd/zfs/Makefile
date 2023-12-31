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
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright 2010 Nexenta Systems, Inc. All rights reserved.
# Copyright (c) 2012, 2015 by Delphix. All rights reserved.
# Copyright 2016 Igor Kozhukhov <ikozhukhov@gmail.com>.
# Copyright 2020 Joyent, Inc.
#

PROG=		zfs
OBJS=		zfs_main.o zfs_iter.o zfs_project.o
POFILES=	zfs_main.po zfs_iter.po zfs_project.po
POFILE=		zfs.po

include ../Makefile.cmd
include ../Makefile.cmd.64
include ../Makefile.ctf

FSTYPE=         zfs
LINKPROGS=	mount umount
ROOTETCFSTYPE=  $(ROOTETC)/fs/$(FSTYPE)
USRLIBFSTYPE=	$(ROOTLIB)/fs/$(FSTYPE)

LDLIBS += -lzfs_core -lzfs -luutil -lumem -lnvpair -lsec -lidmap -lzutil
# cmdutils has list(9F) functions used by the project code.
LDLIBS += -lcmdutils

INCS += -I../../common/zfs
INCS += -I$(SRC)/uts/common/fs/zfs
INCS += -I../../lib/libzutil/common

CSTD=	$(CSTD_GNU99)

CPPFLAGS += -D_REENTRANT $(INCS)
$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG

ROOTUSRSBINLINKS = $(PROG:%=$(ROOTUSRSBIN)/%)
USRLIBFSTYPELINKS = $(LINKPROGS:%=$(USRLIBFSTYPE)/%)
ROOTETCFSTYPELINKS = $(LINKPROGS:%=$(ROOTETCFSTYPE)/%)

.KEEP_STATE:

.PARALLEL:

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) -o $@ $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTSBINPROG) $(ROOTUSRSBINLINKS) $(USRLIBFSTYPELINKS) \
	$(ROOTETCFSTYPELINKS)

$(POFILE): $(POFILES)
	$(RM) $@
	cat $(POFILES) > $@

clean:
	$(RM) $(OBJS)

# Links from /usr/sbin to /sbin
$(ROOTUSRSBINLINKS):
	-$(RM) $@; $(SYMLINK) ../../sbin/$(PROG) $@

# Links from /usr/lib/fs/zfs to /sbin
$(USRLIBFSTYPELINKS):
	-$(RM) $@; $(SYMLINK) ../../../../sbin/$(PROG) $@

# Links from /etc/fs/zfs to /sbin
$(ROOTETCFSTYPELINKS):
	-$(RM) $@; $(SYMLINK) ../../../sbin/$(PROG) $@

FRC:

include ../Makefile.targ
