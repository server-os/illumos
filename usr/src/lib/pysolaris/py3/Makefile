# CDDL HEADER START
#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source. A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#
# CDDL HEADER END

# Copyright 2018 OmniOS Community Edition (OmniOSce) Association.

include ../Makefile.com
include ../../Makefile.lib.64

PYVER = $(PYTHON3_VERSION)
PYSUFFIX= $(PYTHON3_SUFFIX)

LDLIBS64 += -L$(ADJUNCT_PROTO)/usr/lib/$(MACH64)
MAPFILES= $(SRCDIR)/mapfile-py3
ROOTPYSOLFILES=	$(PYSRCS:%=$(ROOTLIBDIR)/%)
$(ROOTLIBDIR)/$(DYNLIB) := FILEMODE= 755

all: $(LIBS)
install: all $(ROOTLIBS) $(ROOTPYSOLFILES)

$(ROOTLIBDIR)/%.so: %.so
	$(INS.file)

$(ROOTLIBDIR)/%: ../common/%
	$(INS.pyfile)
	$(PYTHON3) -mpy_compile $@

