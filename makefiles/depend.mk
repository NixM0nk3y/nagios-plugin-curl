# provides generic rules for C/C++ dependeny generation using
# 'makedepend', 'gcc -MM' or similar mechanisms
#
# requires:
# - compilers CC and CCPP
# - INCLUDEDIRS
# - OBJS, CPP_OBJS and BIN_OBJS
#
# provides:
# - included dependency files
#
# author: Andreas Baumann, abaumann at yahoo dot com

MAKEDEPEND = $(CC) -MM $(INCLUDE_DIRS)
CCPP_MAKEDEPEND = $(CCPP) -MM $(INCLUDE_DIRS)

%.d : %.c
	@echo Generating dependencies for $<
	@$(MAKEDEPEND) $(CFLAGS) $< | \
		sed "s,\($*\.o\)[ :]*\(.*\),$@ : $$\(wildcard \2\)\&\&\&\1 : \2,g" | tr -s '&' "\n" > $@

%.d : %.cpp
	@echo Generating dependencies for $<
	@$(CCPP_MAKEDEPEND) $(CCPPFLAGS) $< | \
		sed "s,\($*\.o\)[ :]*\(.*\),$@ : $$\(wildcard \2\)\&\&\&\1 : \2,g" | tr -s '&' "\n" > $@

-include $(OBJS:.o=.d)
-include $(CPP_OBJS:.o=.d)
-include $(BIN_OBJS:.o=.d)
-include $(CPP_BIN_OBJS:.o=.d)
