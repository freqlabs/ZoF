if BUILD_LINUX
make = $(MAKE) -f Makefile.linux $(AM_MAKEFLAGS)
endif

if BUILD_FREEBSD
make = MAKEFLAGS= make -f Makefile.bsd $(AM_MAKEFLAGS)
endif

all-local:
	$(make) all
check-local:
	$(make) test
clean-local:
	$(make) clean
