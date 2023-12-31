/*
 * CDDL HEADER START
 *
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2016, 2017 by Delphix. All rights reserved.
 * Copyright 2020 Joyent, Inc.
 */

#include "lua.h"
#include "lauxlib.h"

#include <sys/zcp.h>
#include <sys/zcp_set.h>
#include <sys/zcp_change_key.h>
#include <sys/dsl_crypt.h>
#include <sys/dsl_dir.h>
#include <sys/dsl_pool.h>
#include <sys/dsl_prop.h>
#include <sys/dsl_synctask.h>
#include <sys/dsl_dataset.h>
#include <sys/dsl_bookmark.h>
#include <sys/dsl_destroy.h>
#include <sys/dmu_objset.h>
#include <sys/zfs_znode.h>
#include <sys/zfeature.h>
#include <sys/metaslab.h>

#define	DST_AVG_BLKSHIFT 14

typedef struct zcp_inherit_prop_arg {
	lua_State		*zipa_state;
	const char		*zipa_prop;
	dsl_props_set_arg_t	zipa_dpsa;
} zcp_inherit_prop_arg_t;

typedef int (zcp_synctask_func_t)(lua_State *, boolean_t, nvlist_t *);
typedef struct zcp_synctask_info {
	const char *name;
	zcp_synctask_func_t *func;
	const zcp_arg_t pargs[4];
	const zcp_arg_t kwargs[2];
	zfs_space_check_t space_check;
	int blocks_modified;
} zcp_synctask_info_t;

/*
 * Generic synctask interface for channel program syncfuncs.
 *
 * To perform some action in syncing context, we'd generally call
 * dsl_sync_task(), but since the Lua script is already running inside a
 * synctask we need to leave out some actions (such as acquiring the config
 * rwlock and performing space checks).
 *
 * If 'sync' is false, executes a dry run and returns the error code.
 *
 * If we are not running in syncing context and we are not doing a dry run
 * (meaning we are running a zfs.sync function in open-context) then we
 * return a Lua error.
 *
 * This function also handles common fatal error cases for channel program
 * library functions. If a fatal error occurs, err_dsname will be the dataset
 * name reported in error messages, if supplied.
 */
static int
zcp_sync_task(lua_State *state, dsl_checkfunc_t *checkfunc,
    dsl_syncfunc_t *syncfunc, void *arg, boolean_t sync, const char *err_dsname)
{
	int err;
	zcp_run_info_t *ri = zcp_run_info(state);

	err = checkfunc(arg, ri->zri_tx);
	if (!sync)
		return (err);

	if (!ri->zri_sync) {
		return (luaL_error(state, "running functions from the zfs.sync "
		    "submodule requires passing sync=TRUE to "
		    "lzc_channel_program() (i.e. do not specify the \"-n\" "
		    "command line argument)"));
	}

	if (err == 0) {
		syncfunc(arg, ri->zri_tx);
	} else if (err == EIO) {
		if (err_dsname != NULL) {
			return (luaL_error(state,
			    "I/O error while accessing dataset '%s'",
			    err_dsname));
		} else {
			return (luaL_error(state,
			    "I/O error while accessing dataset."));
		}
	}

	return (err);
}


static int zcp_synctask_destroy(lua_State *, boolean_t, nvlist_t *);
static zcp_synctask_info_t zcp_synctask_destroy_info = {
	.name = "destroy",
	.func = zcp_synctask_destroy,
	.pargs = {
	    {.za_name = "filesystem | snapshot", .za_lua_type = LUA_TSTRING},
	    {NULL, 0}
	},
	.kwargs = {
	    {.za_name = "defer", .za_lua_type = LUA_TBOOLEAN},
	    {NULL, 0}
	},
	.space_check = ZFS_SPACE_CHECK_DESTROY,
	.blocks_modified = 0
};

/* ARGSUSED */
static int
zcp_synctask_destroy(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	const char *dsname = lua_tostring(state, 1);

	boolean_t issnap = (strchr(dsname, '@') != NULL);

	if (!issnap && !lua_isnil(state, 2)) {
		return (luaL_error(state,
		    "'deferred' kwarg only supported for snapshots: %s",
		    dsname));
	}

	if (issnap) {
		dsl_destroy_snapshot_arg_t ddsa = { 0 };
		ddsa.ddsa_name = dsname;
		if (!lua_isnil(state, 2)) {
			ddsa.ddsa_defer = lua_toboolean(state, 2);
		} else {
			ddsa.ddsa_defer = B_FALSE;
		}

		err = zcp_sync_task(state, dsl_destroy_snapshot_check,
		    dsl_destroy_snapshot_sync, &ddsa, sync, dsname);
	} else {
		dsl_destroy_head_arg_t ddha = { 0 };
		ddha.ddha_name = dsname;

		err = zcp_sync_task(state, dsl_destroy_head_check,
		    dsl_destroy_head_sync, &ddha, sync, dsname);
	}

	return (err);
}

static int zcp_synctask_promote(lua_State *, boolean_t, nvlist_t *);
static zcp_synctask_info_t zcp_synctask_promote_info = {
	.name = "promote",
	.func = zcp_synctask_promote,
	.pargs = {
	    {.za_name = "clone", .za_lua_type = LUA_TSTRING},
	    {NULL, 0}
	},
	.kwargs = {
	    {NULL, 0}
	},
	.space_check = ZFS_SPACE_CHECK_RESERVED,
	.blocks_modified = 3
};

static int
zcp_synctask_promote(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	dsl_dataset_promote_arg_t ddpa = { 0 };
	const char *dsname = lua_tostring(state, 1);
	zcp_run_info_t *ri = zcp_run_info(state);

	ddpa.ddpa_clonename = dsname;
	ddpa.err_ds = err_details;
	ddpa.cr = ri->zri_cred;

	/*
	 * If there was a snapshot name conflict, then err_ds will be filled
	 * with a list of conflicting snapshot names.
	 */
	err = zcp_sync_task(state, dsl_dataset_promote_check,
	    dsl_dataset_promote_sync, &ddpa, sync, dsname);

	return (err);
}

static int zcp_synctask_rollback(lua_State *, boolean_t, nvlist_t *err_details);
static zcp_synctask_info_t zcp_synctask_rollback_info = {
	.name = "rollback",
	.func = zcp_synctask_rollback,
	.space_check = ZFS_SPACE_CHECK_RESERVED,
	.blocks_modified = 1,
	.pargs = {
	    {.za_name = "filesystem", .za_lua_type = LUA_TSTRING},
	    {NULL, 0}
	},
	.kwargs = {
	    {NULL, 0}
	}
};

static int
zcp_synctask_rollback(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	const char *dsname = lua_tostring(state, 1);
	dsl_dataset_rollback_arg_t ddra = { 0 };

	ddra.ddra_fsname = dsname;
	ddra.ddra_result = err_details;

	err = zcp_sync_task(state, dsl_dataset_rollback_check,
	    dsl_dataset_rollback_sync, &ddra, sync, dsname);

	return (err);
}

static int zcp_synctask_snapshot(lua_State *, boolean_t, nvlist_t *);
static zcp_synctask_info_t zcp_synctask_snapshot_info = {
	.name = "snapshot",
	.func = zcp_synctask_snapshot,
	.pargs = {
	    {.za_name = "filesystem@snapname | volume@snapname",
	    .za_lua_type = LUA_TSTRING},
	    {NULL, 0}
	},
	.kwargs = {
	    {NULL, 0}
	},
	.space_check = ZFS_SPACE_CHECK_NORMAL,
	.blocks_modified = 3
};

/* ARGSUSED */
static int
zcp_synctask_snapshot(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	dsl_dataset_snapshot_arg_t ddsa = { 0 };
	const char *dsname = lua_tostring(state, 1);
	zcp_run_info_t *ri = zcp_run_info(state);

	/*
	 * On old pools, the ZIL must not be active when a snapshot is created,
	 * but we can't suspend the ZIL because we're already in syncing
	 * context.
	 */
	if (spa_version(ri->zri_pool->dp_spa) < SPA_VERSION_FAST_SNAP) {
		return (ENOTSUP);
	}

	/*
	 * We only allow for a single snapshot rather than a list, so the
	 * error list output is unnecessary.
	 */
	ddsa.ddsa_errors = NULL;
	ddsa.ddsa_props = NULL;
	ddsa.ddsa_cr = ri->zri_cred;
	ddsa.ddsa_snaps = fnvlist_alloc();
	fnvlist_add_boolean(ddsa.ddsa_snaps, dsname);

	zcp_cleanup_handler_t *zch = zcp_register_cleanup(state,
	    (zcp_cleanup_t *)&fnvlist_free, ddsa.ddsa_snaps);

	err = zcp_sync_task(state, dsl_dataset_snapshot_check,
	    dsl_dataset_snapshot_sync, &ddsa, sync, dsname);

	zcp_deregister_cleanup(state, zch);
	fnvlist_free(ddsa.ddsa_snaps);

	return (err);
}

static int zcp_synctask_inherit_prop(lua_State *, boolean_t,
    nvlist_t *err_details);
static zcp_synctask_info_t zcp_synctask_inherit_prop_info = {
	.name = "inherit",
	.func = zcp_synctask_inherit_prop,
	.space_check = ZFS_SPACE_CHECK_RESERVED,
	.blocks_modified = 2, /* 2 * numprops */
	.pargs = {
		{ .za_name = "dataset", .za_lua_type = LUA_TSTRING },
		{ .za_name = "property", .za_lua_type = LUA_TSTRING },
		{ NULL, 0 }
	},
	.kwargs = {
		{ NULL, 0 }
	},
};

static int
zcp_synctask_inherit_prop_check(void *arg, dmu_tx_t *tx)
{
	zcp_inherit_prop_arg_t *args = arg;
	zfs_prop_t prop = zfs_name_to_prop(args->zipa_prop);

	if (prop == ZPROP_INVAL) {
		if (zfs_prop_user(args->zipa_prop))
			return (0);

		return (EINVAL);
	}

	if (zfs_prop_readonly(prop))
		return (EINVAL);

	if (!zfs_prop_inheritable(prop))
		return (EINVAL);

	return (dsl_props_set_check(&args->zipa_dpsa, tx));
}

static void
zcp_synctask_inherit_prop_sync(void *arg, dmu_tx_t *tx)
{
	zcp_inherit_prop_arg_t *args = arg;
	dsl_props_set_arg_t *dpsa = &args->zipa_dpsa;

	dsl_props_set_sync(dpsa, tx);
}

static int
zcp_synctask_inherit_prop(lua_State *state, boolean_t sync,
    nvlist_t *err_details)
{
	int err;
	zcp_inherit_prop_arg_t zipa = { 0 };
	dsl_props_set_arg_t *dpsa = &zipa.zipa_dpsa;

	const char *dsname = lua_tostring(state, 1);
	const char *prop = lua_tostring(state, 2);

	zipa.zipa_state = state;
	zipa.zipa_prop = prop;
	dpsa->dpsa_dsname = dsname;
	dpsa->dpsa_source = ZPROP_SRC_INHERITED;
	dpsa->dpsa_props = fnvlist_alloc();
	fnvlist_add_boolean(dpsa->dpsa_props, prop);

	zcp_cleanup_handler_t *zch = zcp_register_cleanup(state,
	    (zcp_cleanup_t *)&fnvlist_free, dpsa->dpsa_props);

	err = zcp_sync_task(state, zcp_synctask_inherit_prop_check,
	    zcp_synctask_inherit_prop_sync, &zipa, sync, dsname);

	zcp_deregister_cleanup(state, zch);
	fnvlist_free(dpsa->dpsa_props);

	return (err);
}

static int zcp_synctask_set_prop(lua_State *, boolean_t, nvlist_t *err_details);
static zcp_synctask_info_t zcp_synctask_set_prop_info = {
	.name = "set_prop",
	.func = zcp_synctask_set_prop,
	.space_check = ZFS_SPACE_CHECK_RESERVED,
	.blocks_modified = 2,
	.pargs = {
		{ .za_name = "dataset", .za_lua_type = LUA_TSTRING},
		{ .za_name = "property", .za_lua_type =  LUA_TSTRING},
		{ .za_name = "value", .za_lua_type =  LUA_TSTRING},
		{ NULL, 0 }
	},
	.kwargs = {
		{ NULL, 0 }
	}
};

static int
zcp_synctask_set_prop(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	zcp_set_prop_arg_t args = { 0 };

	const char *dsname = lua_tostring(state, 1);
	const char *prop = lua_tostring(state, 2);
	const char *val = lua_tostring(state, 3);

	args.state = state;
	args.dsname = dsname;
	args.prop = prop;
	args.val = val;

	err = zcp_sync_task(state, zcp_set_prop_check, zcp_set_prop_sync,
	    &args, sync, dsname);

	return (err);
}

static int zcp_synctask_change_key(lua_State *, boolean_t, nvlist_t *);
static zcp_synctask_info_t zcp_synctask_change_key_info = {
	.name = "change_key",
	.func = zcp_synctask_change_key,
	.pargs = {
		{ .za_name = "dataset", .za_lua_type = LUA_TSTRING },
		{ .za_name = "key", .za_lua_type = LUA_TSTRING },
		{ .za_name = "format", .za_lua_type = LUA_TSTRING },
		{ NULL, 0 },
	},
	.kwargs = {
		{ NULL, 0 }
	},
	.space_check = ZFS_SPACE_CHECK_RESERVED,
	/*
	 * This is the same value that is used when zfs change-key is run.
	 * See spa_keystore_change_key() in dsl_crypt.c
	 */
	.blocks_modified = 15
};

static int
zcp_synctask_change_key(lua_State *state, boolean_t sync, nvlist_t *err_details)
{
	int err;
	spa_keystore_change_key_args_t skcka = { 0 };
	dsl_crypto_params_t *dcp = NULL;
	const char *dsname;
	const char *key;
	const char *format;
	size_t keylen;
	uint64_t keyformat;

	dsname = lua_tostring(state, 1);

	/*
	 * The key may be raw key, which could contain NUL within it.
	 * Use lua_tolstring() instead of lua_tostring() to obtain the length.
	 */
	key = lua_tolstring(state, 2, &keylen);

	format = lua_tostring(state, 3);

	if (zfs_prop_string_to_index(ZFS_PROP_KEYFORMAT, format,
	    &keyformat) != 0)
		return (SET_ERROR(EINVAL));

	err = zcp_synctask_change_key_create_params(key, keylen, keyformat,
	    &dcp);
	if (err != 0)
		goto done;

	skcka.skcka_dsname = dsname;
	skcka.skcka_cp = dcp;

	zcp_cleanup_handler_t *zch = zcp_register_cleanup(state,
	    (zcp_cleanup_t *)&zcp_synctask_change_key_cleanup, &skcka);

	err = zcp_sync_task(state, zcp_synctask_change_key_check,
	    zcp_synctask_change_key_sync, &skcka, sync, dsname);

	zcp_deregister_cleanup(state, zch);

done:
	dsl_crypto_params_free(dcp, (err != 0 || !sync) ? B_TRUE : B_FALSE);
	return (err);
}

static int
zcp_synctask_wrapper(lua_State *state)
{
	int err;
	zcp_cleanup_handler_t *zch;
	int num_ret = 1;
	nvlist_t *err_details = fnvlist_alloc();

	/*
	 * Make sure err_details is properly freed, even if a fatal error is
	 * thrown during the synctask.
	 */
	zch = zcp_register_cleanup(state,
	    (zcp_cleanup_t *)&fnvlist_free, err_details);

	zcp_synctask_info_t *info = lua_touserdata(state, lua_upvalueindex(1));
	boolean_t sync = lua_toboolean(state, lua_upvalueindex(2));

	zcp_run_info_t *ri = zcp_run_info(state);
	dsl_pool_t *dp = ri->zri_pool;

	/* MOS space is triple-dittoed, so we multiply by 3. */
	uint64_t funcspace = (info->blocks_modified << DST_AVG_BLKSHIFT) * 3;

	zcp_parse_args(state, info->name, info->pargs, info->kwargs);

	err = 0;
	if (info->space_check != ZFS_SPACE_CHECK_NONE) {
		uint64_t quota = dsl_pool_unreserved_space(dp,
		    info->space_check);
		uint64_t used = dsl_dir_phys(dp->dp_root_dir)->dd_used_bytes +
		    ri->zri_space_used;

		if (used + funcspace > quota) {
			err = SET_ERROR(ENOSPC);
		}
	}

	if (err == 0) {
		err = info->func(state, sync, err_details);
	}

	if (err == 0) {
		ri->zri_space_used += funcspace;
	}

	lua_pushnumber(state, (lua_Number)err);
	if (fnvlist_num_pairs(err_details) > 0) {
		(void) zcp_nvlist_to_lua(state, err_details, NULL, 0);
		num_ret++;
	}

	zcp_deregister_cleanup(state, zch);
	fnvlist_free(err_details);

	return (num_ret);
}

int
zcp_load_synctask_lib(lua_State *state, boolean_t sync)
{
	int i;
	zcp_synctask_info_t *zcp_synctask_funcs[] = {
		&zcp_synctask_destroy_info,
		&zcp_synctask_promote_info,
		&zcp_synctask_rollback_info,
		&zcp_synctask_snapshot_info,
		&zcp_synctask_inherit_prop_info,
		&zcp_synctask_set_prop_info,
		&zcp_synctask_change_key_info,
		NULL
	};

	lua_newtable(state);

	for (i = 0; zcp_synctask_funcs[i] != NULL; i++) {
		zcp_synctask_info_t *info = zcp_synctask_funcs[i];
		lua_pushlightuserdata(state, info);
		lua_pushboolean(state, sync);
		lua_pushcclosure(state, &zcp_synctask_wrapper, 2);
		lua_setfield(state, -2, info->name);
		info++;
	}

	return (1);
}
