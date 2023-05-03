
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
SDIR1 = phase1
SDIR2 = phase2
TDIR = tests

DEPS = $(IDIR)/*.h

__OBJS1 = pcb ash ns
_OBJS1 = $(patsubst %, $(SDIR)/$(SDIR1)/%, $(__OBJS1))
__TESTS = p1test
_TESTS = $(patsubst %, $(TDIR)/%, $(__TESTS))
ifneq ($(strip $(DELTEST)), $(strip T))
	TESTS = $(patsubst %, $(ODIR)/%.o, $(_TESTS))
endif
ifneq ($(strip $(T)),)
	TESTS += $(patsubst %, $(ODIR)/%.o, $(T))
endif
OBJS = $(patsubst %, $(ODIR)/%.o, $(_OBJS1))




## COMMANDS

DEFS = $(DEPS) $(INCDIR)/libumps.h Makefile

# CFLAGS suggested by project pdf
# CFLAGS = -ffreestanding -ansi -Wall -c -mips1 -mabi=32 -mfp32 \
# 	-mno-gpopt -G 0 -fno-pic -mno-abicalls -o
# this CFLAGS' options:
# std=gnu99 supports: inline, typeof
CFLAGS = -ffreestanding -std=gnu99 -Wall -c -mips1 -mabi=32 -mfp32 \
 	-mno-gpopt -G 0 -fno-pic -mno-abicalls -o
CFLAGSINC = $(patsubst %, -iquote%, $(IDIRS)) -Iinclude/pandos_files/
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
all: $(BDIR)/kernel.core.umps $(BDIR)/disk0.umps

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


# show help screen

.PHONY: help
help: 
	@echo "make [DELTEST={T}] [T=paths]"
	@echo "make			: compile default tests"
	@echo "DELTEST=T		: don't compile default tests"
	@echo "DELTEST={anythig else}	: doesn't do anything"
	@echo "T=paths-to-tests	: also compiles specified tests,"
	@echo "			  to write with path, without extension"
	@echo "make help		: show this help message"
	@echo "make clean		: clean"


# sets the tests to compile, if specified as make command-line arguments

	


# CLEAN
.PHONY: clean
clean:
	rm $(ODIR)/$(SDIR)/$(SDIR1)/*.o $(ODIR)/$(SDIR)/$(SDIR2)/*.o \
		$(ODIR)/$(TDIR)/*.o $(BDIR)/$(filter-out .placeholder, *)