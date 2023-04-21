
/*
This module implements the TLB, Program Trap, and
SYSCALL exception handlers. Furthermore, this module will contain
the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/

/*
TLB-Refill
Eccezione che viene sollevata quando nel TLB non 
viene trovata una entry valida.
Non e’ da gestire in questa fase; potete popolare la 
relativa parte del Pass Up Vector con la funzione 
uTLB_RefillHandler() che troverete nel file p2test.c

Nel caso di un’eccezione che non sia TLB-Refill, 
l’altro handler del Pass Up Vector viene invocato 
dal BIOS.
Per distinguere effettivamente di che eccezione si 
tratta bisogna leggere il registro Cause.ExcCode:
- 0 = Interrupt
- 1-3 = TLB Trap
- 4-7,9-12 = Program Trap
- 8 = Syscall

er quanto riguarda TLB Trap e Program Trap, tutto 
quello che si deve fare e’ passare il controllo a un 
gestore indicato dal processo corrente (se presente) 
oppure ucciderlo.  
I processi possono specificare questo gestore al 
momento della creazione tramite System Call; e’ 
salvato nel campo p_supportStruct della struttura 
pcb_t.

Le eccezioni di classe System Call vengono 
sollevate con l’istruzione SYSCALL.
Per passare dei parametri all’eccezione vengono 
usati i registri a0, a1, a2 e a3.
In base al codice in a0 si esegue una delle 
seguenti operazioni, usando a1 - a3 come 
argomenti.
Gestione delle SYSCALL

SYSCALL 1: Create_Process
int SYSCALL(CREATEPROCESS, state_t *statep, support_t *supportp, 
nsd_t *ns);
–  Questa system call crea un nuovo processo 
come figlio del chiamante. Il primo parametro 
contiene lo stato che deve avere il processo. Se la 
system call ha successo il valore di ritorno è zero 
altrimenti è -1.
– supportp e’ un puntatore alla struttura di 
supporto del processo
– restituisce il pid del processo
int SYSCALL(CREATEPROCESS, state_t *statep, support_t *supportp, 
nsd_t *ns)

SYSCALL 1: Create_Process
–  Ns descrive il namespace di un determinato tipo 
da associare al processo, senza specificare il 
namespace (NULL) verra’ ereditato quello del 
padre.

SYSCALL 1: Create_Process
Al momento della creazione di un processo 
e’ necessario creare per questo un id 
univoco che lo identifichi.
L’id puo’ essere (per esempio) un numero 
progressivo non nullo oppure l’indirizzo 
della struttura pcb_t corrispondente

YSCALL 2: Terminate_Process
void SYSCALL(TERMPROCESS, int pid, 0, 0)

–  Quando invocata, la SYS2 termina il processo 
indicato dal secondo parametro insieme a tutta 
la sua progenie.
– Se il secondo parametro e’ 0 il bersaglio e’ il 
processo invocante. 

SYSCALL 3: Passeren
void SYSCALL(PASSEREN, int *semaddr, 0, 0)
–  Operazione di richiesta di un semaforo binario. Il 
valore del semaforo è memorizzato nella 
variabile di tipo intero passata per indirizzo.
L’indirizzo della variabile agisce da identificatore 
per il semaforo

SYSCALL 4: Verhogen
void SYSCALL(VERHOGEN, int *semaddr, 0, 0)
–  Operazione di rilascio su un semaforo binario. Il 
valore del semaforo è memorizzato nella 
variabile di tipo intero passata per indirizzo.
L’indirizzo della variabile agisce da identificatore 
per il semaforo.


Gestione delle SYSCALL• SYSCALL 5: DO_IO
nt SYSCALL(DOIO, int *cmdAddr, int *cmdValues, 0)
– Effettua un’operazione di I/O. CmdValues e’ un vettore di 2 
interi (per I terminali) o 4 interi (altri device).
– La system call carica I registri di device con i valori di 
CmdValues scrivendo il comando cmdValue nei registri 
cmdAddr e seguenti, e mette in pausa il processo 
chiamante fino a quando non si e’ conclusa.
–L’operazione è bloccante, quindi il chiamante viene sospeso 
sino alla conclusione del comando. Il valore ritornato deve 
essere zero se ha successo, -1 in caso di errore. Il contenuto 
del registro di status del dispositivo potra’ essere letto nel 
corrispondente elemento di cmdValues.

estione delle SYSCALL
• SYSCALL 6: Get_CPU_Time
–  Quando invocata, la NSYS6 restituisce il tempo 
di esecuzione (in microsecondi) del processo che 
l’ha chiamata fino a quel momento.
– Questa System call implica la registrazione del 
tempo passato durante l’esecuzione di un 
processo.
int SYSCALL(GETTIME, 0, 0, 0)

YSCALL 7: Wait_For_Clock
– Equivalente a una Passeren sul semaforo dell’Interval Timer.
– Blocca il processo invocante fino al prossimo tick del 
dispositivo.
int SYSCALL(CLOCKWAIT, 0, 0, 0)

SYSCALL 8: Get_Support_Data
– Restituisce un puntatore alla struttura di supporto del 
processo corrente, ovvero il campo p_supportStruct del 
pcb_t.
support_t* SYSCALL(GETSUPPORTPTR, 0, 0, 0)

SYSCALL 9: Get_Process_Id
nt SYSCALL(GETPROCESSID, int parent, 0, 0)

– Restituisce l’identificatore del processo invocante se parent 
== 0, quello del genitore del processo invocante altrimenti. 
– Se il parent non e’ nello stesso PID namespace del processo 
figlio, questa funzione ritorna 0 (se richiesto il pid del 
padre)!


SYSCALL 10: Get_Children

int SYSCALL(GET_CHILDREN, int *children, int size, 0)
– Un processo che invoca questa system call riceve la lista dei 
suoi figli dentro all’array children.
– Il campo size indica la dimensione massima da caricare 
dentro l’array children.
– La system call ritorna il numero di figli di un processo (che 
potrebbe non aver caricato in children se troppo piccolo)

SYSCALL 10: Get_Children
– Se un figlio non e’ nello stesso namespace PID del 
chiamante, questa funzione non ritorna quel figlio nell’array 
childrens!
int SYSCALL(GET_CHILDREN, int *children, int size, 0)


Le System Call elencate identificate da un 
numero da 1 a 10 sono invocabili solo da 
processi in kernel mode; se si riconosce che il 
processo chiamante e’ in user mode 
quest’ultimo deve essere terminato.
Il processo in kernel mode deve essere 
terminato anche se chiama una System Call con 
codice ma inesistente.

SYSCALL non e’ una convenzionale chiamata di 
funzione. Lo stato del processo viene salvato e si 
passa al contesto del kernel: i parametri vengono 
letti nei registri a0-a3, e il valore di ritorno viene 
passato al processo chiamante nel registro v0.
Il processo viene interrotto nell’istruzione 
SYSCALL; per procedere il program counter deve 
essere incrementato di una word (4 byte).



*/



/*

va qua?

The passing up of the handling of all other events. This includes TLB-
Refill events [Section 3.3], SYSCALLs not handled at this level, page
faults, Program Trap exceptions, etc. [Section 3.7]

*/