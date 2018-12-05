if BUILD_FREEBSD
top_srcdir := $(ZFSBASE)
else
include $(top_srcdir)/config/Rules.am
endif

VPATH = $(top_srcdir)/module/avl/

# Includes kernel code, generate warnings for large stack frames
AM_CFLAGS += $(FRAME_LARGER_THAN)

DEFAULT_INCLUDES += \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/libspl/include

noinst_LTLIBRARIES = libavl.la

USER_C =

KERNEL_C = \
	avl.c

nodist_libavl_la_SOURCES = \
	$(USER_C) \
	$(KERNEL_C)

EXTRA_DIST = $(USER_C)

if BUILD_FREEBSD

LIB = libavl

objs := $(patsubst %.c,%.o, $(nodist_libavl_la_SOURCES))
LDFLAGS += -lthr -lmd -lz

include $(top_srcdir)/lib/Makefile.bsd.inc

endif
