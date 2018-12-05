if BUILD_LINUX

src = $(top_srcdir)/module/nvpair
obj = $(top_builddir)

MODULE := znvpair

obj-$(CONFIG_ZFS) := $(MODULE).o

ccflags-y := $(ZFS_MODULE_CFLAGS) $(ZFS_MODULE_CPPFLAGS)

endif

$(MODULE)-objs += nvpair.o
$(MODULE)-objs += fnvpair.o
$(MODULE)-objs += nvpair_alloc_spl.o
$(MODULE)-objs += nvpair_alloc_fixed.o

if BUILD_FREEBSD

objs = $($(MODULE)-objs)
objs += $($(MODULE)-x86)
all: nvpair.ko

include $(ZFSBASE)/module/Makefile.bsd.inc

nvpair.ko: $(objs)
	ld -r -d -o $@ $(objs)

endif
