/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright 1987, 1988 by MIT Student Information Processing Board
 *
 * For copyright information, see copyright.h.
 */
#include "copyright.h"
#include "ss_internal.h"
#include <libintl.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>

#ifdef lint     /* "lint returns a value which is sometimes ignored" */
#define DONT_USE(x)     x=x;
#else /* !lint */
#define DONT_USE(x)     ;
#endif /* lint */

extern int ss_pager_create();

static char const twentyfive_spaces[26] =
    "                         ";
static char const NL[2] = "\n";

void
ss_list_requests(argc, argv, sci_idx, info_ptr)
    int argc;
    const char * const *argv;
    int sci_idx;
#ifdef __STDC__
    void *info_ptr;
#else
    char *info_ptr;
#endif
{
    register ss_request_entry *entry;
    register char const * const *name;
    register int spacing;
    register ss_request_table **table;

    char buffer[BUFSIZ];
    FILE *output;
    int fd;
#ifdef POSIX_SIGNALS
    struct sigaction nsig, osig;
    sigset_t nmask, omask;
#else
    int mask;
    RETSIGTYPE (*func)();
#endif
#ifndef WAIT_USES_INT
    union wait waitb;
#else
    int waitb;
#endif

    DONT_USE(argc);
    DONT_USE(argv);

#ifdef POSIX_SIGNALS
    sigemptyset(&nmask);
    sigaddset(&nmask, SIGINT);
    sigprocmask(SIG_BLOCK, &nmask, &omask);

    nsig.sa_handler = SIG_IGN;
    sigemptyset(&nsig.sa_mask);
    nsig.sa_flags = 0;
    sigaction(SIGINT, &nsig, &osig);
#else
    mask = sigblock(sigmask(SIGINT));
    func = signal(SIGINT, SIG_IGN);
#endif

    fd = ss_pager_create();
    output = fdopen(fd, "w");

#ifdef POSIX_SIGNALS
    sigprocmask(SIG_SETMASK, &omask, (sigset_t *)0);
#else
    sigsetmask(mask);
#endif

    fprintf (output, dgettext(TEXT_DOMAIN, "Available %s requests:\n\n"),
	     ss_info (sci_idx) -> subsystem_name);

    for (table = ss_info(sci_idx)->rqt_tables; *table; table++) {
        entry = (*table)->requests;
        for (; entry->command_names; entry++) {
            spacing = -2;
            buffer[0] = '\0';
            if (entry->flags & SS_OPT_DONT_LIST)
                continue;
            buffer[sizeof(buffer) - 1] = '\0';
            for (name = entry->command_names; *name; name++) {
                register int len = strlen(*name);
                strncat(buffer, *name, sizeof(buffer) - 1 - strlen(buffer));
                spacing += len + 2;
                if (name[1]) {
                    strncat(buffer, ", ", sizeof(buffer) - 1 - strlen(buffer));
                }
            }
            if (spacing > 23) {
                strncat(buffer, NL, sizeof(buffer) - 1 - strlen(buffer));
                fputs(buffer, output);
                spacing = 0;
                buffer[0] = '\0';
            }
            strncat(buffer, twentyfive_spaces, strlen(twentyfive_spaces) - spacing);

            /*
             * Due to libss not knowing what TEXT_DOMAIN
             * the calling application is using for its info_string
             * messages, we know require the callers (ktutil,kadmin)
             * to L10N the messages before calling libss.
             */
            strncat(buffer, entry->info_string, sizeof(buffer) -1 - strlen(buffer));
            strncat(buffer, NL,  sizeof(buffer) - 1 - strlen(buffer));
            fputs(buffer, output);
        }
    }
    fclose(output);
#ifndef NO_FORK
    wait(&waitb);
#endif
#ifdef POSIX_SIGNALS
    sigaction(SIGINT, &osig, (struct sigaction *)0);
#else
    (void) signal(SIGINT, func);
#endif
}
