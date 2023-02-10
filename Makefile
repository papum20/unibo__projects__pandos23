
# DIRS

# headers (.h) dir
IDIR = include
# object files (.o) dir
ODIR = obj
# binaries/executables dir
BDIR = bin
# libraries dir
#LDIR = lib


# COMMANDS

CC = gcc
CFLAGS = -iquote$(IDIR)
# gcc parameters for including libraries
#LIBS = lib


# dependancies
_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# object (.o) files
_OBJ = 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))



$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

panda1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)



.PHONY: clean
clean:
	rm $(ODIR)/*.o $(BDIR)/*