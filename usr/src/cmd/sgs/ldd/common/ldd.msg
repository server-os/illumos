#
# Copyright (c) 1996, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2022 Garrett D'Amore <garrett@damore.org>
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


@ _START_

# Message file for cmd/sgs/ldd.

@ MSG_ID_LDD


# Argument usage messages.

@ MSG_ARG_USAGE		"usage: %s [-d | -r] [-c] [-D] [-e envar] [-f] [-i] \
			 [-L] [-l] [-p] [-s] [-U | -u] [-v] [-w] file(s)\n"

# Environment messages.

@ MSG_ENV_FAILED	"%s: can't add to environment, putenv(3C) failed\n"


# Unsupported file messages.

@ MSG_USP_NODYNORSO	"%s: %s: file is not a dynamic executable or \
			 shared object\n"
@ MSG_USP_NOHDR		"%s: %s: file built with ?N (NOHDR) mapfile option\n"
@ MSG_USP_UNKNOWN	"%s: %s: unsupported or unknown file type\n"
@ MSG_USP_ELFINS	"%s: %s: file has insecure interpreter %s\n"
@ MSG_USP_NOTEXEC	"%s: %s: is not executable\n"


# ELF messages

@ MSG_ELF_LIBELF	"%s: libelf: version not supported: %d"
@ MSG_ELF_BADMAGIC	"%s: %s: bad magic number\n"
@ MSG_ELF_CLASS		"%s: %s: ELF class: %s: unrecognized\n"
@ MSG_ELF_KCLASS32	"%s: %s: ELF class: %s: is incompatible with 32-bit \
			 system\n"
@ MSG_ELF_MACHTYPE	"%s: %s: ELF machine type: %s: is incompatible with \
			 system\n"
@ MSG_ELF_DATA		"%s: %s: ELF byte order: %s: is incompatible with \
			 system\n"
@ MSG_ELF_MACHFLAGS	"%s: %s: wrong machine flags\n"

@ MSG_ELF_GETEHDR	"%s: %s: elf_getehdr: %s\n"
@ MSG_ELF_GETPHDR	"%s: %s: elf_getphdr: %s\n"


# System error messages

@ MSG_SYS_FORK		"%s: cannot fork: %s\n"
@ MSG_SYS_OPEN		"%s: %s: cannot open file: %s\n"

@ MSG_SYS_MALLOC	"%s: malloc failed\n"
@ MSG_SYS_EXEC		"%s: %s: execution failed "
@ MSG_SYS_EXEC_SIG	"due to signal %d %s\n"
@ MSG_SYS_EXEC_CORE	"(core dumped)"
@ MSG_SYS_EXEC_STAT	"with exit status %d\n"

@ _END_


# The following strings represent reserved words, files, pathnames and symbols.
# Reference to this strings is via the MSG_ORIG() macro, and thus no message
# translation is required.

@ MSG_STR_EMPTY		""

@ MSG_PTH_LIB		"/lib"
@ MSG_PTH_USRLIB	"/usr/lib"
@ MSG_PTH_ETCLIB	"/etc/lib"

@ MSG_LD_PRELOAD_32	"LD_PRELOAD_32"
@ MSG_LD_PRELOAD_64	"LD_PRELOAD_64"
@ MSG_LD_PRELOAD	"LD_PRELOAD"

@ MSG_SUNW_OST_SGS	"SUNW_OST_SGS"

@ MSG_STR_GETOPT	"cDde:fiLlprsUuvw"
@ MSG_STR_FMT1		"%s=./%s %s"
@ MSG_STR_FMT2		"%s=%s %s"
@ MSG_STR_FMT3		"%s:\n"
