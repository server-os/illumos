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
# Copyright 2018 Joyent, Inc.
#

MODULE = i40e.so
MDBTGT = kvm

MODSRCS = i40e.c

include ../../../../../Makefile.cmd
include ../../../../../Makefile.cmd.64
include ../../../Makefile.amd64
include ../../../../Makefile.module

CPPFLAGS += -I$(SRC)/cmd/mdb/common
CPPFLAGS += -I$(SRC)/uts/common/io/i40e
CPPFLAGS += -I$(SRC)/uts/common/io/i40e/core
CPPFLAGS += -I$(SRC)/uts/common
CPPFLAGS += -D_I40E_MDB_DMOD
