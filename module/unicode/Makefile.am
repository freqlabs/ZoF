if BUILD_LINUX

src = $(top_srcdir)/module/unicode
obj = $(top_builddir)

MODULE := zunicode

obj-$(CONFIG_ZFS) := $(MODULE).o

ccflags-y := $(ZFS_MODULE_CFLAGS) $(ZFS_MODULE_CPPFLAGS)

endif

$(MODULE)-objs += u8_textprep.o
$(MODULE)-objs += uconv.o

if BUILD_FREEBSD

objs = $($(MODULE)-objs)
objs += $($(MODULE)-x86)
all: unicode.ko

include $(ZFSBASE)/module/Makefile.bsd.inc

unicode.ko: $(objs)
	ld -r -d -o unicode.ko $(objs)

endif
