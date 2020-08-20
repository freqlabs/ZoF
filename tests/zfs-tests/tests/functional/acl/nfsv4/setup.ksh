#!/bin/ksh
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

#
# Portions Copyright 2007 Sun Microsystems, Inc.
# Portions Copyright 2016 Delphix
# Portions Copyright 2020 iXsystems, Inc.
#

. $STF_SUITE/include/libtest.shlib
. $STF_SUITE/tests/functional/acl/acl_common.kshlib

# Prepare the staff user and group
cleanup_user_group
log_must add_group $ZFS_ACL_STAFF_GROUP 
log_must add_user $ZFS_ACL_STAFF_GROUP $ZFS_ACL_STAFF1

# Create a pool with a filesystem mounted at $TESTDIR
DISK=${DISKS%% *}
default_setup_noexit $DISK
log_must chmod 777 $TESTDIR

# Use NFSv4 ACLs on filesystem
log_must zfs set acltype=nfsv4 $TESTPOOL/$TESTFS
log_must zfs set xattr=sa $TESTPOOL/$TESTFS

log_pass
