# makefile for a sub package
#
# requires:
# - TOPDIR
# - SUBDIRS
# - INCLUDE_DIRS
#
# provides:
# - target: all targets

-include $(TOPDIR)/makefiles/platform.mk
-include $(TOPDIR)/makefiles/compiler.mk

.PHONY: all $(SUBDIRS) local_all
all: local_all $(OBJS) $(CPPOBJS) $(BIN_OBJS) $(CPP_BIN_OBJS) $(BINS) $(CPP_BINS) $(CMODULES) $(CPPMODULES)
	@test -z "$(SUBDIRS)" || ( set -e; for d in $(SUBDIRS)""; do \
	  (set -e; $(MAKE) -C $$d all || exit 1); done)

.PHONY: test local_test
test: local_test $(OBJS) $(CPPOBJS) $(BIN_OBJS) $(BINS) $(CPP_BINS) $(CMODULES) $(CPPMODULES)
	@test -z "$(SUBDIRS)" || ( set -e; for d in $(SUBDIRS)""; do \
	  (set -e; $(MAKE) -C $$d test || exit 1); done)

-include $(TOPDIR)/makefiles/depend.mk
-include $(TOPDIR)/makefiles/clean.mk
