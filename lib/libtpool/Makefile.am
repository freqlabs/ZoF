if BUILD_FREEBSD
top_srcdir := $(ZFSBASE)
endif

include $(top_srcdir)/config/Rules.am

DEFAULT_INCLUDES += \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/libspl/include

noinst_LTLIBRARIES = libtpool.la

USER_C = \
	thread_pool.c
if BUILD_LINUX
USER_C += \
	thread_pool_impl.h
endif

nodist_libtpool_la_SOURCES = $(USER_C)

libtpool_la_LIBADD = \
	$(top_builddir)/lib/libspl/libspl.la

libtpool_la_LDFLAGS = -pthread

EXTRA_DIST = $(USER_C)

if BUILD_FREEBSD

lib = libtpool

objs := $(patsubst %.c,%.o, $(nodist_libtpool_la_SOURCES))
LDFLAGS += -lthr -lmd -lz

include $(top_srcdir)/lib/Makefile.bsd.inc

endif
