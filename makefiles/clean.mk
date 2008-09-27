# cleans up directories
#
# requires:
# - BINS, OBJS, CPPOBJS, BIN_OBJS
# - CMODULES, CPPMODULES
# - SUBDIRS
#
# provides:
# - target: clean
# - target: distclean

.PHONY: clean_recursive clean local_clean

clean_recursive:
	@test -z "$(SUBDIRS)" || ( set -e; for d in $(SUBDIRS)""; do \
	  (set -e; $(MAKE) -C $$d clean || exit 1); done)

clean: clean_recursive local_clean
	-@rm *.bak 2>/dev/null
	-@rm *~ 2>/dev/null
	-@rm *.d port/*.d 2>/dev/null
	-@rm $(BINS) $(CPP_BINS) 2>/dev/null
	-@rm $(OBJS) $(CPPOBJS) $(BIN_OBJS) $(CPP_BIN_OBJS) 2>/dev/null
	-@rm exec/*
	-@rm *.core
	-@rm $(CMODULES) $(CPPMODULES)
	-@rm $(CMODULES .o=.d) $(CPPMODULES .o=.d)

.PHONY: distclean_recursive distclean local_distclean

distclean_recursive:
	@test -z "$(SUBDIRS)" || ( set -e; for d in $(SUBDIRS)""; do \
	  (set -e; $(MAKE) -C $$d distclean || exit 1); done)

distclean: distclean_recursive local_distclean clean
