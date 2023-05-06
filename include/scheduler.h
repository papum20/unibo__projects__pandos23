
/*
his module implements the Scheduler and the deadlock
detector
*/


/*
Scheduler
Una volta che lo scheduler viene lanciato la 
prima volta il controllo non dovrebbe piu’ 
tornare al main().
Da qui in avanti l’esecuzione e’ in mano al 
processo di test; l’unico momento in cui si torna 
al vostro kernel e’ nell’eventualita’ di 
un’eccezione.


cheduler
Lo scheduler di Panda+ ha un singolo livello di 
priorita’.
Lo scheduler e’ implementato con una coda di 
processi.
Il ruolo dello scheduler e’, fondamentalmente, 
decidere quale processo deve entrare in 
esecuzione.

Se la coda dei processi non e’ vuota si estrae il 
primo processo disponibile da li’ e lo si carica in 
esecuzione.
I processi vengono estratti dalla coda in ordine 
FIFO.

Stato del processore
Caricare un processo in esecuzione significa 
copiare il suo stato salvato (il campo p_s del 
relativo pcb_t) nel processore.
A questo scopo esiste la funzione LDST.

Un processo puo’ essere:
- ready
- running
- blocked/waiting
Il contatore di “soft blocked” indica il numero di 
processi bloccati su un’operazione di IO (che quindi 
finira’) e non un semaforo.
Man mano che i processi passano da uno stato 
all’altro e’ vostra responsabilita’ aggiornare questo 
contatore.

Se ad un certo momento la coda e’ vuota, lo 
scheduler deve verificare alcune condizioni:
- Se non ci sono piu’ processi (Process Count == 
0), spegnere la macchina con HALT().
- Se Process Count > 0 e Soft Blocked Count > 0 il 
processore deve essere messo in stato di attesa 
(funzione WAIT()).
- Se Process Count > 0 e Soft Blocked Count == 0 
lo scheduler e’ in deadlock: invocare PANIC().

*/

/*

the Scheduler ever discovers that the Ready Queue is empty it will
either HALT execution (no more processes to run), WAIT for an I/O
to complete (which will unblock a pcb and populate the Ready Queue),
or PANIC (halt execution in the presence of deadlock)
*/

#include "list.h"
#include "scheduler_help.h"

extern void Scheduler();
/* TEMPORARY NAME !!
*/

/*ritorna la lista dei processi ready (fatta da Giuseppe, se vuoi Tommaso la puoi cambiare)*/
extern struct list_head* getHeadRd();
//sono io Giuseppe, questa funzione ritorna the amount of CPU time used during the current quantum/time slice.
extern cpu_t get_CPU_time_slice_passed();
