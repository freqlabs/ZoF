if BUILD_FREEBSD
top_srcdir := $(ZFSBASE)
endif

include $(top_srcdir)/config/Rules.am

VPATH = \
	$(top_srcdir)/lib/libspl \
	$(top_srcdir)/lib/libspl/$(TARGET_ASM_DIR)

SUBDIRS = include $(TARGET_ASM_DIR)
DIST_SUBDIRS = include asm-generic asm-i386 asm-x86_64

DEFAULT_INCLUDES += \
	-I$(top_srcdir)/lib/libspl/include

AM_CCASFLAGS = \
	$(CFLAGS)

noinst_LTLIBRARIES = libspl.la

USER_C = \
	getexecname.c \
	gethostid.c \
	list.c \
	mkdirp.c \
	page.c \
	strlcat.c \
	strlcpy.c \
	strnlen.c \
	timestamp.c \
	zone.c
if BUILD_LINUX
USER_C += \
	getmntany.c \
	include/sys/list.h \
	include/sys/list_impl.h
endif

if BUILD_LINUX
USER_ASM = atomic.S
endif
if BUILD_FREEBSD
USER_ASM = asm-x86_64/atomic.S
endif

nodist_libspl_la_SOURCES = \
	$(USER_C) \
if BUILD_LINUX
	$(USER_ASM)
endif

libspl_la_LIBADD = -lrt

EXTRA_DIST = $(USER_C)

if BUILD_FREEBSD

LIB = libspl

objs := $(patsubst %.c,%.o, $(nodist_libspl_la_SOURCES))
asm_objs := $(patsubst %.S,%.o, $(USER_ASM))

LDFLAGS += -lthr -lmd -lz

include $(top_srcdir)/lib/Makefile.bsd.inc

endif
