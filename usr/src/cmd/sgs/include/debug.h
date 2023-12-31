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
 * Copyright (c) 1991, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2022 Oxide Computer Company
 */

#ifndef	_DEBUG_H
#define	_DEBUG_H

/*
 * Global include file for lddbg debugging.
 *
 * ld(1) and ld.so.1(1) carry out all diagnostic debugging calls via lazy
 * loading the library liblddbg.so.  Thus debugging is always enabled.  The
 * utility elfdump(1) is explicitly dependent upon this library.  There are two
 * categories of routines defined in this library:
 *
 *  o	Debugging routines that have specific linker knowledge, and test the
 *	class of debugging allowable before proceeding, start with the `Dbg_'
 *	prefix.
 *
 *  o	Lower level routines that provide generic ELF structure interpretation
 *	start with the `Elf_' prefix.  These latter routines are the only
 *	routines used by the elfdump(1) utility.
 */
#include <sys/times.h>
#include <sgs.h>
#include <libld.h>
#include <rtld.h>
#include <gelf.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Define Dbg_*() interface values.  These values direct the debugging routine
 * to generate different diagnostics, thus the strings themselves are maintained
 * in the debugging library.
 */
#define	DBG_SUP_ENVIRON		1
#define	DBG_SUP_CMDLINE		2

#define	DBG_CONF_IGNORE		1	/* configuration processing errors */
#define	DBG_CONF_VERSION	2
#define	DBG_CONF_PRCFAIL	3
#define	DBG_CONF_CORRUPT	4
#define	DBG_CONF_ABIMISMATCH	5

#define	DBG_ORDER_INFO_RANGE	1	/* sh_link out of range */
#define	DBG_ORDER_INFO_ORDER	2	/* sh_info also ordered */
#define	DBG_ORDER_LINK_OUTRANGE	3	/* sh_link out of range */
#define	DBG_ORDER_FLAGS		4	/* sh_flags do not match */
#define	DBG_ORDER_CYCLIC	5	/* sh_link cyclic */
#define	DBG_ORDER_LINK_ERROR	6	/* sh_link (one) has an error */

#define	DBG_INIT_SORT		1	/* calling init from sorted order */
#define	DBG_INIT_PEND		2	/* calling pending init */
#define	DBG_INIT_DYN		3	/* dynamically triggered init */
#define	DBG_INIT_DONE		4	/* init completed */

#define	DBG_DLSYM_DEF		0
#define	DBG_DLSYM_NEXT		1
#define	DBG_DLSYM_DEFAULT	2
#define	DBG_DLSYM_SELF		3
#define	DBG_DLSYM_PROBE		4
#define	DBG_DLSYM_SINGLETON	5
#define	DBG_DLSYM_NUM		DBG_DLSYM_SINGLETON + 1

#define	DBG_DLCLOSE_NULL	0
#define	DBG_DLCLOSE_IGNORE	1
#define	DBG_DLCLOSE_RESCAN	2

#define	DBG_SYM_REDUCE_GLOBAL	1	/* reporting global symbols to local */
#define	DBG_SYM_REDUCE_RETAIN	2	/* reporting non reduced local syms */

#define	DBG_AUD_CALL		1	/* original call to auditor */
#define	DBG_AUD_RET		2	/* return from auditor diagnostic */

#define	DBG_AUD_LOCAL		0	/* auditor is local */
#define	DBG_AUD_GLOBAL		1	/* auditor is global */
#define	DBG_AUD_PRELOAD		2	/* auditor is preloaded */

/*
 * Group handle operations - passed to Dbg_file_hdl_title().  Indicate why
 * handle dependencies are being manipulated.
 */
#define	DBG_HDL_CREATE		0	/* handle creation */
#define	DBG_HDL_ADD		1	/* addition to existing handle */
#define	DBG_HDL_DELETE		2	/* deletion from a handle */
#define	DBG_HDL_ORPHAN		3	/* handle being moved to orphan list */
#define	DBG_HDL_REINST		4	/* handle reinstated from orphan list */

/*
 * Group handle dependency operations - passed to Dbg_file_hdl_action().
 * Identify the dependencies that are associated with a handle.
 */
#define	DBG_DEP_ADD		0	/* dependency added */
#define	DBG_DEP_UPDATE		1	/* dependency updated */
#define	DBG_DEP_DELETE		2	/* dependency deleted */
#define	DBG_DEP_REMOVE		3	/* dependency removed from handle */
#define	DBG_DEP_REMAIN		4	/* dependency must remain on handle */
#define	DBG_DEP_ORPHAN		5	/* dependency must remain an orphan */
#define	DBG_DEP_REINST		6	/* dependency reinstated from orphan */

/*
 * Binding information, indicating the result of a symbol binding.  Can also
 * indicate the reference as being EXTERN or PARENT.  Binding information is
 * used to augment diagnostic binding information (which in turn can be used by
 * lari(1)), and to enable ldd(1) -p processing.
 */
#define	DBG_BINFO_FOUND		0x0001	/* information regarding binding */
#define	DBG_BINFO_DIRECT	0x0002	/* bound directly */
#define	DBG_BINFO_COPYREF	0x0004	/* bound to copy relocated reference */
#define	DBG_BINFO_FILTEE	0x0008	/* bound to filtee */
#define	DBG_BINFO_INTERPOSE	0x0010	/* bound to an identified interposer */
#define	DBG_BINFO_PLTADDR	0x0020	/* bound to executables undefined plt */
#define	DBG_BINFO_MSK		0x0fff

#define	DBG_BINFO_REF_EXTERN	0x1000	/* reference to EXTERN */
#define	DBG_BINFO_REF_PARENT	0x2000	/* reference to PARENT */
#define	DBG_BINFO_REF_MSK	0xf000

/*
 * ld.so.1(1) symbol capabilities processing.
 */
#define	DBG_CAP_DEFAULT		0
#define	DBG_CAP_USED		1
#define	DBG_CAP_CANDIDATE	2
#define	DBG_CAP_REJECTED	3
#define	DBG_CAP_HW_1		4
#define	DBG_CAP_SF_1		5
#define	DBG_CAP_HW_2		6
#define	DBG_CAP_PLAT		7
#define	DBG_CAP_MACH		8
#define	DBG_CAP_HW_3		9

#define	DBG_REL_START		1
#define	DBG_REL_FINISH		2
#define	DBG_REL_NONE		3

#define	DBG_NL_STD		0	/* newline controllers - standard and */
#define	DBG_NL_FRC		2	/*    forced. */

#define	DBG_BNDREJ_DIRECT	0	/* bind rejected, direct to nodirect */
#define	DBG_BNDREJ_GROUP	1	/* bind rejected, group to nodirect */
#define	DBG_BNDREJ_SINGLE	2	/* bind rejected, singleton without */
					/*    default search model */
#define	DBG_BNDREJ_NUM		DBG_BNDREJ_SINGLE

/*
 * Dbg_state_str() is used to obtain commonly used "state transition"
 * strings used in various debugging output.
 */
#define	DBG_STATE_ADD		0	/* add */
#define	DBG_STATE_CURRENT	1	/* current */
#define	DBG_STATE_EXCLUDE	2	/* exclude */
#define	DBG_STATE_IGNORED	3	/* ignored */
#define	DBG_STATE_MOD_BEFORE	4	/* modify (before) */
#define	DBG_STATE_MOD_AFTER	5	/* modify (after) */
#define	DBG_STATE_NEW		6	/* new */
#define	DBG_STATE_NEW_IMPLICIT	7	/* new (implicit) */
#define	DBG_STATE_RESET		8	/* reset */
#define	DBG_STATE_ORIGINAL	9	/* original */
#define	DBG_STATE_RESOLVED	10	/* resolved */

#define	DBG_STATE_NUM		11
typedef uint_t dbg_state_t;
extern	const char *Dbg_state_str(dbg_state_t);

/*
 * Define a debug descriptor, and a user macro that inspects the descriptor as
 * a means of triggering a class of diagnostic output.
 */
typedef struct {
	uint_t		d_class;	/* debugging classes */
	uint_t		d_extra;	/* extra public information */
	APlist		*d_list;	/* accepted link-map list names */
	struct timeval	d_totaltime;	/* total time since entry - */
					/*	gettimeofday(3c) */
	struct timeval	d_deltatime;	/* delta time since last diagnostic - */
					/*	gettimeofday(3c) */
} Dbg_desc;

extern	Dbg_desc	*dbg_desc;

/*
 * Macros used to avoid calls to liblddbg unless debugging is enabled.
 * liblddbg is lazy loaded --- this prevents it from happening unless
 * it will actually be used.
 */
#define	DBG_ENABLED	(dbg_desc->d_class)
#define	DBG_CALL(func)	if (DBG_ENABLED) func

/*
 * Most debugging tokens are interpreted within liblddbg, and thus any flags
 * within d_class are only meaningful to this library.  The following flags
 * extend the d_class diagnostic, and are maintained in d_extra.  These flags
 * may be interpreted by the debugging library itself or from the callers
 * dbg_print() routine.
 */
#define	DBG_E_DETAIL	0x00000001	/* add detail to a class */
#define	DBG_E_LONG	0x00000002	/* use long names (ie. no truncation) */
#define	DBG_E_DEMANGLE	0x00000004	/* demangle symbol names */
#define	DBG_E_STDNL	0x00000008	/* standard newline indicator */
#define	DBG_E_HELP	0x00000010	/* help requested */
#define	DBG_E_HELP_EXIT	0x00000020	/* hint: user should exit after help */
#define	DBG_E_TTIME	0x00000040	/* prepend total time */
#define	DBG_E_DTIME	0x00000080	/* prepend delta time */
#define	DBG_E_RESET	0x00000100	/* reset times */

/* ld only */
#define	DBG_E_SNAME	0x00001000	/* prepend simple name */
#define	DBG_E_FNAME	0x00002000	/* prepend full name */
#define	DBG_E_CLASS	0x00004000	/* prepend ELF class */

/* ld.so.1 only */
#define	DBG_E_LMID	0x00100000	/* prepend link-map id */
#define	DBG_E_LMID_LDSO	0x00200000	/* show ldso link-map list */
#define	DBG_E_LMID_ALL	0x00400000	/* show all non-ldso link-map lists */
#define	DBG_E_LMID_ALT	0x00800000	/* show all ALT link-map lists */
#define	DBG_E_LMID_BASE	0x01000000	/* show BASE link-map list */


#define	DBG_NOTDETAIL()	!(dbg_desc->d_extra & DBG_E_DETAIL)
#define	DBG_NOTLONG()	!(dbg_desc->d_extra & DBG_E_LONG)

#define	DBG_ISDEMANGLE() \
			(dbg_desc->d_extra & DBG_E_DEMANGLE)

#define	DBG_TOTALTIME	(dbg_desc->d_totaltime)
#define	DBG_DELTATIME	(dbg_desc->d_deltatime)

#define	DBG_ISTTIME()	(dbg_desc->d_extra & DBG_E_TTIME)
#define	DBG_ISDTIME()	(dbg_desc->d_extra & DBG_E_DTIME)
#define	DBG_ISTIME()	(dbg_desc->d_extra & (DBG_E_TTIME | DBG_E_DTIME))
#define	DBG_NOTTIME()	!(dbg_desc->d_extra & (DBG_E_TTIME | DBG_E_DTIME))

#define	DBG_ISRESET()	(dbg_desc->d_extra & DBG_E_RESET)
#define	DBG_ONRESET()	(dbg_desc->d_extra |= DBG_E_RESET)
#define	DBG_OFFRESET()	(dbg_desc->d_extra &= ~DBG_E_RESET)

#define	DBG_ISSNAME()	(dbg_desc->d_extra & DBG_E_SNAME)
#define	DBG_ISFNAME()	(dbg_desc->d_extra & DBG_E_FNAME)
#define	DBG_ISCLASS()	(dbg_desc->d_extra & DBG_E_CLASS)
#define	DBG_ISLMID()	(dbg_desc->d_extra & DBG_E_LMID)

/*
 * Print routine, this must be supplied by the application.  The initial
 * argument may provide a link-map list to associate with the format statement
 * that follows.
 */
/* PRINTFLIKE2 */
extern	void		dbg_print(Lm_list *, const char *, ...);

/*
 * Initialization routine, called before any other Dbg routines to
 * establish the necessary state.
 */
typedef enum { DBG_CALLER_LD, DBG_CALLER_RTLD } dbg_setup_caller_t;
extern	int		Dbg_setup(dbg_setup_caller_t, const char *,
			    Dbg_desc *, const char **);

/* Call dbg_print() to produce linker version output */
extern void		Dbg_version(void);

/* Call dbg_print() to produce help output */
extern	void		Dbg_help(void);

/*
 * Establish ELF32 and ELF64 class Dbg_*() interfaces.
 */
#if	defined(_ELF64)

#define	Dbg_demangle_name	Dbg64_demangle_name

#define	Dbg_bind_global		Dbg64_bind_global
#define	Dbg_bind_plt_summary	Dbg64_bind_plt_summary
#define	Dbg_bind_pltpad_from	Dbg64_bind_pltpad_from
#define	Dbg_bind_pltpad_to	Dbg64_bind_pltpad_to
#define	Dbg_bind_reject		Dbg64_bind_reject
#define	Dbg_bind_weak		Dbg64_bind_weak

#define	Dbg_cap_candidate	Dbg64_cap_candidate
#define	Dbg_cap_filter		Dbg64_cap_filter
#define	Dbg_cap_id		Dbg64_cap_id
#define	Dbg_cap_identical	Dbg64_cap_identical
#define	Dbg_cap_mapfile_title	Dbg64_cap_mapfile_title
#define	Dbg_cap_post_title	Dbg64_cap_post_title
#define	Dbg_cap_sec_title	Dbg64_cap_sec_title
#define	Dbg_cap_val		Dbg64_cap_val
#define	Dbg_cap_ptr_entry	Dbg64_cap_ptr_entry
#define	Dbg_cap_val_entry	Dbg64_cap_val_entry

#define	Dbg_dl_dladdr		Dbg64_dl_dladdr
#define	Dbg_dl_dlclose		Dbg64_dl_dlclose
#define	Dbg_dl_dldump		Dbg64_dl_dldump
#define	Dbg_dl_dlerror		Dbg64_dl_dlerror
#define	Dbg_dl_dlinfo		Dbg64_dl_dlinfo
#define	Dbg_dl_dlopen		Dbg64_dl_dlopen
#define	Dbg_dl_dlsym		Dbg64_dl_dlsym
#define	Dbg_dl_iphdr_enter	Dbg64_dl_iphdr_enter
#define	Dbg_dl_iphdr_callback	Dbg64_dl_iphdr_callback
#define	Dbg_dl_iphdr_mapchange	Dbg64_dl_iphdr_mapchange
#define	Dbg_dl_iphdr_unmap_ret	Dbg64_dl_iphdr_unmap_ret

#define	Dbg_ent_entry		Dbg64_ent_entry
#define	Dbg_ent_print		Dbg64_ent_print

#define	Dbg_file_analyze	Dbg64_file_analyze
#define	Dbg_file_ar		Dbg64_file_ar
#define	Dbg_file_ar_rescan	Dbg64_file_ar_rescan
#define	Dbg_file_bind_entry	Dbg64_file_bind_entry
#define	Dbg_file_bindings	Dbg64_file_bindings
#define	Dbg_file_bindings_done	Dbg64_file_bindings_done
#define	Dbg_file_cleanup	Dbg64_file_cleanup
#define	Dbg_file_cntl		Dbg64_file_cntl
#define	Dbg_file_config_dis	Dbg64_file_config_dis
#define	Dbg_file_config_obj	Dbg64_file_config_obj
#define	Dbg_file_del_rescan	Dbg64_file_del_rescan
#define	Dbg_file_delete		Dbg64_file_delete
#define	Dbg_file_deferred	Dbg64_file_deferred
#define	Dbg_file_elf		Dbg64_file_elf
#define	Dbg_file_filtee		Dbg64_file_filtee
#define	Dbg_file_filter		Dbg64_file_filter
#define	Dbg_file_fixname	Dbg64_file_fixname
#define	Dbg_file_generic	Dbg64_file_generic
#define	Dbg_file_hdl_action	Dbg64_file_hdl_action
#define	Dbg_file_hdl_collect	Dbg64_file_hdl_collect
#define	Dbg_file_hdl_title	Dbg64_file_hdl_title
#define	Dbg_file_lazyload	Dbg64_file_lazyload
#define	Dbg_file_ldso		Dbg64_file_ldso
#define	Dbg_file_mmapobj	Dbg64_file_mmapobj
#define	Dbg_file_mode_promote	Dbg64_file_mode_promote
#define	Dbg_file_modified	Dbg64_file_modified
#define	Dbg_file_needed		Dbg64_file_needed
#define	Dbg_file_output		Dbg64_file_output
#define	Dbg_file_preload	Dbg64_file_preload
#define	Dbg_file_prot		Dbg64_file_prot
#define	Dbg_file_rejected	Dbg64_file_rejected
#define	Dbg_file_reuse		Dbg64_file_reuse
#define	Dbg_file_skip		Dbg64_file_skip

#define	Dbg_got_display		Dbg64_got_display

#define	Dbg_libs_audit		Dbg64_libs_audit
#define	Dbg_libs_find		Dbg64_libs_find
#define	Dbg_libs_found		Dbg64_libs_found
#define	Dbg_libs_insecure	Dbg64_libs_insecure
#define	Dbg_libs_init		Dbg64_libs_init
#define	Dbg_libs_l		Dbg64_libs_l
#define	Dbg_libs_path		Dbg64_libs_path
#define	Dbg_libs_req		Dbg64_libs_req
#define	Dbg_libs_update		Dbg64_libs_update
#define	Dbg_libs_yp		Dbg64_libs_yp
#define	Dbg_libs_ylu		Dbg64_libs_ylu

#define	Dbg_map_cexp_id		Dbg64_map_cexp_id
#define	Dbg_map_dv		Dbg64_map_dv
#define	Dbg_map_dv_entry	Dbg64_map_dv_entry
#define	Dbg_map_ent		Dbg64_map_ent
#define	Dbg_map_ent_ord_title	Dbg64_map_ent_ord_title
#define	Dbg_map_hdr_noalloc	Dbg64_map_hdr_noalloc
#define	Dbg_map_parse		Dbg64_map_parse
#define	Dbg_map_pass		Dbg64_map_pass
#define	Dbg_map_post_title	Dbg64_map_post_title
#define	Dbg_map_seg		Dbg64_map_seg
#define	Dbg_map_seg_order	Dbg64_map_seg_order
#define	Dbg_map_seg_os_order	Dbg64_map_seg_os_order
#define	Dbg_map_size_new	Dbg64_map_size_new
#define	Dbg_map_size_old	Dbg64_map_size_old
#define	Dbg_map_sort_seg	Dbg64_map_sort_seg
#define	Dbg_map_sort_title	Dbg64_map_sort_title
#define	Dbg_map_symbol		Dbg64_map_symbol
#define	Dbg_map_version		Dbg64_map_version

#define	Dbg_move_adjexpandreloc	Dbg64_move_adjexpandreloc
#define	Dbg_move_adjmovereloc	Dbg64_move_adjmovereloc
#define	Dbg_move_bad		Dbg64_move_bad
#define	Dbg_move_data		Dbg64_move_data
#define	Dbg_move_entry1		Dbg64_move_entry1
#define	Dbg_move_entry2		Dbg64_move_entry2
#define	Dbg_move_expand		Dbg64_move_expand
#define	Dbg_move_input		Dbg64_move_input
#define	Dbg_move_outmove	Dbg64_move_outmove
#define	Dbg_move_outsctadj	Dbg64_move_outsctadj
#define	Dbg_move_parexpn	Dbg64_move_parexpn

#define	Dbg_reloc_apply_reg	Dbg64_reloc_apply_reg
#define	Dbg_reloc_apply_val	Dbg64_reloc_apply_val
#define	Dbg_reloc_ars_entry	Dbg64_reloc_ars_entry
#define	Dbg_reloc_copy		Dbg64_reloc_copy
#define	Dbg_reloc_discard	Dbg64_reloc_discard
#define	Dbg_reloc_doact		Dbg64_reloc_doact
#define	Dbg_reloc_doact_title	Dbg64_reloc_doact_title
#define	Dbg_reloc_dooutrel	Dbg64_reloc_dooutrel
#define	Dbg_reloc_entry		Dbg64_reloc_entry
#define	Dbg_reloc_error		Dbg64_reloc_error
#define	Dbg_reloc_generate	Dbg64_reloc_generate
#define	Dbg_reloc_in		Dbg64_reloc_in
#define	Dbg_reloc_ors_entry	Dbg64_reloc_ors_entry
#define	Dbg_reloc_out		Dbg64_reloc_out
#define	Dbg_reloc_proc		Dbg64_reloc_proc
#define	Dbg_reloc_run		Dbg64_reloc_run
#define	Dbg_reloc_transition	Dbg64_reloc_transition
#define	Dbg_reloc_sloppycomdat	Dbg64_reloc_sloppycomdat

#define	Dbg_sec_added		Dbg64_sec_added
#define	Dbg_sec_backing		Dbg64_sec_backing
#define	Dbg_sec_created		Dbg64_sec_created
#define	Dbg_sec_discarded	Dbg64_sec_discarded
#define	Dbg_sec_genstr_compress	Dbg64_sec_genstr_compress
#define	Dbg_sec_group		Dbg64_sec_group
#define	Dbg_sec_gnu_comdat	Dbg64_sec_gnu_comdat
#define	Dbg_sec_in		Dbg64_sec_in
#define	Dbg_sec_order_error	Dbg64_sec_order_error
#define	Dbg_sec_order_list	Dbg64_sec_order_list
#define	Dbg_sec_redirected	Dbg64_sec_redirected
#define	Dbg_sec_strtab		Dbg64_sec_strtab
#define	Dbg_sec_unsup_strmerge	Dbg64_sec_unsup_strmerge

#define	Dbg_seg_desc_entry	Dbg64_seg_desc_entry
#define	Dbg_seg_entry		Dbg64_seg_entry
#define	Dbg_seg_list		Dbg64_seg_list
#define	Dbg_seg_os		Dbg64_seg_os
#define	Dbg_seg_title		Dbg64_seg_title

#define	Dbg_shdr_modified	Dbg64_shdr_modified

#define	Dbg_statistics_ar	Dbg64_statistics_ar
#define	Dbg_statistics_ld	Dbg64_statistics_ld

#define	Dbg_support_action	Dbg64_support_action
#define	Dbg_support_load	Dbg64_support_load
#define	Dbg_support_req		Dbg64_support_req
#define	Dbg_support_vnone	Dbg64_support_vnone

#define	Dbg_syminfo_entry	Dbg64_syminfo_entry
#define	Dbg_syminfo_title	Dbg64_syminfo_title

#define	Dbg_syms_ar_checking	Dbg64_syms_ar_checking
#define	Dbg_syms_ar_force	Dbg64_syms_ar_force
#define	Dbg_syms_ar_resolve	Dbg64_syms_ar_resolve
#define	Dbg_syms_ar_skip	Dbg64_syms_ar_skip
#define	Dbg_syms_ar_title	Dbg64_syms_ar_title
#define	Dbg_syms_cap_convert	Dbg64_syms_cap_convert
#define	Dbg_syms_cap_local	Dbg64_syms_cap_local
#define	Dbg_syms_cap_lookup	Dbg64_syms_cap_lookup
#define	Dbg_syms_cap_title	Dbg64_syms_cap_title
#define	Dbg_syms_copy_reloc	Dbg64_syms_copy_reloc
#define	Dbg_syms_created	Dbg64_syms_created
#define	Dbg_syms_discarded	Dbg64_syms_discarded
#define	Dbg_syms_dup_discarded	Dbg64_syms_dup_discarded
#define	Dbg_syms_dup_sort_addr	Dbg64_syms_dup_sort_addr
#define	Dbg_syms_entered	Dbg64_syms_entered
#define	Dbg_syms_entry		Dbg64_syms_entry
#define	Dbg_syms_global		Dbg64_syms_global
#define	Dbg_syms_ignore		Dbg64_syms_ignore
#define	Dbg_syms_ignore_gnuver	Dbg64_syms_ignore_gnuver
#define	Dbg_syms_lazy_rescan	Dbg64_syms_lazy_rescan
#define	Dbg_syms_lookup		Dbg64_syms_lookup
#define	Dbg_syms_new		Dbg64_syms_new
#define	Dbg_syms_old		Dbg64_syms_old
#define	Dbg_syms_process	Dbg64_syms_process
#define	Dbg_syms_reduce		Dbg64_syms_reduce
#define	Dbg_syms_resolved	Dbg64_syms_resolved
#define	Dbg_syms_resolving	Dbg64_syms_resolving
#define	Dbg_syms_sec_entry	Dbg64_syms_sec_entry
#define	Dbg_syms_sec_title	Dbg64_syms_sec_title
#define	Dbg_syms_spec_title	Dbg64_syms_spec_title
#define	Dbg_syms_updated	Dbg64_syms_updated
#define	Dbg_syms_up_title	Dbg64_syms_up_title
#define	Dbg_syms_wrap		Dbg64_syms_wrap

#define	Dbg_util_call_array	Dbg64_util_call_array
#define	Dbg_util_call_fini	Dbg64_util_call_fini
#define	Dbg_util_call_init	Dbg64_util_call_init
#define	Dbg_util_call_main	Dbg64_util_call_main
#define	Dbg_util_collect	Dbg64_util_collect
#define	Dbg_util_dbnotify	Dbg64_util_dbnotify
#define	Dbg_util_edge_in	Dbg64_util_edge_in
#define	Dbg_util_edge_out	Dbg64_util_edge_out
#define	Dbg_util_intoolate	Dbg64_util_intoolate
#define	Dbg_util_lcinterface	Dbg64_util_lcinterface
#define	Dbg_util_nl		Dbg64_util_nl
#define	Dbg_util_scc_entry	Dbg64_util_scc_entry
#define	Dbg_util_scc_title	Dbg64_util_scc_title
#define	Dbg_util_str		Dbg64_util_str

#define	Dbg_unused_file		Dbg64_unused_file
#define	Dbg_unused_lcinterface	Dbg64_unused_lcinterface
#define	Dbg_unused_path		Dbg64_unused_path
#define	Dbg_unused_sec		Dbg64_unused_sec
#define	Dbg_unused_unref	Dbg64_unused_unref

#define	Dbg_ver_avail_entry	Dbg64_ver_avail_entry
#define	Dbg_ver_avail_title	Dbg64_ver_avail_title
#define	Dbg_ver_def_title	Dbg64_ver_def_title
#define	Dbg_ver_desc_entry	Dbg64_ver_desc_entry
#define	Dbg_ver_need_done	Dbg64_ver_need_done
#define	Dbg_ver_need_entry	Dbg64_ver_need_entry
#define	Dbg_ver_need_title	Dbg64_ver_need_title
#define	Dbg_ver_nointerface	Dbg64_ver_nointerface
#define	Dbg_ver_symbol		Dbg64_ver_symbol

#else

#define	Dbg_demangle_name	Dbg32_demangle_name

#define	Dbg_bind_global		Dbg32_bind_global
#define	Dbg_bind_plt_summary	Dbg32_bind_plt_summary
#define	Dbg_bind_reject		Dbg32_bind_reject
#define	Dbg_bind_weak		Dbg32_bind_weak

#define	Dbg_cap_candidate	Dbg32_cap_candidate
#define	Dbg_cap_filter		Dbg32_cap_filter
#define	Dbg_cap_id		Dbg32_cap_id
#define	Dbg_cap_identical	Dbg32_cap_identical
#define	Dbg_cap_mapfile_title	Dbg32_cap_mapfile_title
#define	Dbg_cap_post_title	Dbg32_cap_post_title
#define	Dbg_cap_sec_title	Dbg32_cap_sec_title
#define	Dbg_cap_val		Dbg32_cap_val
#define	Dbg_cap_ptr_entry	Dbg32_cap_ptr_entry
#define	Dbg_cap_val_entry	Dbg32_cap_val_entry

#define	Dbg_dl_dladdr		Dbg32_dl_dladdr
#define	Dbg_dl_dlclose		Dbg32_dl_dlclose
#define	Dbg_dl_dldump		Dbg32_dl_dldump
#define	Dbg_dl_dlerror		Dbg32_dl_dlerror
#define	Dbg_dl_dlinfo		Dbg32_dl_dlinfo
#define	Dbg_dl_dlopen		Dbg32_dl_dlopen
#define	Dbg_dl_dlsym		Dbg32_dl_dlsym
#define	Dbg_dl_iphdr_enter	Dbg32_dl_iphdr_enter
#define	Dbg_dl_iphdr_callback	Dbg32_dl_iphdr_callback
#define	Dbg_dl_iphdr_mapchange	Dbg32_dl_iphdr_mapchange
#define	Dbg_dl_iphdr_unmap_ret	Dbg32_dl_iphdr_unmap_ret

#define	Dbg_ent_entry		Dbg32_ent_entry
#define	Dbg_ent_print		Dbg32_ent_print

#define	Dbg_file_analyze	Dbg32_file_analyze
#define	Dbg_file_ar		Dbg32_file_ar
#define	Dbg_file_ar_rescan	Dbg32_file_ar_rescan
#define	Dbg_file_bind_entry	Dbg32_file_bind_entry
#define	Dbg_file_bindings	Dbg32_file_bindings
#define	Dbg_file_bindings_done	Dbg32_file_bindings_done
#define	Dbg_file_cleanup	Dbg32_file_cleanup
#define	Dbg_file_cntl		Dbg32_file_cntl
#define	Dbg_file_config_dis	Dbg32_file_config_dis
#define	Dbg_file_config_obj	Dbg32_file_config_obj
#define	Dbg_file_del_rescan	Dbg32_file_del_rescan
#define	Dbg_file_delete		Dbg32_file_delete
#define	Dbg_file_deferred	Dbg32_file_deferred
#define	Dbg_file_elf		Dbg32_file_elf
#define	Dbg_file_filtee		Dbg32_file_filtee
#define	Dbg_file_filter		Dbg32_file_filter
#define	Dbg_file_fixname	Dbg32_file_fixname
#define	Dbg_file_generic	Dbg32_file_generic
#define	Dbg_file_hdl_action	Dbg32_file_hdl_action
#define	Dbg_file_hdl_collect	Dbg32_file_hdl_collect
#define	Dbg_file_hdl_title	Dbg32_file_hdl_title
#define	Dbg_file_lazyload	Dbg32_file_lazyload
#define	Dbg_file_ldso		Dbg32_file_ldso
#define	Dbg_file_mmapobj	Dbg32_file_mmapobj
#define	Dbg_file_mode_promote	Dbg32_file_mode_promote
#define	Dbg_file_modified	Dbg32_file_modified
#define	Dbg_file_needed		Dbg32_file_needed
#define	Dbg_file_output		Dbg32_file_output
#define	Dbg_file_preload	Dbg32_file_preload
#define	Dbg_file_prot		Dbg32_file_prot
#define	Dbg_file_rejected	Dbg32_file_rejected
#define	Dbg_file_reuse		Dbg32_file_reuse
#define	Dbg_file_skip		Dbg32_file_skip

#define	Dbg_got_display		Dbg32_got_display

#define	Dbg_libs_audit		Dbg32_libs_audit
#define	Dbg_libs_find		Dbg32_libs_find
#define	Dbg_libs_found		Dbg32_libs_found
#define	Dbg_libs_insecure	Dbg32_libs_insecure
#define	Dbg_libs_init		Dbg32_libs_init
#define	Dbg_libs_l		Dbg32_libs_l
#define	Dbg_libs_path		Dbg32_libs_path
#define	Dbg_libs_req		Dbg32_libs_req
#define	Dbg_libs_update		Dbg32_libs_update
#define	Dbg_libs_yp		Dbg32_libs_yp
#define	Dbg_libs_ylu		Dbg32_libs_ylu

#define	Dbg_map_cexp_id		Dbg32_map_cexp_id
#define	Dbg_map_dv		Dbg32_map_dv
#define	Dbg_map_dv_entry	Dbg32_map_dv_entry
#define	Dbg_map_ent		Dbg32_map_ent
#define	Dbg_map_ent_ord_title	Dbg32_map_ent_ord_title
#define	Dbg_map_hdr_noalloc	Dbg32_map_hdr_noalloc
#define	Dbg_map_parse		Dbg32_map_parse
#define	Dbg_map_pass		Dbg32_map_pass
#define	Dbg_map_post_title	Dbg32_map_post_title
#define	Dbg_map_seg		Dbg32_map_seg
#define	Dbg_map_seg_order	Dbg32_map_seg_order
#define	Dbg_map_seg_os_order	Dbg32_map_seg_os_order
#define	Dbg_map_size_new	Dbg32_map_size_new
#define	Dbg_map_size_old	Dbg32_map_size_old
#define	Dbg_map_sort_seg	Dbg32_map_sort_seg
#define	Dbg_map_sort_title	Dbg32_map_sort_title
#define	Dbg_map_symbol		Dbg32_map_symbol
#define	Dbg_map_version		Dbg32_map_version

#define	Dbg_move_adjexpandreloc	Dbg32_move_adjexpandreloc
#define	Dbg_move_adjmovereloc	Dbg32_move_adjmovereloc
#define	Dbg_move_bad		Dbg32_move_bad
#define	Dbg_move_data		Dbg32_move_data
#define	Dbg_move_entry1		Dbg32_move_entry1
#define	Dbg_move_entry2		Dbg32_move_entry2
#define	Dbg_move_expand		Dbg32_move_expand
#define	Dbg_move_input		Dbg32_move_input
#define	Dbg_move_outmove	Dbg32_move_outmove
#define	Dbg_move_outsctadj	Dbg32_move_outsctadj
#define	Dbg_move_parexpn	Dbg32_move_parexpn

#define	Dbg_reloc_apply_reg	Dbg32_reloc_apply_reg
#define	Dbg_reloc_apply_val	Dbg32_reloc_apply_val
#define	Dbg_reloc_ars_entry	Dbg32_reloc_ars_entry
#define	Dbg_reloc_copy		Dbg32_reloc_copy
#define	Dbg_reloc_discard	Dbg32_reloc_discard
#define	Dbg_reloc_doact		Dbg32_reloc_doact
#define	Dbg_reloc_doact_title	Dbg32_reloc_doact_title
#define	Dbg_reloc_dooutrel	Dbg32_reloc_dooutrel
#define	Dbg_reloc_entry		Dbg32_reloc_entry
#define	Dbg_reloc_error		Dbg32_reloc_error
#define	Dbg_reloc_generate	Dbg32_reloc_generate
#define	Dbg_reloc_in		Dbg32_reloc_in
#define	Dbg_reloc_ors_entry	Dbg32_reloc_ors_entry
#define	Dbg_reloc_out		Dbg32_reloc_out
#define	Dbg_reloc_proc		Dbg32_reloc_proc
#define	Dbg_reloc_run		Dbg32_reloc_run
#define	Dbg_reloc_transition	Dbg32_reloc_transition
#define	Dbg_reloc_sloppycomdat	Dbg32_reloc_sloppycomdat

#define	Dbg_sec_added		Dbg32_sec_added
#define	Dbg_sec_backing		Dbg32_sec_backing
#define	Dbg_sec_created		Dbg32_sec_created
#define	Dbg_sec_discarded	Dbg32_sec_discarded
#define	Dbg_sec_genstr_compress	Dbg32_sec_genstr_compress
#define	Dbg_sec_group		Dbg32_sec_group
#define	Dbg_sec_gnu_comdat	Dbg32_sec_gnu_comdat
#define	Dbg_sec_in		Dbg32_sec_in
#define	Dbg_sec_order_error	Dbg32_sec_order_error
#define	Dbg_sec_order_list	Dbg32_sec_order_list
#define	Dbg_sec_redirected	Dbg32_sec_redirected
#define	Dbg_sec_strtab		Dbg32_sec_strtab
#define	Dbg_sec_unsup_strmerge	Dbg32_sec_unsup_strmerge

#define	Dbg_seg_desc_entry	Dbg32_seg_desc_entry
#define	Dbg_seg_entry		Dbg32_seg_entry
#define	Dbg_seg_list		Dbg32_seg_list
#define	Dbg_seg_os		Dbg32_seg_os
#define	Dbg_seg_title		Dbg32_seg_title

#define	Dbg_shdr_modified	Dbg32_shdr_modified

#define	Dbg_statistics_ar	Dbg32_statistics_ar
#define	Dbg_statistics_ld	Dbg32_statistics_ld

#define	Dbg_support_action	Dbg32_support_action
#define	Dbg_support_load	Dbg32_support_load
#define	Dbg_support_req		Dbg32_support_req
#define	Dbg_support_vnone	Dbg32_support_vnone

#define	Dbg_syminfo_entry	Dbg32_syminfo_entry
#define	Dbg_syminfo_title	Dbg32_syminfo_title

#define	Dbg_syms_ar_checking	Dbg32_syms_ar_checking
#define	Dbg_syms_ar_force	Dbg32_syms_ar_force
#define	Dbg_syms_ar_resolve	Dbg32_syms_ar_resolve
#define	Dbg_syms_ar_skip	Dbg32_syms_ar_skip
#define	Dbg_syms_ar_title	Dbg32_syms_ar_title
#define	Dbg_syms_cap_convert	Dbg32_syms_cap_convert
#define	Dbg_syms_cap_local	Dbg32_syms_cap_local
#define	Dbg_syms_cap_lookup	Dbg32_syms_cap_lookup
#define	Dbg_syms_cap_title	Dbg32_syms_cap_title
#define	Dbg_syms_copy_reloc	Dbg32_syms_copy_reloc
#define	Dbg_syms_created	Dbg32_syms_created
#define	Dbg_syms_discarded	Dbg32_syms_discarded
#define	Dbg_syms_dup_discarded	Dbg32_syms_dup_discarded
#define	Dbg_syms_dup_sort_addr	Dbg32_syms_dup_sort_addr
#define	Dbg_syms_entered	Dbg32_syms_entered
#define	Dbg_syms_entry		Dbg32_syms_entry
#define	Dbg_syms_global		Dbg32_syms_global
#define	Dbg_syms_ignore		Dbg32_syms_ignore
#define	Dbg_syms_ignore_gnuver	Dbg32_syms_ignore_gnuver
#define	Dbg_syms_lazy_rescan	Dbg32_syms_lazy_rescan
#define	Dbg_syms_lookup		Dbg32_syms_lookup
#define	Dbg_syms_new		Dbg32_syms_new
#define	Dbg_syms_old		Dbg32_syms_old
#define	Dbg_syms_process	Dbg32_syms_process
#define	Dbg_syms_reduce		Dbg32_syms_reduce
#define	Dbg_syms_resolved	Dbg32_syms_resolved
#define	Dbg_syms_resolving	Dbg32_syms_resolving
#define	Dbg_syms_sec_entry	Dbg32_syms_sec_entry
#define	Dbg_syms_sec_title	Dbg32_syms_sec_title
#define	Dbg_syms_spec_title	Dbg32_syms_spec_title
#define	Dbg_syms_updated	Dbg32_syms_updated
#define	Dbg_syms_up_title	Dbg32_syms_up_title
#define	Dbg_syms_wrap		Dbg32_syms_wrap

#define	Dbg_util_call_array	Dbg32_util_call_array
#define	Dbg_util_call_fini	Dbg32_util_call_fini
#define	Dbg_util_call_init	Dbg32_util_call_init
#define	Dbg_util_call_main	Dbg32_util_call_main
#define	Dbg_util_collect	Dbg32_util_collect
#define	Dbg_util_dbnotify	Dbg32_util_dbnotify
#define	Dbg_util_edge_in	Dbg32_util_edge_in
#define	Dbg_util_edge_out	Dbg32_util_edge_out
#define	Dbg_util_intoolate	Dbg32_util_intoolate
#define	Dbg_util_lcinterface	Dbg32_util_lcinterface
#define	Dbg_util_nl		Dbg32_util_nl
#define	Dbg_util_scc_entry	Dbg32_util_scc_entry
#define	Dbg_util_scc_title	Dbg32_util_scc_title
#define	Dbg_util_str		Dbg32_util_str

#define	Dbg_unused_file		Dbg32_unused_file
#define	Dbg_unused_lcinterface	Dbg32_unused_lcinterface
#define	Dbg_unused_path		Dbg32_unused_path
#define	Dbg_unused_sec		Dbg32_unused_sec
#define	Dbg_unused_unref	Dbg32_unused_unref

#define	Dbg_ver_avail_entry	Dbg32_ver_avail_entry
#define	Dbg_ver_avail_title	Dbg32_ver_avail_title
#define	Dbg_ver_def_title	Dbg32_ver_def_title
#define	Dbg_ver_desc_entry	Dbg32_ver_desc_entry
#define	Dbg_ver_need_done	Dbg32_ver_need_done
#define	Dbg_ver_need_entry	Dbg32_ver_need_entry
#define	Dbg_ver_need_title	Dbg32_ver_need_title
#define	Dbg_ver_nointerface	Dbg32_ver_nointerface
#define	Dbg_ver_symbol		Dbg32_ver_symbol

#endif

/*
 * External Dbg_*() interface routines.
 */
extern	void	Dbg_args_file(Lm_list *, int, char *);
extern	void	Dbg_args_guidance_unknown(Lm_list *, const char *);
extern	void	Dbg_args_option(Lm_list *, int, int, char *);
extern	void	Dbg_args_str2chr(Lm_list *, int, const char *, int);
extern	void	Dbg_args_Wldel(Lm_list *, int, const char *);
extern	void	Dbg_audit_activity(Lm_list *, const char *, const char *,
		    uint_t);
extern	void	Dbg_audit_ignore(Rt_map *);
extern	void	Dbg_audit_interface(Lm_list *, const char *, const char *);
extern	void	Dbg_audit_lib(Rt_map *, const char *, int);
extern	void	Dbg_audit_objclose(Lm_list *, const char *, const char *);
extern	void	Dbg_audit_objopen(Lm_list *, int, const char *, const char *,
		    uint_t, Boolean);
extern	void	Dbg_audit_objfilter(Lm_list *, int, const char *, const char *,
		    const char *, const char *);
extern	void	Dbg_audit_objsearch(Lm_list *, int, const char *, const char *,
		    uint_t, const char *);
extern	void	Dbg_audit_pltenter(Lm_list *, int, const char *, const char *,
		    Addr);
extern	void	Dbg_audit_pltexit(Lm_list *, const char *, const char *);
extern	void	Dbg_audit_preinit(Lm_list *, const char *, const char *);
extern	void	Dbg_audit_symbind(Lm_list *, int, const char *, const char *,
		    Addr, uint_t);
extern	void	Dbg_audit_skip(Lm_list *, const char *, const char *);
extern	void	Dbg_audit_terminate(Lm_list *, const char *);
extern	void	Dbg_audit_version(Lm_list *, const char *, uint_t, uint_t);

extern	void	Dbg_basic_collect(Lm_list *);
extern	void	Dbg_basic_create(Lm_list *);
extern	void	Dbg_basic_finish(Lm_list *);
extern	void	Dbg_basic_files(Lm_list *);
extern	void	Dbg_basic_options(Lm_list *);
extern	void	Dbg_basic_relocate(Lm_list *);
extern	void	Dbg_basic_validate(Lm_list *);

extern	void	Dbg_bind_global(Rt_map *, Addr, Off, Xword, Pltbindtype,
		    Rt_map *, Addr, Off, const char *, uint_t);
extern	void	Dbg_bind_plt_summary(Lm_list *, Half, Word, Word, Word, Word,
		    Word, Word);
#if	defined(_ELF64)
extern	void	Dbg_bind_pltpad_from(Rt_map *, Addr, const char *);
extern	void	Dbg_bind_pltpad_to(Rt_map *, Addr, const char *, const char *);
#endif
extern	void	Dbg_bind_reject(Rt_map *, Rt_map *, const char *, int);
extern	void	Dbg_bind_weak(Rt_map *, Addr, Addr, const char *);

extern	void	Dbg_cap_candidate(Lm_list *, const char *);
extern	void	Dbg_cap_filter(Lm_list *, const char *, Rt_map *);
extern	void	Dbg_cap_id(Lm_list *, Lineno, const char *, const char *);
extern	void	Dbg_cap_identical(Lm_list *, const char *, const char *);
extern	void	Dbg_cap_mapfile_title(Lm_list *, Lineno);
extern	void	Dbg_cap_post_title(Lm_list *, int *);
extern	void	Dbg_cap_sec_title(Lm_list *, const char *);
extern	void	Dbg_cap_val(Lm_list *, Syscapset *, Syscapset *, Half);
extern	void	Dbg_cap_ptr_entry(Lm_list *, dbg_state_t, Xword, const char *);
extern	void	Dbg_cap_val_entry(Lm_list *, dbg_state_t, Xword, Xword, Half);

extern	void	Dbg_dl_dladdr(Rt_map *, void *);
extern	void	Dbg_dl_dlclose(Rt_map *, const char *, int);
extern	void	Dbg_dl_dldump(Rt_map *, const char *, const char *, int);
extern	void	Dbg_dl_dlerror(Rt_map *, const char *);
extern	void	Dbg_dl_dlinfo(Rt_map *, const char *, int, void *);
extern	void	Dbg_dl_dlopen(Rt_map *, const char *, int *, int);
extern	void	Dbg_dl_dlsym(Rt_map *, const char *, int *, const char *, int);
extern	void	Dbg_dl_iphdr_enter(Rt_map *, u_longlong_t, u_longlong_t);
extern	void	Dbg_dl_iphdr_callback(Lm_list *, struct dl_phdr_info *);
extern	void	Dbg_dl_iphdr_mapchange(Lm_list *, u_longlong_t, u_longlong_t);
extern	void	Dbg_dl_iphdr_unmap_ret(Lm_list *);

extern	const char *
		Dbg_demangle_name(const char *);

extern	void	Dbg_ent_entry(Lm_list *, uchar_t, Half, Ent_desc *);
extern	void	Dbg_ent_print(Lm_list *, uchar_t, Half, APlist *);

extern	void	Dbg_file_analyze(Rt_map *);
extern	void	Dbg_file_ar(Lm_list *, const char *, Boolean);
extern	void	Dbg_file_ar_rescan(Lm_list *, int, int);
extern	void	Dbg_file_bind_entry(Lm_list *, Bnd_desc *);
extern	void	Dbg_file_bindings(Rt_map *, int);
extern	void	Dbg_file_bindings_done(Lm_list *);
extern	void	Dbg_file_cleanup(Lm_list *, const char *, Aliste);
extern	void	Dbg_file_cntl(Lm_list *, Aliste, Aliste);
extern	void	Dbg_file_config_dis(Lm_list *, const char *, int);
extern	void	Dbg_file_config_obj(Lm_list *, const char *, const char *,
		    const char *);
extern	void	Dbg_file_del_rescan(Lm_list *);
extern	void	Dbg_file_delete(Rt_map *);
extern	void	Dbg_file_deferred(Lm_list *, const char *, const char *);
extern	void	Dbg_file_elf(Lm_list *, const char *, Addr, size_t,
		    const char *, Aliste);
extern	void	Dbg_file_filtee(Lm_list *, const char *, const char *, int);
extern	void	Dbg_file_filter(Lm_list *, const char *, const char *, int);
extern	void	Dbg_file_fixname(Lm_list *, const char *, const char *);
extern	void	Dbg_file_generic(Lm_list *, Ifl_desc *);
extern	void	Dbg_file_hdl_action(Grp_hdl *, Rt_map *, int, uint_t);
extern	void	Dbg_file_hdl_collect(Grp_hdl *, const char *);
extern	void	Dbg_file_hdl_title(int);
extern	void	Dbg_file_lazyload(Rt_map *, const char *, const char *);
extern	void	Dbg_file_ldso(Rt_map *, char **, auxv_t *, const char *,
		    Aliste);
extern	void	Dbg_file_mmapobj(Lm_list *, const char *, mmapobj_result_t *,
		    uint_t);
extern	void	Dbg_file_mode_promote(Rt_map *, int);
extern	void	Dbg_file_modified(Lm_list *, const char *, const char *,
		    const char *, int, int, Elf *, Elf *);
extern	void	Dbg_file_needed(Rt_map *, const char *);
extern	void	Dbg_file_output(Ofl_desc *);
extern	void	Dbg_file_preload(Lm_list *, const char *);
extern	void	Dbg_file_prot(Rt_map *, int);
extern	void	Dbg_file_rejected(Lm_list *, Rej_desc *, Half mach);
extern	void	Dbg_file_reuse(Lm_list *, const char *, const char *);
extern	void	Dbg_file_skip(Lm_list *, const char *, const char *);

extern	void	Dbg_got_display(Ofl_desc *, Off, int, Word, size_t);

extern	void	Dbg_libs_audit(Lm_list *, const char *, const char *);
extern	void	Dbg_libs_find(Lm_list *, const char *);
extern	void	Dbg_libs_found(Lm_list *, const char *, int);
extern	void	Dbg_libs_insecure(Lm_list *, const char *, int);
extern	void	Dbg_libs_init(Lm_list *, APlist *, APlist *);
extern	void	Dbg_libs_l(Lm_list *, const char *, const char *);
extern	void	Dbg_libs_path(Lm_list *, const char *, uint_t, const char *);
extern	void	Dbg_libs_req(Lm_list *, const char *, const char *,
		    const char *);
extern	void	Dbg_libs_update(Lm_list *, APlist *, APlist *);
extern	void	Dbg_libs_yp(Lm_list *, const char *);
extern	void	Dbg_libs_ylu(Lm_list *, const char *, const char *, int);

extern	void	Dbg_map_cexp_id(Lm_list *, Boolean, const char *, Lineno,
		    const char *);
extern	void	Dbg_map_dv(Lm_list *, const char *, Lineno);
extern	void	Dbg_map_dv_entry(Lm_list *, Lineno, int, const char *);
extern	void	Dbg_map_ent(Lm_list *, Ent_desc *, Ofl_desc *, Lineno);
extern	void	Dbg_map_ent_ord_title(Lm_list *, const char *);
extern	void	Dbg_map_hdr_noalloc(Lm_list *, Lineno);
extern	void	Dbg_map_parse(Lm_list *, const char *, int);
extern	void	Dbg_map_pass(Lm_list *, Boolean, const char *, Lineno,
		    const char *);
extern	void	Dbg_map_post_title(Lm_list *);
extern	void	Dbg_map_seg(Ofl_desc *, dbg_state_t, int, Sg_desc *, Lineno);
extern	void	Dbg_map_seg_order(Ofl_desc *, uchar_t, Half, dbg_state_t,
		    Lineno);
extern	void	Dbg_map_seg_os_order(Lm_list *, Sg_desc *, const char *,
		    Word, Lineno);
extern	void	Dbg_map_size_new(Lm_list *, const char *, const char *, Lineno);
extern	void	Dbg_map_size_old(Ofl_desc *, Sym_desc *, const char *, Lineno);
extern	void	Dbg_map_sort_title(Lm_list *, Boolean);
extern	void	Dbg_map_sort_seg(Lm_list *, uchar_t, Half, Sg_desc *);
extern	void	Dbg_map_symbol(Ofl_desc *, Sym_desc *);
extern	void	Dbg_map_version(Lm_list *, const char *, const char *, int);

extern	void	Dbg_move_adjexpandreloc(Lm_list *, Xword, const char *);
extern	void	Dbg_move_adjmovereloc(Lm_list *, Xword, Xword, const char *);
extern	void	Dbg_move_bad(Lm_list *, ulong_t, const char *, Addr);
extern	void	Dbg_move_data(Rt_map *);
extern	void	Dbg_move_entry1(Lm_list *, int, Move *, Sym_desc *);
extern	void	Dbg_move_entry2(Lm_list *, Move *, Word, const char *);
extern	void	Dbg_move_expand(Lm_list *, Move *, Addr);
extern	void	Dbg_move_input(Lm_list *, const char *);
extern	void	Dbg_move_outmove(Lm_list *, const char *);
extern	void	Dbg_move_outsctadj(Lm_list *, Sym_desc *);
extern	void	Dbg_move_parexpn(Lm_list *, const char *, const char *);

extern	void	Dbg_reloc_apply_reg(Lm_list *, int, Half, Xword, Xword);
extern	void	Dbg_reloc_apply_val(Lm_list *, int, Xword, Xword);
extern	void	Dbg_reloc_ars_entry(Lm_list *, int, Word, Half, Rel_desc *);
extern	void	Dbg_reloc_copy(Rt_map *, Rt_map *, const char *, int);
extern	void	Dbg_reloc_discard(Lm_list *, Half, Rel_desc *);
extern	void	Dbg_reloc_doact(Lm_list *, int, Half, Word, Rel_desc *,
		    Xword, Xword, rel_desc_sname_func_t);
extern	void	Dbg_reloc_doact_title(Lm_list *);
extern	void	Dbg_reloc_dooutrel(Lm_list *, Word);
extern	void	Dbg_reloc_entry(Lm_list *, const char *, Half, Word, void *,
		    const char *, const char *, const char *);
extern	void	Dbg_reloc_error(Lm_list *, int, Half, Word, void *,
		    const char *);
extern	void	Dbg_reloc_generate(Lm_list *, Os_desc *, Word);
extern	void	Dbg_reloc_in(Lm_list *, int, Half, Word, void *, const char *,
		    Word, const char *);
extern	void	Dbg_reloc_ors_entry(Lm_list *, int, Word, Half, Rel_desc *);
extern	void	Dbg_reloc_out(Ofl_desc *, int, Word, void *, const char *,
		    const char *);
extern	void	Dbg_reloc_proc(Lm_list *, Os_desc *, Is_desc *, Is_desc *);
extern	void	Dbg_reloc_run(Rt_map *, uint_t, int, int);
extern	void	Dbg_reloc_transition(Lm_list *, Half, Word, Rel_desc *,
		    rel_desc_sname_func_t);
extern	void	Dbg_reloc_sloppycomdat(Lm_list *, Sym_desc *);

extern	void	Dbg_sec_added(Lm_list *, Os_desc *, Sg_desc *);
extern	void	Dbg_sec_backing(Lm_list *);
extern	void	Dbg_sec_created(Lm_list *, Os_desc *, Sg_desc *);
extern	void	Dbg_sec_discarded(Lm_list *, Is_desc *, Is_desc *);
extern	void	Dbg_sec_genstr_compress(Lm_list *, const char *,
		    Xword, Xword);
extern	void	Dbg_sec_group(Lm_list *, Is_desc *, Group_desc *);
extern	void	Dbg_sec_gnu_comdat(Lm_list *, Is_desc *, Boolean, Boolean);
extern	void	Dbg_sec_in(Lm_list *, Is_desc *);
extern	void	Dbg_sec_order_error(Lm_list *, Ifl_desc *, Word, int);
extern	void	Dbg_sec_order_list(Ofl_desc *, int);
extern	void	Dbg_sec_redirected(Lm_list *, Is_desc *, const char *);
extern	void	Dbg_sec_strtab(Lm_list *, Os_desc *, Str_tbl *);
extern	void	Dbg_sec_unsup_strmerge(Lm_list *, Is_desc *);

extern	void	Dbg_seg_desc_entry(Lm_list *, uchar_t, Half, int, Sg_desc *,
		    Boolean);
extern	void	Dbg_seg_entry(Ofl_desc *, int, Sg_desc *);
extern	void	Dbg_seg_list(Lm_list *, uchar_t, Half, APlist *);
extern	void	Dbg_seg_os(Ofl_desc *, Os_desc *, int);
extern	void	Dbg_seg_title(Lm_list *);

extern	void	Dbg_shdr_modified(Lm_list *, const char *, uchar_t, Half,
		    Word, Shdr *, Shdr *, const char *);

extern	void	Dbg_statistics_ar(Ofl_desc *);
extern	void	Dbg_statistics_ld(Ofl_desc *);

extern	void	Dbg_support_action(Lm_list *, const char *, const char *,
		    Support_ndx, const char *);
extern	void	Dbg_support_load(Lm_list *, const char *, const char *);
extern	void	Dbg_support_req(Lm_list *, const char *, int);
extern	void	Dbg_support_vnone(Lm_list *, const char *);

extern	void	Dbg_syminfo_entry(Lm_list *, Word, Syminfo *, Sym *,
		    const char *, Dyn *);
extern	void	Dbg_syminfo_title(Lm_list *);

extern	void	Dbg_syms_ar_checking(Lm_list *, const char *, const char *,
		    Elf_Arsym *);
extern	void	Dbg_syms_ar_force(Lm_list *, const char *, const char *);
extern	void	Dbg_syms_ar_resolve(Lm_list *, const char *, const char *,
		    Elf_Arsym *);
extern	void	Dbg_syms_ar_skip(Lm_list *, const char *, Elf_Arsym *);
extern	void	Dbg_syms_ar_title(Lm_list *, const char *, Boolean);
extern	void	Dbg_syms_cap_convert(Ofl_desc *, Word, const char *, Sym *);
extern	void	Dbg_syms_cap_local(Ofl_desc *, Word, const char *, Sym *,
		    Sym_desc *);
extern	void	Dbg_syms_cap_lookup(Rt_map *, uint_t, const char *, uint_t,
		    Half, Syscapset *);
extern	void	Dbg_syms_cap_title(Ofl_desc *);
extern	void	Dbg_syms_copy_reloc(Ofl_desc *, Sym_desc *, Word);
extern	void	Dbg_syms_created(Lm_list *, const char *);
extern	void	Dbg_syms_discarded(Lm_list *, Sym_desc *);
extern	void	Dbg_syms_dup_discarded(Lm_list *, Word ndx, Sym_desc *);
extern	void	Dbg_syms_dup_sort_addr(Lm_list *, const char *, const char *,
		    const char *, Addr);
extern	void	Dbg_syms_entered(Ofl_desc *, Sym *, Sym_desc *);
extern	void	Dbg_syms_entry(Lm_list *, Word, Sym_desc *);
extern	void	Dbg_syms_global(Lm_list *, Word, const char *);
extern	void	Dbg_syms_ignore(Ofl_desc *, Sym_desc *);
extern	void	Dbg_syms_ignore_gnuver(Rt_map *, const char *, Word, Versym);
extern	void	Dbg_syms_lazy_rescan(Lm_list *, const char *);
extern	void	Dbg_syms_lookup(Rt_map *, const char *, const char *);
extern	void	Dbg_syms_new(Ofl_desc *, Sym *, Sym_desc *);
extern	void	Dbg_syms_old(Ofl_desc *, Sym_desc *);
extern	void	Dbg_syms_process(Lm_list *, Ifl_desc *);
extern	void	Dbg_syms_reduce(Ofl_desc *, int, Sym_desc *, int,
		    const char *);
extern	void	Dbg_syms_resolved(Ofl_desc *, Sym_desc *);
extern	void	Dbg_syms_resolving(Ofl_desc *, Word, const char *, int, int,
		    Sym *, Sym *, Sym_desc *, Ifl_desc *);
extern	void	Dbg_syms_sec_entry(Lm_list *, Word, Sg_desc *, Os_desc *);
extern	void	Dbg_syms_sec_title(Lm_list *);
extern	void	Dbg_syms_spec_title(Lm_list *);
extern	void	Dbg_syms_updated(Ofl_desc *, Sym_desc *, const char *);
extern	void	Dbg_syms_up_title(Lm_list *);
extern	void	Dbg_syms_wrap(Lm_list *, Word, const char *, const char *);

extern	void	Dbg_tls_modactivity(Lm_list *, void *, uint_t);
extern	void	Dbg_tls_static_block(Lm_list *, void *, ulong_t, ulong_t);
extern	void	Dbg_tls_static_resv(Rt_map *, ulong_t, ulong_t);

extern	void	Dbg_util_call_array(Rt_map *, void *, int, Word);
extern	void	Dbg_util_call_fini(Rt_map *);
extern	void	Dbg_util_call_init(Rt_map *, int);
extern	void	Dbg_util_call_main(Rt_map *);
extern	void	Dbg_util_collect(Rt_map *, int, int);
extern	void	Dbg_util_dbnotify(Lm_list *, rd_event_e, r_state_e);
extern	void	Dbg_util_edge_in(Lm_list *, Rt_map *, uint_t, Rt_map *,
		    int, int);
extern	void	Dbg_util_edge_out(Rt_map *, Rt_map *);
extern	void	Dbg_util_intoolate(Rt_map *);
extern	void	Dbg_util_lcinterface(Rt_map *, int, char *);
extern	void	Dbg_util_nl(Lm_list *, int);
extern	void	Dbg_util_scc_entry(Rt_map *, uint_t);
extern	void	Dbg_util_scc_title(Lm_list *, int);
extern	void	Dbg_util_str(Lm_list *, const char *);

extern	void	Dbg_unused_file(Lm_list *, const char *, int, uint_t);
extern	void	Dbg_unused_lcinterface(Rt_map *, Rt_map *, int);
extern	void	Dbg_unused_path(Lm_list *, const char *, uint_t, uint_t,
		    const char *);
extern	void	Dbg_unused_sec(Lm_list *, Is_desc *);
extern	void	Dbg_unused_unref(Rt_map *, const char *);

extern	void	Dbg_ver_avail_entry(Lm_list *, Ver_index *, const char *);
extern	void	Dbg_ver_avail_title(Lm_list *, const char *);
extern	void	Dbg_ver_def_title(Lm_list *, const char *);
extern	void	Dbg_ver_desc_entry(Lm_list *, Ver_desc *);
extern	void	Dbg_ver_need_done(Lm_list *);
extern	void	Dbg_ver_need_entry(Lm_list *, Half, const char *,
		    const char *);
extern	void	Dbg_ver_need_title(Lm_list *, const char *);
extern	void	Dbg_ver_nointerface(Lm_list *, const char *);
extern	void	Dbg_ver_symbol(Lm_list *, const char *);

/*
 * Define Elf_*() interface flags.
 */
#define	ELF_DBG_ELFDUMP		1
#define	ELF_DBG_RTLD		2
#define	ELF_DBG_LD		3
#define	ELF_DBG_LD_ACT		4

/*
 * Define generic Elf_*() interfaces.
 */
extern	void Elf_syminfo_entry(Lm_list *, Word, Syminfo *, const char *,
	    const char *);
extern	void Elf_syminfo_title(Lm_list *);

/*
 * Establish ELF32 and ELF64 class Elf_*() interfaces.
 */
#if	defined(_ELF64)

#define	Elf_cap_entry		Elf64_cap_entry
#define	Elf_cap_title		Elf64_cap_title

#define	Elf_demangle_name	Elf64_demangle_name
#define	Elf_dyn_entry		Elf64_dyn_entry
#define	Elf_dyn_null_entry	Elf64_dyn_null_entry
#define	Elf_dyn_title		Elf64_dyn_title

#define	Elf_ehdr		Elf64_ehdr

#define	Elf_got_entry		Elf64_got_entry
#define	Elf_got_title		Elf64_got_title

#define	Elf_reloc_apply_reg	Elf64_reloc_apply_reg
#define	Elf_reloc_apply_val	Elf64_reloc_apply_val
#define	Elf_reloc_entry_1	Elf64_reloc_entry_1
#define	Elf_reloc_entry_2	Elf64_reloc_entry_2
#define	Elf_reloc_title		Elf64_reloc_title

#define	Elf_phdr		Elf64_phdr

#define	Elf_shdr		Elf64_shdr

#define	Elf_syms_table_entry	Elf64_syms_table_entry
#define	Elf_syms_table_title	Elf64_syms_table_title

#define	Elf_ver_def_title	Elf64_ver_def_title
#define	Elf_ver_line_1		Elf64_ver_line_1
#define	Elf_ver_line_2		Elf64_ver_line_2
#define	Elf_ver_line_3		Elf64_ver_line_3
#define	Elf_ver_line_4		Elf64_ver_line_4
#define	Elf_ver_line_5		Elf64_ver_line_5
#define	Elf_ver_need_title	Elf64_ver_need_title

#else

#define	Elf_cap_entry		Elf32_cap_entry
#define	Elf_cap_title		Elf32_cap_title

#define	Elf_demangle_name	Elf32_demangle_name
#define	Elf_dyn_entry		Elf32_dyn_entry
#define	Elf_dyn_null_entry	Elf32_dyn_null_entry
#define	Elf_dyn_title		Elf32_dyn_title

#define	Elf_ehdr		Elf32_ehdr

#define	Elf_got_entry		Elf32_got_entry
#define	Elf_got_title		Elf32_got_title

#define	Elf_reloc_apply_reg	Elf32_reloc_apply_reg
#define	Elf_reloc_apply_val	Elf32_reloc_apply_val
#define	Elf_reloc_entry_1	Elf32_reloc_entry_1
#define	Elf_reloc_entry_2	Elf32_reloc_entry_2
#define	Elf_reloc_title		Elf32_reloc_title

#define	Elf_phdr		Elf32_phdr

#define	Elf_shdr		Elf32_shdr

#define	Elf_syms_table_entry	Elf32_syms_table_entry
#define	Elf_syms_table_title	Elf32_syms_table_title

#define	Elf_ver_def_title	Elf32_ver_def_title
#define	Elf_ver_line_1		Elf32_ver_line_1
#define	Elf_ver_line_2		Elf32_ver_line_2
#define	Elf_ver_line_3		Elf32_ver_line_3
#define	Elf_ver_line_4		Elf32_ver_line_4
#define	Elf_ver_line_5		Elf32_ver_line_5
#define	Elf_ver_need_title	Elf32_ver_need_title

#endif

extern	void	Elf_cap_entry(Lm_list *, Cap *, int, const char *, size_t,
		    Half);
extern	void	Elf_cap_title(Lm_list *);

extern	const char \
		*Elf_demangle_name(const char *);
extern	void	Elf_dyn_entry(Lm_list *, Dyn *, int, const char *,
		    uchar_t, Half);
extern	void	Elf_dyn_null_entry(Lm_list *, Dyn *, int, int);
extern	void	Elf_dyn_title(Lm_list *);

extern	void	Elf_ehdr(Lm_list *, Ehdr *, Shdr *);

extern	void	Elf_got_entry(Lm_list *, Sword, Addr, Xword, Half,
		    uchar_t, uchar_t, Word, void *, const char *);
extern	void	Elf_got_title(Lm_list *);

extern	void	Elf_phdr(Lm_list *, uchar_t, Half, Phdr *);

extern	void	Elf_reloc_apply_val(Lm_list *, int, Xword, Xword);
extern	void	Elf_reloc_apply_reg(Lm_list *, int, Half, Xword, Xword);
extern	void	Elf_reloc_entry_1(Lm_list *, int, const char *, Half, Word,
		    void *, const char *, const char *, const char *);
extern	void	Elf_reloc_entry_2(Lm_list *, int, const char *, Word,
		    const char *, Off, Sxword, const char *, const char *,
		    const char *);
extern	void	Elf_reloc_title(Lm_list *, int, Word);

extern	void	Elf_shdr(Lm_list *, uchar_t, Half, Shdr *);

extern	void	Elf_syms_table_entry(Lm_list *, int, const char *, uchar_t,
		    Half, Sym *, Versym, int, const char *, const char *);
extern	void	Elf_syms_table_title(Lm_list *, int);

extern	void	Elf_ver_def_title(Lm_list *);
extern	void	Elf_ver_line_1(Lm_list *, const char *, const char *,
		    const char *, const char *);
extern	void	Elf_ver_line_2(Lm_list *, const char *, const char *);
extern	void	Elf_ver_line_3(Lm_list *, const char *, const char *,
		    const char *);
extern	void	Elf_ver_line_4(Lm_list *, const char *);
extern	void	Elf_ver_line_5(Lm_list *, const char *, const char *);
extern	void	Elf_ver_need_title(Lm_list *, int);


#ifdef	__cplusplus
}
#endif

#endif /* _DEBUG_H */
