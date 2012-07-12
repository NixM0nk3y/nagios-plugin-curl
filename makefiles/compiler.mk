# sets compiler settings
#
# requires:
# - INCLUDE_DIRS
#
# provides:
# - BIN_OBJS: the object files we need for the binaries (containing the main)
#

# -Wswitch-default: not good for switches with enums
# -Wsystem-headers: bad idea, as header files are usually happily broken :-)
# -Wtraditional: we don't want to program tradition K&R C anymore!
# -Wunsafe-loop-optimizations: ??
# -Wno-attributes, -Wmissing-format-attribute: ?? later
# -Wpacked -Wpadded: ?? very questionable
# -Wunreachable-code: doesn't work
# -Wno-div-by-zero: we get NaN and friend over macros, so need for funny tricks :-)
# -Wstrict-overflow=5 is relatively new, later maybe
# -fstack-protector or -fstack-protector-all: should be used, but U
#  have currently big problems to get it around compiler gcc and -lssl
#  probing! FIXME later
# -fstack-protector-all: does something funny to the shared objects..
# -Wstack-protector makes no sense without SSP
# everything implied by -Wall is not explicitly specified (gcc 4.2.3)
# -Waggregate-return: is for K&R code and mostly useless nowadays

#-Werror

# compilation flags and compilers
COMMON_COMPILE_FLAGS = \
	-g -D_REENTRANT \
	-fstrict-aliasing \
	-pedantic -Wall \
	-Wunused -Wno-import \
	-Wformat -Wformat-y2k -Wformat-nonliteral -Wformat-security -Wformat-y2k \
	-Wswitch-enum -Wunknown-pragmas -Wfloat-equal \
	-Wundef -Wshadow -Wpointer-arith \
	-Wcast-qual -Wcast-align \
	-Wwrite-strings -Wconversion \
	-Wmissing-noreturn \
	-Wno-multichar -Wparentheses -Wredundant-decls \
	-Winline \
	-Wdisabled-optimization \
	$(INCLUDE_DIRS)

ifeq "$(GCC_MAJOR_VERSION)" "4"
COMMON_COMPILE_FLAGS += \
	-Wfatal-errors -Wmissing-include-dirs -Wvariadic-macros \
	-Wvolatile-register-var \
	-Wstrict-aliasing=2 -Wextra -Winit-self
endif

ifeq "$(GCC_MAJOR_VERSION)" "3"

# gcc 3.3, testend on OpenBSD 4.2
ifeq "$(GCC_MINOR_VERSION)" "3"
COMMON_COMPILE_FLAGS += \
	-W
endif

# gcc 3.4, not tested yet
ifeq "$(GCC_MINOR_VERSION)" "4"
COMMON_COMPILE_FLAGS += \
	-Wstrict-aliasing=2 -Wextra -Winit-self
endif

endif

COMPILE_FLAGS = \
	$(COMMON_COMPILE_FLAGS) \
	-std=c99 \
	-Wnonnull \
	-Wbad-function-cast -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs

# gcc 4.x
ifeq "$(GCC_MAJOR_VERSION)" "4"
COMPILE_FLAGS += \
	-Wc++-compat -Wdeclaration-after-statement -Wold-style-definition
endif

ifeq "$(GCC_MAJOR_VERSION)" "3"

# gcc 3.4, not tested yet
ifeq "$(GCC_MINOR_VERSION)" "4"
COMPILE_FLAGS += \
	-Wdeclaration-after-statement -Wold-style-definition
endif

# gcc 3.3, testend on OpenBSD 4.2
ifeq "$(GCC_MINOR_VERSION)" "3"
#COMPILE_FLAGS += \
#	-Wdeclaration-after-statement
endif


endif

CCPP_COMPILE_FLAGS = \
	$(COMMON_COMPILE_FLAGS) \
	-std=c++98

# gcc 4.x
ifeq "$(GCC_MAJOR_VERSION)" "4"
CCPP_COMPILE_FLAGS += \
	-Wno-invalid-offsetof
endif

ifeq "$(GCC_MAJOR_VERSION)" "3"

# gcc 3.4, not tested yet
ifeq "$(GCC_MINOR_VERSION)" "4"
CCPP_COMPILE_FLAGS += \
	-Wno-invalid-offsetof
endif

# gcc 3.3, testend on OpenBSD 4.2
ifeq "$(GCC_MINOR_VERSION)" "3"
#CCPP_COMPILE_FLAGS += \
#	-Wdeclaration-after-statement
endif

endif

CFLAGS = $(COMPILE_FLAGS) $(PLATFORM_COMPILE_FLAGS) $(DEBUGLEVELFLAGS)
CCPPFLAGS = $(CCPP_COMPILE_FLAGS) $(PLATFORM_COMPILE_FLAGS) $(DEBUGLEVELFLAGS)
CC = gcc
CCPP = g++

LDFLAGS = $(LDFLAGS_DIR)
LIBS = $(LIBS_DL) $(LIBS_SSP) $(LIBS_DIR)
LINK = $(CC)
CCPP_LINK = $(CCPP)

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.o : %.cpp
	$(CCPP) -c -o $@ $(CCPPFLAGS) $<

%$(EXE): %.o $(OBJS)
	$(LINK) -o $@ $(LIBS) $(OBJS) $<

%.sho : %.c
	$(CC) -c -o $@ -fPIC -DSHARED $(CFLAGS) $<

%$(SO) : %.sho $(OBJS)
	$(LINK) -shared -o $@ $(LDFLAGS) $(LIBS) $(OBJS) $<

%.sho++ : %.cpp
	$(CCPP) -c -o $@ -fPIC -DSHARED $(CCPPFLAGS) $<

%$(SO) : %.sho++ $(OBJS) $(CPPOBJS)
	$(CCPP_LINK) -shared -o $@ $(LDFLAGS) $(LIBS) $(OBJS) $(CPPOBJS) $<
 
BIN_OBJS = $(BINS:$(EXE)=.o) 
CPP_BIN_OBJS = $(CPP_BINS:$(EXE)=.o)
