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
# Copyright 2021 Joyent, Inc.
#

SUBDIRS = libvarpd direct files svp

all :=		TARGET = all
clean :=	TARGET = clean
clobber :=	TARGET = clobber
check :=	TARGET = check
install :=	TARGET = install
install_h :=	TARGET = install_h

.KEEP_STATE:

all clean clobber install install_h check: $(SUBDIRS)
direct files svp: libvarpd

$(SUBDIRS):  FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
