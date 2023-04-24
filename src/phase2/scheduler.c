#include "scheduler.h"
//Puntatore al processo corrente, variabile condivisa
pcb_t* current_proc = NULL;

pcb_t* runningProc() //Funzione che permette di accere al current proc da altri file
{
	return current_proc;
}