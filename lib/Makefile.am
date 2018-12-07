# NB: GNU Automake Manual, Chapter 8.3.5: Libtool Convenience Libraries
# These six libraries are intermediary build components.
SUBDIRS = libavl libicp libspl libtpool libunicode
if BUILD_LINUX
SUBDIRS+= libefi
endif

# These five libraries, which are installed as the final build product,
# incorporate the six convenience libraries given above.
SUBDIRS += libuutil libnvpair libzpool libzfs_core libzfs
