#ifndef MANDOC_CONFIG_H
#define MANDOC_CONFIG_H

#include <sys/types.h>

/*
 * The tools build may be on a system without O_DIRECTORY. So we need to
 * explicitly include sys/fcntl.h and check for O_DIRECTORY and if not present,
 * use the default of it being zero.
 */
#include <sys/fcntl.h>
#ifndef	O_DIRECTORY
#define O_DIRECTORY 0
#endif

#ifdef __cplusplus
#error "Do not use C++.  See the INSTALL file."
#endif


#define MAN_CONF_FILE "/etc/man.conf"
#define MANPATH_BASE "/usr/share/man"
#define MANPATH_DEFAULT "/usr/share/man"
#define OSENUM MANDOC_OS_OTHER
#define UTF8_LOCALE "en_US.UTF-8"
#define EFTYPE EINVAL

#define HAVE_DIRENT_NAMLEN 0
#define HAVE_ENDIAN 1
#define HAVE_ERR 1
#define HAVE_FTS 1
#define HAVE_FTS_COMPARE_CONST 0
#define HAVE_GETLINE 1
#define HAVE_GETSUBOPT 1
#define HAVE_ISBLANK 1
#define HAVE_LESS_T 1
#define HAVE_MKDTEMP 1
#define HAVE_MKSTEMPS 1
#define HAVE_NTOHL 1
#define HAVE_PLEDGE 0
#define HAVE_PROGNAME 1
#define HAVE_REALLOCARRAY 1
#define HAVE_RECALLOCARRAY 1
#define HAVE_REWB_BSD 1
#define HAVE_REWB_SYSV 1
#define HAVE_SANDBOX_INIT 0
#define HAVE_STRCASESTR 1
#define HAVE_STRINGLIST 0
#define HAVE_STRLCAT 1
#define HAVE_STRLCPY 1
#define HAVE_STRNDUP 1
#define HAVE_STRPTIME 1
#define HAVE_STRSEP 1
#define HAVE_STRTONUM 1
#define HAVE_SYS_ENDIAN 0
#define HAVE_VASPRINTF 1
#define HAVE_WCHAR 1
#define HAVE_OHASH 0
#define NEED_XPG4_2 1

#define BINM_APROPOS "apropos"
#define BINM_MAKEWHATIS "man -w"
#define BINM_CATMAN "catman"
#define BINM_MAN "man"
#define BINM_SOELIM "soelim"
#define BINM_WHATIS "whatis"
#define BINM_PAGER "less"

#endif /* MANDOC_CONFIG_H */
