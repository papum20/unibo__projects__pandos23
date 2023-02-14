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
IDIRS = include include/helper include/phase1_files
IDIR = include
ODIR = obj
SDIR = src

DEPS = $(IDIR)/*.h

_OBJS = phase1/pcb phase1/ash phase1/ns
OBJS = $(patsubst %, $(ODIR)/%.o, $(_OBJS))




## COMMANDS

DEFS = $(DEPS) $(INCDIR)/libumps.h Makefile

CFLAGS = -ffreestanding -ansi -Wall -c -mips1 -mabi=32 -mfp32 \
	-mno-gpopt -G 0 -fno-pic -mno-abicalls -o
CFLAGSINC = $(patsubst %, -iquote%, $(IDIRS))
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

#main target
all: $(BDIR)/kernel.core.umps $(BDIR)/disk0.umps

# use umps3-mkdev to create the disk0 device
$(BDIR)/disk0.umps:
	$(UDEV) -d $(BDIR)/disk0.umps

# create the kernel.core.umps kernel executable file
$(BDIR)/kernel.core.umps: $(BDIR)/kernel
	$(EF) -k $(BDIR)/kernel

$(BDIR)/kernel: $(OBJS)
	$(LD) $(LDCOREFLAGS) $(LIBDIR)/crtso.o $(OBJS) \
		$(LIBDIR)/libumps.o -o $(BDIR)/kernel

%.o: src/phase1/pcb.c $(DEFS)
	$(CC) $(CFLAGS) $@ $< $(CFLAGSINC)