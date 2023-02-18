# unibo__projects__pandos23
PandOS+ project for operating systems course at University of Bologna, for the year 2022-2023.

## bin
compiled files/executables
## documentation/project
our documentation.
## documentation/tasks
project specifications.
## include
headers (.h).
### include/helper
helper headers, intended to be "private", used in implementation.
### include/kernel
headers readapted from kernel.
### include/phase*_files
headers, given for the project.
## misc
misc files for support while developing.
### misc/ex
examples, exercises, stuff to make practice while developing.
### misc/umps-config
config files for umps3
### misc/vscode
suggested vscode settings (for linux/debian-like).
## obj/path-to-c
object file (.o) of path-to-c
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
compile all .c files and default tests.
### make help
show help screen.
### make DELTEST=T
don't compile default tests
### make T=paths-to-tests
also compile specified tests, written with path and without extension
### make clean
clean.
## README
- README(.md): git readme.
- README.txt: plaint text readme file.
