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
# Portions Copyright 2020 iXsystems, Inc.
#

. $STF_SUITE/include/libtest.shlib
. $STF_SUITE/tests/functional/acl/acl_common.kshlib
. $STF_SUITE/tests/functional/acl/nfsv4/nfsv4_common.kshlib

#
# DESCRIPTION:
#	Verify Allow ACEs are functional for acltype=nfsv4.
#
# STRATEGY:
#	1. Create directory with one subdirectory and one file
#	2. grant everyone execute(x) on directory
#	3. Remove all ACL entries from test file.
#	4. Iterate through NFSv4 permissions bits. Grant them
#	   one at a time and verify the following as limited user:
#	   a) Permission in question is granted
#	   b) No additional permissions are granted
#	5. Repeat step (4) with subdirectory.
#
