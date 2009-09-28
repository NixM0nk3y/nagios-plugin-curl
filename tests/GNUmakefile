TOPDIR = ..

SUBDIRS =

INCLUDE_DIRS =

LDFLAGS_DIR =

LIBS_DIR =
	
BINS =

OBJS =

-include $(TOPDIR)/makefiles/sub.mk

local_all:
	
local_clean:

local_distclean:
	
local_test:
	-$(TOPDIR)/src/check_curl --config-file clientcertcheck
	-$(TOPDIR)/src/check_curl --config-file connect_failure
	-$(TOPDIR)/src/check_curl --config-file DNS_failure
	-$(TOPDIR)/src/check_curl --config-file iis
	-$(TOPDIR)/src/check_curl --config-file not_found
	-$(TOPDIR)/src/check_curl --config-file openssl.org
	-$(TOPDIR)/src/check_curl --config-file PortalZH
	-$(TOPDIR)/src/check_curl --config-file useragent
