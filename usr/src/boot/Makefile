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
# Copyright 2015 Toomas Soome <tsoome@me.com>
#

.KEEP_STATE:

include $(SRC)/Makefile.master

INSTDIRS = i386 efi forth
SUBDIRS = libsa libficl $(INSTDIRS)

all	:=	TARGET = all
clean	:=	TARGET = clean
clobber	:=	TARGET = clobber
install	:=	TARGET = install

all clean clobber: $(SUBDIRS)

#
# The directories in INSTDIRS depend implicitly on SUBDIRS being built already.
# We use .WAIT instead of explicit dependencies because we only want to make
# the "install" target in INSTDIRS, not in SUBDIRS.
#
# If adding SUBDIRS which are not dependencies of INSTDIRS, "install: all"
# rules should be added to the Makefiles in those directories.
#
install: all .WAIT $(INSTDIRS)

.PARALLEL: libsa libficl

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
