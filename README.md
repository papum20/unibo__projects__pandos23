# unibo__projects__pandos23
PandOS+ project for operating systems course at University of Bologna, for the year 2022-2023.

## bin
compiled files/executables
## documentation
project specifications.
## include
headers (.h).
### include/phase*_files
headers, given for the project.
## misc
misc files for support while developing.
### misc/vscode
suggested vscode settings (for linux/debian-like).
### misc/ex
examples, exercises, stuff to make practice while developing.
## obj
object files (.o)
## src
source files (.c).
### src/phase*
source files for phase * of the project.
## tests
testers for umps3.
## Makefile
.o files go in obj/  
executables go in bin/
### make
only compiles all .c files
### make TARGET [O="DEPENDENCIES"]
with TARGET written as full path from this folder, without extension, compiles it (and creates executable).  
if specified, DEPENDENCIES are needed source files (you write like O="DEP1 DEP2 ...")
### make TARGET.o
with TARGET.o written as full path from this folder to source file, but with .o extension instead of .c, compiles it.
### make clean
clean.
## README
- README(.md): git readme.
- README.txt: plaint text readme file.
