#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/errno.h>
#include <sys/uio.h>
#include <sys/buf.h>
#include <sys/file.h>
#include <sys/kmem.h>
#include <sys/conf.h>
#include <sys/cmn_err.h>
#include <sys/stat.h>
#include <sys/zfs_ioctl.h>
#include <sys/zfs_vfsops.h>
#include <sys/zfs_znode.h>
#include <sys/zap.h>
#include <sys/spa.h>
#include <sys/spa_impl.h>
#include <sys/vdev.h>
#include <sys/dmu.h>
#include <sys/dsl_dir.h>
#include <sys/dsl_dataset.h>
#include <sys/dsl_prop.h>
#include <sys/dsl_deleg.h>
#include <sys/dmu_objset.h>
#include <sys/dmu_impl.h>
#include <sys/dmu_tx.h>
#include <sys/sunddi.h>
#include <sys/policy.h>
#include <sys/zone.h>
#include <sys/nvpair.h>
#include <sys/mount.h>
#include <sys/taskqueue.h>
#include <sys/sdt.h>
#include <sys/varargs.h>
#include <sys/fs/zfs.h>
#include <sys/zfs_ctldir.h>
#include <sys/zfs_dir.h>
#include <sys/zfs_onexit.h>
#include <sys/zvol.h>
#include <sys/dsl_scan.h>
#include <sys/dmu_objset.h>
#include <sys/dmu_send.h>
#include <sys/dsl_destroy.h>
#include <sys/dsl_bookmark.h>
#include <sys/dsl_userhold.h>
#include <sys/zfeature.h>
#include <sys/zcp.h>
#include <sys/zio_checksum.h>
#include <sys/vdev_removal.h>
#include <sys/dsl_crypt.h>

#include <zfs_ioctl_compat.h>

#include <sys/arc_impl.h>

extern arc_state_t ARC_anon;
extern arc_state_t ARC_mru;
extern arc_state_t ARC_mru_ghost;
extern arc_state_t ARC_mfu;
extern arc_state_t ARC_mfu_ghost;
extern arc_state_t ARC_l2c_only;

/* L2ARC Performance Tunables */
extern unsigned long l2arc_write_max;	/* def max write size */
extern unsigned long l2arc_write_boost;	/* extra warmup write */
extern unsigned long l2arc_headroom;		/* # of dev writes */
extern unsigned long l2arc_headroom_boost;
extern unsigned long l2arc_feed_secs;	/* interval seconds */
extern unsigned long l2arc_feed_min_ms;	/* min interval msecs */
extern int l2arc_noprefetch;			/* don't cache prefetch bufs */
extern int l2arc_feed_again;			/* turbo warmup */
extern int l2arc_norw;			/* no reads during writes */

/*
 * minimum lifespan of a prefetch block in clock ticks
 * (initialized in arc_init())
 */
extern int		arc_min_prefetch_ms;
extern int		arc_min_prescient_prefetch_ms;


SYSCTL_DECL(_vfs_zfs);
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2arc_write_max, CTLFLAG_RW,
    &l2arc_write_max, 0, "max write size");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2arc_write_boost, CTLFLAG_RW,
    &l2arc_write_boost, 0, "extra write during warmup");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2arc_headroom, CTLFLAG_RW,
    &l2arc_headroom, 0, "number of dev writes");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2arc_feed_secs, CTLFLAG_RW,
    &l2arc_feed_secs, 0, "interval seconds");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2arc_feed_min_ms, CTLFLAG_RW,
    &l2arc_feed_min_ms, 0, "min interval milliseconds");

SYSCTL_INT(_vfs_zfs, OID_AUTO, l2arc_noprefetch, CTLFLAG_RW,
    &l2arc_noprefetch, 0, "don't cache prefetch bufs");
SYSCTL_INT(_vfs_zfs, OID_AUTO, l2arc_feed_again, CTLFLAG_RW,
    &l2arc_feed_again, 0, "turbo warmup");
SYSCTL_INT(_vfs_zfs, OID_AUTO, l2arc_norw, CTLFLAG_RW,
    &l2arc_norw, 0, "no reads during writes");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, anon_size, CTLFLAG_RD,
    &ARC_anon.arcs_size.rc_count, 0, "size of anonymous state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, anon_metadata_esize, CTLFLAG_RD,
    &ARC_anon.arcs_esize[ARC_BUFC_METADATA].rc_count, 0,
    "size of anonymous state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, anon_data_esize, CTLFLAG_RD,
    &ARC_anon.arcs_esize[ARC_BUFC_DATA].rc_count, 0,
    "size of anonymous state");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_size, CTLFLAG_RD,
    &ARC_mru.arcs_size.rc_count, 0, "size of mru state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_metadata_esize, CTLFLAG_RD,
    &ARC_mru.arcs_esize[ARC_BUFC_METADATA].rc_count, 0,
    "size of metadata in mru state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_data_esize, CTLFLAG_RD,
    &ARC_mru.arcs_esize[ARC_BUFC_DATA].rc_count, 0,
    "size of data in mru state");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_ghost_size, CTLFLAG_RD,
    &ARC_mru_ghost.arcs_size.rc_count, 0, "size of mru ghost state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_ghost_metadata_esize, CTLFLAG_RD,
    &ARC_mru_ghost.arcs_esize[ARC_BUFC_METADATA].rc_count, 0,
    "size of metadata in mru ghost state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mru_ghost_data_esize, CTLFLAG_RD,
    &ARC_mru_ghost.arcs_esize[ARC_BUFC_DATA].rc_count, 0,
    "size of data in mru ghost state");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_size, CTLFLAG_RD,
    &ARC_mfu.arcs_size.rc_count, 0, "size of mfu state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_metadata_esize, CTLFLAG_RD,
    &ARC_mfu.arcs_esize[ARC_BUFC_METADATA].rc_count, 0,
    "size of metadata in mfu state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_data_esize, CTLFLAG_RD,
    &ARC_mfu.arcs_esize[ARC_BUFC_DATA].rc_count, 0,
    "size of data in mfu state");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_ghost_size, CTLFLAG_RD,
    &ARC_mfu_ghost.arcs_size.rc_count, 0, "size of mfu ghost state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_ghost_metadata_esize, CTLFLAG_RD,
    &ARC_mfu_ghost.arcs_esize[ARC_BUFC_METADATA].rc_count, 0,
    "size of metadata in mfu ghost state");
SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, mfu_ghost_data_esize, CTLFLAG_RD,
    &ARC_mfu_ghost.arcs_esize[ARC_BUFC_DATA].rc_count, 0,
    "size of data in mfu ghost state");

SYSCTL_UQUAD(_vfs_zfs, OID_AUTO, l2c_only_size, CTLFLAG_RD,
    &ARC_l2c_only.arcs_size.rc_count, 0, "size of mru state");

SYSCTL_UINT(_vfs_zfs, OID_AUTO, arc_min_prefetch_ms, CTLFLAG_RW,
    &arc_min_prefetch_ms, 0, "Min life of prefetch block in ms");
SYSCTL_UINT(_vfs_zfs, OID_AUTO, arc_min_prescient_prefetch_ms, CTLFLAG_RW,
    &arc_min_prescient_prefetch_ms, 0, "Min life of prescient prefetched block in ms");
