if BUILD_FREEBSD
top_srcdir := $(ZFSBASE)
else
include $(top_srcdir)/config/Rules.am
endif

DEFAULT_INCLUDES += \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/libspl/include

lib_LTLIBRARIES = libzfs_core.la

USER_C = \
	libzfs_core.c
if BUILD_FREEBSD	
USER_C += \
	libzfs_core_compat.c
endif

nodist_libzfs_core_la_SOURCES = $(USER_C)

libzfs_core_la_LIBADD = \
	$(top_builddir)/lib/libnvpair/libnvpair.la \
	$(top_builddir)/lib/libuutil/libuutil.la

libzfs_core_la_LDFLAGS = -version-info 1:0:0

EXTRA_DIST = $(USER_C)

if BUILD_FREEBSD

LIB = libzfs_core
CFLAGS += -I$(top_srcdir)/lib/libzfs

objs := $(patsubst %.c,%.o, $(nodist_libzfs_core_la_SOURCES))
LDFLAGS += -lthr -lmd -lz

include $(ZFSBASE)/lib/lib/Makefile.bsd.inc

endif
