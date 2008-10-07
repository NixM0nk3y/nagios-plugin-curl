TOPDIR = ..

SUBDIRS =

INCLUDE_DIRS = \
	-I. `curl-config --cflags`

LDFLAGS_DIR =

LIBS_DIR = \
	`curl-config --libs`
	
BINS = \
	check_curl

OBJS = \
	utils.o \
	curlhelper.o \
	cmdline.o

-include $(TOPDIR)/makefiles/sub.mk

# ABa: currently a special rule for cmdline.c as gengetopt is not
# completly fixed yet
check_curl.d : cmdline.h
cmdline.c : check_curl.ggo
	gengetopt --unamed-opts --include-getopt --conf-parser -i $<

cmdline.o : cmdline.c cmdline.h
	$(CC) -c -o $@ $<

local_all: cmdline.h
	
local_clean:

local_distclean:
	-@rm cmdline.c cmdline.h
	
local_test:
