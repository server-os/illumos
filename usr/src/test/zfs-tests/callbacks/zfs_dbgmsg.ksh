#!/usr/bin/ksh -p

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
# Copyright (c) 2016 by Delphix. All rights reserved.
#

# $1: number of lines to output (default: 200)
typeset lines=${1:-200}

echo "================================================================="
echo " Tailing last $lines lines of zfs_dbgmsg log"
echo "================================================================="

id=$(id -u)

if (( id == 0 )); then
	mdb -ke "::zfs_dbgmsg ! tail -n $lines"
else
	sudo mdb -ke "::zfs_dbgmsg ! tail -n $lines"
fi

echo "================================================================="
echo " End of zfs_dbgmsg log"
echo "================================================================="
