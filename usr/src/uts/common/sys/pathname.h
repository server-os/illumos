/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved	*/

/*
 * Portions of this source code were derived from Berkeley 4.3 BSD
 * under license from the Regents of the University of California.
 */

#ifndef _SYS_PATHNAME_H
#define	_SYS_PATHNAME_H

#include <sys/vnode.h>
#include <sys/cred.h>
#include <sys/uio.h>
#include <sys/dirent.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Pathname structure.
 * System calls that operate on path names gather the path name
 * from the system call into this structure and reduce it by
 * peeling off translated components.  If a symbolic link is
 * encountered the new path name to be translated is also
 * assembled in this structure.
 *
 * By convention pn_buf is not changed once it's been set to point
 * to the underlying storage; routines which manipulate the pathname
 * do so by changing pn_path and pn_pathlen.  pn_pathlen is redundant
 * since the path name is null-terminated, but is provided to make
 * some computations faster.
 */
typedef struct pathname {
	char	*pn_buf;		/* underlying storage */
	char	*pn_path;		/* remaining pathname */
	size_t	pn_pathlen;		/* remaining length */
	size_t	pn_bufsize;		/* total size of pn_buf */
} pathname_t;

#define	pn_pathleft(pnp)	((pnp)->pn_pathlen)

extern void	pn_alloc(struct pathname *);
extern void	pn_alloc_sz(struct pathname *, size_t);
extern int	pn_get(const char *, enum uio_seg, struct pathname *);
extern int	pn_get_buf(const char *, enum uio_seg, struct pathname *,
			void *, size_t);
extern int	pn_set(struct pathname *, const char *);
extern int	pn_insert(struct pathname *, struct pathname *, size_t);
extern int	pn_getsymlink(vnode_t *, struct pathname *, cred_t *);
extern int	pn_getcomponent(struct pathname *, char *);
extern void	pn_setlast(struct pathname *);
extern void	pn_skipslash(struct pathname *);
extern int	pn_fixslash(struct pathname *);
extern int	pn_addslash(struct pathname *);
extern void	pn_free(struct pathname *);

extern int lookupname(const char *, enum uio_seg, int follow,
		vnode_t **, vnode_t **);
extern int lookupnameat(const char *, enum uio_seg, int follow,
		vnode_t **, vnode_t **, vnode_t *);
extern int lookupnameatcred(const char *, enum uio_seg, int follow,
		vnode_t **, vnode_t **, vnode_t *, cred_t *);
extern int lookuppn(struct pathname *, struct pathname *, int follow,
		vnode_t **, vnode_t **);
extern int lookuppnat(struct pathname *, struct pathname *, int follow,
		vnode_t **, vnode_t **, vnode_t *);
extern int lookuppnatcred(struct pathname *, struct pathname *, int follow,
		vnode_t **, vnode_t **, vnode_t *, cred_t *);

extern int lookuppnvp(struct pathname *, struct pathname *, int follow,
		vnode_t **, vnode_t **, vnode_t *, vnode_t *, cred_t *);
extern int traverse(vnode_t **);

extern int vnodetopath(vnode_t *, vnode_t *, char *, size_t, cred_t *);
extern int dogetcwd(char *, size_t);
extern int dirfindvp(vnode_t *, vnode_t *, vnode_t *, cred_t *, char *,
		size_t, dirent64_t **);

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_PATHNAME_H */
