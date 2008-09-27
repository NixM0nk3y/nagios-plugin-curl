# sets  e. g. to LINUX, OS_MAJOR_VERSION to 2 and OS_MINOR_VERSION to 6
# by calling the 'guess_env' shell script, where the actual probing happens
# Also sets PLATFORM_COMPILE_FLAGS to be included when compiling C/C++ code
#
# requires:
# - TOPDIR
#
# provides:
# - PLATFORM
# - OS_MAJOR_VERSION
# - OS_MINOR_VERSION
# - PLATFORM_COMPILE_FLAGS
# - EXE
# - SO
#
# author: Andreas Baumann, abaumann at yahoo dot com

PLATFORM =		$(shell $(TOPDIR)/makefiles/guess_env --platform)
OS_MAJOR_VERSION =	$(shell $(TOPDIR)/makefiles/guess_env --os-major-version)
OS_MINOR_VERSION =	$(shell $(TOPDIR)/makefiles/guess_env --os-minor-version)

PLATFORM_COMPILE_FLAGS = \
			-D$(PLATFORM) \
			-DOS_MAJOR_VERSION=$(OS_MAJOR_VERSION) \
			-DOS_MINOR_VERSION=$(OS_MINOR_VERSION)

LIBS_DL = $(shell $(TOPDIR)/makefiles/guess_env --libs-dl)
LIBS_SSP = $(shell $(TOPDIR)/makefiles/guess_env --libs-ssl)

EXE =
SO = .so

GCC_MAJOR_VERSION = $(shell $(TOPDIR)/makefiles/guess_env --gcc-major-version $(CC))
GCC_MINOR_VERSION = $(shell $(TOPDIR)/makefiles/guess_env --gcc-minor-version $(CC))
