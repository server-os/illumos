/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/ioctl.h>
#include "ipf.h"
#include "netinet/ipl.h"

#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf


ip_pool_t *printpool_live(pool, fd, name, opts)
ip_pool_t *pool;
int fd;
char *name;
int opts;
{
	ip_pool_node_t entry, *top, *node;
	ipflookupiter_t iter;
	int i, printed = 0, last;
	ipfobj_t obj;

	if ((name != NULL) && strncmp(name, pool->ipo_name, FR_GROUPLEN))
		return pool->ipo_next;

	printpooldata(pool, opts);

	if ((opts & OPT_DEBUG) == 0)
		PRINTF("\t{");

	obj.ipfo_rev = IPFILTER_VERSION;
	obj.ipfo_type = IPFOBJ_LOOKUPITER;
	obj.ipfo_ptr = &iter;
	obj.ipfo_size = sizeof(iter);

	iter.ili_data = &entry;
	iter.ili_type = IPLT_POOL;
	iter.ili_otype = IPFLOOKUPITER_NODE;
	iter.ili_ival = IPFGENITER_LOOKUP;
	iter.ili_unit = pool->ipo_unit;
	strncpy(iter.ili_name, pool->ipo_name, FR_GROUPLEN);

	last = 0;
	top = NULL;

	while (!last && (ioctl(fd, SIOCLOOKUPITER, &obj) == 0)) {
		if (entry.ipn_next == NULL)
			last = 1;
		entry.ipn_next = top;
		top = malloc(sizeof(*top));
		if (top == NULL)
			break;
		bcopy(&entry, top, sizeof(entry));
	}

	while (top != NULL) {
		node = top;
		(void) printpoolnode(node, opts);
		top = node->ipn_next;
		free(node);
		printed++;

		if ((opts & OPT_DEBUG) == 0)
			putchar(';');
	}

	if (printed == 0)
		putchar(';');

	if ((opts & OPT_DEBUG) == 0)
		PRINTF(" };\n");
	return pool->ipo_next;
}
