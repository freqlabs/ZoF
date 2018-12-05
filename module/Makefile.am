if MAKE_LINUX

subdir-m += avl
subdir-m += icp
subdir-m += lua
subdir-m += nvpair
subdir-m += spl
subdir-m += unicode
subdir-m += zcommon
subdir-m += zfs

INSTALL_MOD_DIR ?= extra

ZFS_MODULE_CFLAGS += -std=gnu99 -Wno-declaration-after-statement
ZFS_MODULE_CFLAGS += @KERNEL_DEBUG_CFLAGS@
ZFS_MODULE_CFLAGS += -include @abs_top_builddir@/zfs_config.h
ZFS_MODULE_CFLAGS += -I@abs_top_srcdir@/include/spl
ZFS_MODULE_CFLAGS += -I@abs_top_srcdir@/include

ZFS_MODULE_CPPFLAGS += -D_KERNEL
ZFS_MODULE_CPPFLAGS += @KERNEL_DEBUG_CPPFLAGS@

@CONFIG_QAT_TRUE@ZFS_MODULE_CFLAGS += -I@QAT_SRC@/include
@CONFIG_QAT_TRUE@KBUILD_EXTRA_SYMBOLS += @QAT_SYMBOLS@

export ZFS_MODULE_CFLAGS ZFS_MODULE_CPPFLAGS

SUBDIR_TARGETS = icp lua

modules:
	list='$(SUBDIR_TARGETS)'; for targetdir in $$list; do \
		$(MAKE) -C $$targetdir; \
	done
	$(MAKE) -C @LINUX_OBJ@ SUBDIRS=`pwd` @KERNEL_MAKE@ CONFIG_ZFS=m $@

clean:
	@# Only cleanup the kernel build directories when CONFIG_KERNEL
	@# is defined.  This indicates that kernel modules should be built.
@CONFIG_KERNEL_TRUE@	$(MAKE) -C @LINUX_OBJ@ SUBDIRS=`pwd` @KERNEL_MAKE@ $@

	if [ -f @LINUX_SYMBOLS@ ]; then $(RM) @LINUX_SYMBOLS@; fi
	if [ -f Module.markers ]; then $(RM) Module.markers; fi

	find . -name '*.ur-safe' -type f -print | xargs $(RM)

modules_install:
	@# Install the kernel modules
	$(MAKE) -C @LINUX_OBJ@ SUBDIRS=`pwd` $@ \
		INSTALL_MOD_PATH=$(DESTDIR)$(INSTALL_MOD_PATH) \
		INSTALL_MOD_DIR=$(INSTALL_MOD_DIR) \
		KERNELRELEASE=@LINUX_VERSION@
	@# Remove extraneous build products when packaging
	kmoddir=$(DESTDIR)$(INSTALL_MOD_PATH)/lib/modules/@LINUX_VERSION@; \
	if [ -n "$(DESTDIR)" ]; then \
		find $$kmoddir -name 'modules.*' | xargs $(RM); \
	fi
	sysmap=$(DESTDIR)$(INSTALL_MOD_PATH)/boot/System.map-@LINUX_VERSION@; \
	if [ -f $$sysmap ]; then \
		depmod -ae -F $$sysmap @LINUX_VERSION@; \
	fi

modules_uninstall:
	@# Uninstall the kernel modules
	kmoddir=$(DESTDIR)$(INSTALL_MOD_PATH)/lib/modules/@LINUX_VERSION@
	list='$(subdir-m)'; for subdir in $$list; do \
		$(RM) -R $$kmoddir/$(INSTALL_MOD_DIR)/$$subdir; \
	done

distdir:
	list='$(subdir-m)'; for subdir in $$list; do \
		(cd @top_srcdir@/module && find $$subdir -name '*.c' -o -name '*.h' -o -name '*.S' |\
		xargs cp --parents -t $$distdir); \
	done

distclean maintainer-clean: clean
install: modules_install
uninstall: modules_uninstall
all: modules
check:

endif

if MAKE_FREEBSD

ZFSBASE := $(shell pwd)/..

all: zfs.ko

archives := avl/avl.ko lua/lua.ko nvpair/nvpair.ko zcommon/zcommon.ko zfs/zfs_common.ko \
	unicode/unicode.ko os/freebsd/zfs/zfs_os.ko os/freebsd/spl/spl.ko

zfs.ko: ${archives}
	ld -m elf_x86_64_fbsd -d -warn-common --build-id=sha1 -r -d -o zfs.ko ${archives}

avl/avl.ko:
	$(MAKE) -C avl
lua/lua.ko:
	$(MAKE) -C lua
nvpair/nvpair.ko:
	$(MAKE) -C nvpair
unicode/unicode.ko:
	$(MAKE) -C unicode
zcommon/zcommon.ko:
	$(MAKE) -C zcommon
zfs/zfs_common.ko:
	$(MAKE) -C zfs

os/freebsd/spl/spl.ko:
	$(MAKE) -C os/freebsd/spl

os/freebsd/zfs/zfs_os.ko:
	$(MAKE) -C os/freebsd/zfs

endif
