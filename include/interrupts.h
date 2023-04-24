
/*
This module implements the device/timer interrupt excep-
tion handler. This module will process all the device/timer interrupts,
converting device/timer interrupts into V operations on the appropri-
ate semaphores.
*/


/*
Tabella degli interrupt ...
Interrupt LineDevice Class
0 Inter-processor interrupts
1 Processor Local Timer
2 Bus (Interval Timer)
3 Disk Devices
4 Tape Devices
5 Network (Ethernet) Devices
6 Printer Devices
7 Terminal Devices

Tabella degli interrupt ...
Interru
pt Line
Device Class
0 Inter-processor interrupts
1 Processor Local Timer
2 Bus (Interval Timer)
3 Disk Devices
4 Tape Devices
5 Network (Ethernet) Devices
6 Printer Devices
7 Terminal Devices
Un solo dispositivo
Otto dispositivi per
Ciascuna linea
Distinguere tra sub-device in ricezione o trasmissione

l nucleo deve gestire le linee di interrupt da 1 a 7.
• Azioni che il nucleo deve svolgere:
1. Identificare la sorgente dell’interrupt
- Linea:  registro Cause.IP
- Device sulla linea (>3): Interrupting Device Bit Map
2. Acknowledgment dell’interrupt
-  Scrivere un comando di ack (linea >3) o un nuovo comando 
nel registro del device.
• Interrupt con numero di linea più bassa hanno priorità più alta, e 
dovrebbero essere gestiti per primi.

Utilizzate un semaforo per ogni device per 
“risvegliare” il processo che ha richiesto 
l’operazione di I/O con la SYSCALL 5 (due semafori 
per i terminali che sono device “doppi”).
Notate che le linee di interrupt per i dispositivi di 
I/O (dalla linea 3 in poi) possono essere relative a 
istanze multiple, per cui bisogna distinguere quale 
di esse abbia effettivamente lanciato l’eccezione.

Se si verifica una System Call con codice > 10 o 
una eccezione Trap, il controllo dovrebbe 
passare per la struttura di supporto indicata al 
momento della creazione del processo. Se non e’ 
stata indicata (NULL), il processo deve terminare.

Struttura di supporto
ypedef struct context t {
unsigned int stackPtr, status, pc;
} context t;
typedef struct support t {
int sup_asid;
state_t sup_exceptState[2];
context_t sup_exceptContext[2];
} support t;


*/
/*sono Giuseppe, questa funzione la uso in exceptions.c*/
extern void Interrupt_handler();