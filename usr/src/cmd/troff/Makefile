#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
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
# Copyright (c) 1989 by Sun Microsystems, Inc.
#

include ../Makefile.cmd

SUBDIRS =	nroff.d troff.d


all :=		TARGET = all
install :=	TARGET = install
clean :=	TARGET = clean
clobber :=	TARGET = clobber
lint :=		TARGET = lint
strip :=	TARGET = strip
_msg :=         TARGET = catalog

#
# for message catalog
#
POFILES= $(SUBDIRS:%=%/%.po)
POFILE= troff.po


.KEEP_STATE:

all:		$(SUBDIRS)

install clean lint strip:	$(SUBDIRS)

clobber: $(SUBDIRS) local_clobber

local_clobber:
	$(RM)	$(CLOBBERFILES)

$(POFILE):      $(SUBDIRS) $(POFILES2)
	cat     $(POFILES) $(POFILES2)  >       $@

_msg:   $(MSGDOMAIN) $(SUBDIRS)
	  $(RM)	$(POFILE)
	  cat    $(POFILES)      > $(POFILE)
	  $(RM) $(MSGDOMAIN)/$(POFILE)
	  cp $(POFILE) $(MSGDOMAIN)

$(SUBDIRS):	FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:

include $(SRC)/Makefile.msg.targ
