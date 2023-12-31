#!/bin/ksh
#
# CDDL HEADER START
#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.

# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#
# CDDL HEADER END
#

#
# Copyright (c) 2012, 2016 by Delphix. All rights reserved.
#

. $STF_SUITE/include/libtest.shlib

#
# DESCRIPTION:
#	Verify 'zfs list -t all -o name,origin,clones' prints the correct
#	clone information
#
# STRATEGY:
#	1. Create datasets
#	2. Create recursive snapshots and their clones
#	3. Verify zfs clones property displays right information for different
#	   cases
#

verify_runnable "both"

function local_cleanup
{
	typeset -i i=1
	for ds in $datasets; do
                datasetexists $ds/$TESTCLONE.$i && \
		    log_must zfs destroy -rf $ds/$TESTCLONE.$i
                datasetexists $ds && log_must zfs destroy -Rf $ds
		((i=i+1))
	done
}

# Set up filesystem with clones
function setup_ds
{
	typeset -i i=1
	# create nested datasets
	log_must zfs create -p $TESTPOOL/$TESTFS1/$TESTFS2/$TESTFS3

	# verify dataset creation
	for ds in $datasets; do
		datasetexists $ds || log_fail "Create $ds dataset fail."
	done

	# create recursive nested snapshot
	log_must zfs snapshot -r $TESTPOOL/$TESTFS1@snap
	for ds in $datasets; do
		datasetexists $ds@snap || \
		    log_fail "Create $ds@snap snapshot fail."
	done
	for ds in $datasets; do
		for fs in $datasets; do
			log_must zfs clone $ds@snap $fs/$TESTCLONE.$i
		done
		((i=i+1))
	done
}

# Verify clone list
function verify_clones
{
	typeset -i no_clones=$1
	typeset unexpected=$2
	typeset clone_snap=$3
	typeset -i i=1
	for ds in $datasets; do
		if [[ -n $clone_snap ]]; then
			clone_snap=/$TESTCLONE.$i
		fi
		snapshot=$(echo "$names" | grep $ds$clone_snap@snap)
		actual_clone=$(zfs list -t all -o clones $snapshot | tail -1)
		save=$IFS
		IFS=','
		typeset -a clones=()
		for token in $actual_clone; do
			clones=( "${clones[@]}" "$token" )
		done
		IFS=$save
		[[ ${#clones[*]} -ne $no_clones ]] && \
		    log_fail "$snapshot has unexpected number of clones" \
		        " ${#clones[*]}"
		expected_clone=""
		unexpected_clone=""
		if [[ $unexpected -eq 1 ]]; then
			for fs in $datasets; do
				if [[ $fs == $ds ]]; then
					if [[ -z $clone_snap ]]; then
						unexpected_clone=$fs/$TESTCLONE.$i
						(for match in ${clones[@]};do
						[[ $match != $unexpected_clone ]] && \
						    exit 0; done) || log_fail \
					            "Unexpected clones of the snapshot"
					else
						expected_clone=$fs
						unexpected_clone=$fs/$TESTCLONE.$i
						(for match in ${clones[@]};do
						[[ $match == $expected_clone ]] && \
						    [[ $match != $unexpected_clone ]] \
						    && exit 0; done) || log_fail \
						    "Unexpected clones of the snapshot"
					fi
				else
					expected_clone=$fs/$TESTCLONE.$i
					(for match in ${clones[@]};do
					[[ $match == $expected_clone ]] && \
					    exit 0; done) || log_fail \
					    "Unexpected clones of the snapshot"
				fi
			done
		else
			for fs in $datasets; do
				expected_clone=$fs/$TESTCLONE.$i
				(for match in ${clones[@]};do
				[[ $match == $expected_clone ]] && exit 0; \
				    done) || log_fail "Unexpected clones" \
				    " of the snapshot"
			done
		fi
		((i=i+1))
	done
}


log_onexit local_cleanup
datasets="$TESTPOOL/$TESTFS1 $TESTPOOL/$TESTFS1/$TESTFS2
    $TESTPOOL/$TESTFS1/$TESTFS2/$TESTFS3"

typeset -a d_clones
typeset -a deferred_snaps
typeset -i i
i=1
log_must setup_ds

log_note "Verify zfs clone propery for multiple clones"
names=$(zfs list -rt all -o name $TESTPOOL)
log_must verify_clones 3 0

log_note "verfify clone property for clone deletion"
i=1
for ds in $datasets; do
	log_must zfs destroy $ds/$TESTCLONE.$i
	((i=i+1))
done
names=$(zfs list -rt all -o name $TESTPOOL)
i=1
log_must verify_clones 2 1

log_must local_cleanup
log_must setup_ds

log_note "verify zfs deferred destroy on clones property"
i=1
names=$(zfs list -rt all -o name $TESTPOOL)
for ds in $datasets; do
	log_must zfs destroy -d $ds@snap
	deferred_snaps=( "${deferred_snaps[@]}" "$ds@snap" )
	((i=i+1))
done
log_must verify_clones 3 0

log_note "verify zfs deferred destroy by destroying clones on clones property"
d_clones=()
i=1
for ds in $datasets; do
	for fs in $datasets; do
		log_must zfs destroy $fs/$TESTCLONE.$i
		d_clones=( "${d_clones[@]}" "$fs/$TESTCLONE.$i" )
	done
	((i=i+1))
done
names=$(zfs list -rtall -o name $TESTPOOL)
for snap in ${deferred_snaps[@]}; do
	status=$(echo "$names" | grep $snap)
	[[ -z $status ]] || \
	    log_fail "$snap exist after deferred destroy"
done
for dclone in ${d_clones[@]}; do
	log_note "D CLONE = $dclone"
	status=$(echo "$names" | grep $dclone)
	[[ -z $status ]] || \
	    log_fail "$dclone exist after deferred destroy"
done

log_must local_cleanup
log_must setup_ds
log_note "verify clone property for zfs promote"
i=1
for ds in $datasets; do
	log_must zfs promote $ds/$TESTCLONE.$i
	((i=i+1))
done
names=$(zfs list -rt all -o name,clones $TESTPOOL)
log_must verify_clones 3 1 $TESTCLONE
i=1
for ds in $datasets; do
	log_must zfs promote $ds
	((i=i+1))
done
log_must local_cleanup

log_note "verify clone list truncated correctly"
typeset -i j=200
i=1
fs=$TESTPOOL/$TESTFS1
log_must zfs create $fs
log_must zfs snapshot $fs@snap
while((i < 7)); do
	log_must zfs clone $fs@snap $fs/$TESTCLONE$(printf %0.sx {1..200}).$i
	((i=i+1))
	((j=j+200))
done
clone_list=$(zfs list -o clones $fs@snap)
char_count=$(echo "$clone_list" | tail -1 | wc | awk '{print $3}')
[[ $char_count -eq 1024 ]] || \
    log_fail "Clone list not truncated correctly. Unexpected character count" \
        "$char_count"

log_pass "'zfs list -o name,origin,clones prints the correct clone information."
