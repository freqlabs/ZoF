if BUILD_LINUX

src = $(top_srcdir)/module/avl
obj = $(abs_builddir)

MODULE := zavl

obj-$(CONFIG_ZFS) := $(MODULE).o

ccflags-y := $(ZFS_MODULE_CFLAGS) $(ZFS_MODULE_CPPFLAGS)

endif

$(MODULE)-objs += avl.o

if BUILD_FREEBSD

objs = $($(MODULE)-objs)
objs += $($(MODULE)-x86)
all: avl.ko

include $(ZFSBASE)/module/Makefile.bsd.inc

avl.ko: $(objs)
	ld -r -d -o avl.ko $(objs)

endif
