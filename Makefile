
########
# DIRS #
########

# headers (.h) dir
IDIR = include
# source (.c) dir
SDIR = src
# object files (.o) dir
ODIR = obj
# binaries/executables dir
BDIR = bin
# libraries dir
#LDIR = lib

# CLEAN EXCEPTIONS
NOCLEAN = .placeholder


############
# COMMANDS #
############

CC = gcc
CFLAGS = -iquote$(IDIR)
# gcc parameters for including libraries
#LIBS = lib



################
# DEPENDENCIES #
################

# dependencies
_DEPS = *
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

# command line object files .o
O = 
# object (.o) files
_OBJ = $(O)
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))



#########
# RULES #
#########

# MAKE COMMAND LINE ARGUMENT
FOO:=$(DIOCANE)
NOWANTS = $(wildcard *.o)
$(filter-out $(NOWANTS), %): %.o
	echo "arg"
	echo $(OBJ)
	echo $(DEPS)
	echo $<
#	$(CC) -c -o $@ $< $(CFLAGS)

# MAKE .o

%.o: $(SDIR)/%.c $(DEPS)
	echo "o"
#	$(CC) -c -o $(ODIR)/$@ $< $(CFLAGS)

# MAKE all

.PHONY: all
all: $(OBJ)
	echo "all"
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#panda1: $(OBJ)
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)



#########
# CLEAN #
#########

.PHONY: clean
clean:
	rm $(ODIR)/$(filter-out $(NOCLEAN), *.o) $(BDIR)/$(filter-out $(NOCLEAN), *)