
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
OBJ = $(patsubst %, %.o, $(O))



#########
# RULES #
#########

.PRECIOUS: $(ODIR)/%.o

# MAKE COMMAND LINE ARGUMENT

misc/%: $(OBJ) misc/%.o
	$(warning ARG: $@)
	$(CC) -o $(BDIR)/$(notdir $@) $(patsubst %.o, $(ODIR)/%.o, $(notdir $^)) $(CFLAGS)

src/%: src/%.o $(OBJ)
	$(warning ARG: $@)
	$(CC) -o $(BDIR)/$(notdir $@) $(patsubst %.o, $(ODIR)/%.o, $(notdir $^)) $(CFLAGS)

# MAKE .o

%.o: %.c $(DEPS)
	$(warning OBJ: $@)
	$(CC) -c -o $(ODIR)/$(notdir $@) $< $(CFLAGS)

# MAKE all

.PHONY: all
_OBJALL != find . | grep -v .git | grep -F .c
OBJALL = $(patsubst %.c, %.o, $(_OBJALL))
all: $(OBJALL)
	$(warning (ONLY BUILD) ALL: $@)
	$(OBJALL)
	$(warning (ONLY BUILD) ALL: $@)

#panda1: $(OBJ)
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)



#########
# CLEAN #
#########

.PHONY: clean
clean:
	rm $(ODIR)/$(filter-out $(NOCLEAN), *.o) $(BDIR)/$(filter-out $(NOCLEAN), *)