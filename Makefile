
## COMMAND-LINE ARGUMENTS

# DELTEST = T if the default tests should not be compiled
DELTEST = 
# T = tests to compile, in tests/
T = 


## UMPS LIBRARIES AND REQUIREMENTS

# This makefile creates a kernel file from three source files:
ifneq ($(wildcard /usr/bin/umps3),)
	UMPS3_DIR_PREFIX = /usr
	LIBDIR = $(UMPS3_DIR_PREFIX)/lib/x86_64-linux-gnu/umps3
else
	UMPS3_DIR_PREFIX = /usr/local
	LIBDIR = $(UMPS3_DIR_PREFIX)/lib/umps3
endif

INCDIR = $(UMPS3_DIR_PREFIX)/include/umps3/umps
SUPDIR = $(UMPS3_DIR_PREFIX)/share/umps3


## PROJECT'S REQUIREMENTS

BDIR = bin
IDIRS = include include/helper include/kernel include/pandos_files
IDIR = include
ODIR = obj
SDIR = src
SDIRS = $(wildcard $(SDIR)/*) # source sub-directories
SDIR1 = phase1
SDIR2 = phase2
TDIR = tests

DEPS = $(IDIR)/*.h

# __OBJS1 = pcb ash ns
# _OBJS1 = $(patsubst %, $(SDIR)/$(SDIR1)/%, $(__OBJS1))
# __OBJS2 = exceptions initial interrupts scheduler
# _OBJS2 = $(patsubst %, $(SDIR)/$(SDIR2)/%, $(__OBJS2))
# __TESTS1 = p1test
# _TESTS1 = $(patsubst %, $(TDIR)/%, $(__TESTS1))
# ifneq ($(strip $(DELTEST)), $(strip T))
# 	TESTS = $(patsubst %, $(ODIR)/%.o, $(_TESTS1))
# endif
# ifneq ($(strip $(T)),)
# 	TESTS += $(patsubst %, $(ODIR)/%.o, $(T))
# endif
# OBJS = $(patsubst %, $(ODIR)/%.o, $(_OBJS1) $(_OBJS2))

__TESTS1 = p2test.04
_TESTS1 = $(patsubst %, $(TDIR)/%, $(__TESTS1))
ifneq ($(strip $(DELTEST)), $(strip T))
	TESTS = $(patsubst %, $(ODIR)/%.o, $(_TESTS1))
endif
ifneq ($(strip $(T)),)
	TESTS += $(patsubst %, $(ODIR)/%.o, $(T))
endif

# obj directory structure copies the structure of the main folder (.)
_OBJS_NAMES = $(wildcard $(SDIR)/*/*.c)
OBJS = $(patsubst %.c, $(ODIR)/$(notdir %).o, $(_OBJS_NAMES))


## COMMANDS

DEFS = $(DEPS) Makefile

# CFLAGS suggested by project pdf
# CFLAGS = -ffreestanding -ansi -Wall -c -mips1 -mabi=32 -mfp32 \
# 	-mno-gpopt -G 0 -fno-pic -mno-abicalls -o
# this CFLAGS' options:
# std=gnu99 supports: inline, typeof
CFLAGS = -ffreestanding -std=gnu99 -Wall -c -mips1 -mabi=32 -mfp32 \
 	-mno-gpopt -G 0 -fno-pic -mno-abicalls -o
CFLAGSINC = $(patsubst %, -iquote%, $(IDIRS)) -Iinclude/ -Iinclude/pandos_files/ -I/usr/include/umps3/
LDAOUTFLAGS = -G 0 -nostdlib -T $(SUPDIR)/umpsaout.ldscript
LDCOREFLAGS = -G 0 -nostdlib -T $(SUPDIR)/umpscore.ldscript

CC = mipsel-linux-gnu-gcc
LD = mipsel-linux-gnu-ld
AS = mipsel-linux-gnu-as -KPIC
EF = umps3-elf2umps
UDEV = umps3-mkdev


# keep the .o
.PRECIOUS: $(ODIR)/*


## MAKE


# main target

.PHONY: all
all: mk_obj_dirs phase2


# main target for phase 1, using adequate test file. Compiles the nucleus structures and the test

# .PHONY: all
# all: $(BDIR)/kernel.core.umps $(BDIR)/disk0.umps
# 
# # use umps3-mkdev to create the disk0 device
# $(BDIR)/disk0.umps:
# 	$(UDEV) -d $(BDIR)/disk0.umps
# 
# # create the kernel.core.umps kernel executable file
# $(BDIR)/kernel.core.umps: $(BDIR)/kernel
# 	$(EF) -k $(BDIR)/kernel
# 
# $(BDIR)/kernel: $(OBJS) $(TESTS)
# 	$(LD) $(LDCOREFLAGS) $(LIBDIR)/crtso.o $(OBJS) $(TESTS) \
# 		$(LIBDIR)/libumps.o -o $(BDIR)/kernel
# 
# #objects
# $(ODIR)/%.o: %.c $(DEFS) 
# 	$(CC) $(CFLAGS) $@ $< $(CFLAGSINC)


# compile the nucleus (phase 2)

.PHONY: phase2
phase2: $(BDIR)/kernel.core.umps $(BDIR)/disk0.umps

# use umps3-mkdev to create the disk0 device
$(BDIR)/disk0.umps:
	$(UDEV) -d $(BDIR)/disk0.umps

# create the kernel.core.umps kernel executable file
$(BDIR)/kernel.core.umps: $(BDIR)/kernel
	$(EF) -k $(BDIR)/kernel

$(BDIR)/kernel: $(OBJS) $(TESTS)
	$(LD) $(LDCOREFLAGS) $(LIBDIR)/crtso.o $(OBJS) $(TESTS) \
		$(LIBDIR)/libumps.o -o $(BDIR)/kernel

#objects
$(ODIR)/%.o: %.c $(DEFS) 
	$(CC) $(CFLAGS) $@ $< $(CFLAGSINC)


# create src directories structure in obj dir

.PHONY: mk_obj_dirs
mk_obj_dirs:
	@mkdir -p $(patsubst %, $(ODIR)/%, $(SDIRS) $(TDIR)) $(BDIR)


# show help screen

.PHONY: help
help: 
	@echo "make [DELTEST=T] [T=TEST_BASENAME]"
	@echo "make			: make mk_obj_dirs && make phase2"
	@echo "DELTEST=T		: don't compile default test (tests/p2test.04.c)"
	@echo "T=TEST_BASENAME		: compile TEST_BASENAME, i.e. test's relative path without extension"
	@echo "			e.g.: make DELTEST=T T=tests/p2test.03"
	@echo "make phase2		: compile the nucleus (phase1+phase2) and TESTS (=p2test.04.c)"
	@echo "make mk_obj_dirs	: create output dirs tree (obj/*)"
	@echo "make help		: show this help message"
	@echo "make clean		: clean"


# sets the tests to compile, if specified as make command-line arguments

	


# CLEAN
.PHONY: clean
clean:
	rm $(wildcard $(ODIR)/$(SDIR)/*/*.o) \
		$(ODIR)/$(TDIR)/*.o $(BDIR)/$(filter-out .placeholder, *)