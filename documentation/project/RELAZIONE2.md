RELAZIONE PROGETTO SISTEMI OPERATIVI PARTE 2
PandOS 2022/2023	-	Università di Bologna



## PARTECIPANTI/AUTORI:

	Daniele		D'Ugo		0001027741
	Osama		Elatfi		0001042877
	Tommaso		Michetti 	0000881326
	Giuseppe	Spathis		0001043077



## DIVISIONE DEL LAVORO:

	Daniele:
		init,testing&debugging,makefile,helper
	Giuseppe:
		Exceptions
	Osama:
		Interrupt 
	Tommaso:
		Scheduler


## STRUTTURA E CONVENZIONI GENERALI:
	Nel corso dello sviluppo, per quanto possibile e in linea con le nostre conoscenze, "imparando facendo", abbiamo cercato di mantenere il codice e i file ordinati, e di rimanere coerenti con delle scelte precise riguardo ad alcuni aspetti, una volta stabilite.

	La fase 2 si suddivide in 4 moduli principali: init, scheduler, interrupts ed exceptions. Su questa falsa riga, abbiamo sviluppato un modulo ciascuno, consci della interazione tra i vari moduli.


## CONVENZIONI (abbiamo mantenuto le stesse della fase 1, questo capitolo è copiato dalla RELAZIONE1):
	static/hidden - public/private:
		Per risolvere l'ambiguità della keyword static, utilizzata per rendere oggetti persistenti in memoria,
		sia per creare il concetto di membro "privato" di un modulo, locale e accessibile solo a chi lo
		dichiara, abbiamo definito il suo alias HIDDEN, utilizzato con il significato di privato.

		Dunque, mentre le funzioni dell'interfaccia pubblica (negli header principali) sono dichiarate extern
		(e definite nei sorgenti), quelle private, utili all'implementazione, collocate nei sorgenti e in
		include/helper/ , sono HIDDEN; infine, le variabili che devono rimanere costanti per il tempo di vita
		del sistema operativo, sono dichiarate static.

		Per leggibilità, le funzioni HIDDEN iniziano con un doppio underscore (__).

	fifo:
		Tutte le liste vengono gestite in modo fifo, in particolare con inserimento in coda e rimozione in testa, per garantire la "fifoness" dove necessaria e per coerenza in generale.

	valori booleani:
		Per coerenza, utilizziamo sempre le costanti true/false definite in phase1_files/types.h.

## COMPILAZIONE:
	E' stato mantenuto il makefile della phase1, con leggere modifiche - nulla di rilevante.
	Il makefile si aspetta di trovare, come test file, 'tests/p2test.04.c'; in alternativa, si possono usare i suoi parametri (make help).
	Comandi:
	-	`make` : compila tutto (produce i file per umps3 in bin/)
	-	`make help` : help
	-	`make clean` : clean


## IMPLEMENTAZIONE:

### scelte progettuali:

	-	Gestione del tempo accumulato dei processi:
		Come indicato dalla documentazione, c'era da fare una scelta di implementazione a riguardo, in particolare se accreditare o meno il tempo di exception handling al processo corrente/che l'aveva causata. La nostra scelta, dunque, essendo risultata anche la più naturale e semplice in corso di sviluppo, è stata quella di lasciar scorrere il PLT durante la gestione delle eccezioni; di conseguenza abbiamo applicato la prima opzione.

	-	Gestione dei semafori:
		I semafori sono memorizzati in un array nel seguente ordine: prima i device esterni (secondo l'ordine delle loro linee), naturalmente il doppio dei samafori per i terminali (prima gli 8 semafori di receipt, seguiti da 8 di transmission) e infine i semafori per il PLT e Interval Timer.
		Per ottenere il semaforo utilizziamo l'indirizzo del registro del device interessato; per il caso particolare dei terminali (con registri di 2 word), si usa l'inizio del "sub-registro" di 2 word come parametro: questo permette di distinguere univocamente tra vari device e tra read/write su terminale usando solo tale parametro.
	
	-   Gestione del pid:
		Riguardo alla scelta del pid associato a ogni processo, esso doveva essere un valore univoco e diverso da zero, quindi abbiamo optato per l'indirizzo di memoria del pcb.


### Strutture e variabili globali:

	Le variabili globali del nucleus sono raccolte in `include/helper/env_nucleus.h` come dichiarazioni `extern`.
	In questo modo, possono essere inizializzate utilizzate tutte nei vari moduli includendo tale file.
	Le vere instanze si trovano invece in `initial.c`, che le inizializza e le mantiene private (non visibili dall'interfaccia, '.h'),
	essendo in un file '.c'.

	Per questa fase, è anche stato creato un largo numero di costanti, macro e 'utility', per semplificare l'interazione con la macchina aggiungendo
	un livello d'astrazione e per rendere il codice più leggibile, dando nomi precisi ed esplicativi a indirizzi di memoria, operazioni a basso livello con i bit
	e semplici comandi che, dovendoli implementare sulla macchina di umps3, sono in realtà composti di più istruzioni: per esempio, esistono delle macro/funzioni
	('RETURN_*', in 'exceptions_help.h') per raccogliere le operazioni compiute a ogni 'return` da una system call, dove bisogna interagire con i registri e le aree
	di memoria giusti.

### modulo INITIAL:
	Semplici inizializzazioni, come indicato nella documentazione e nei commenti del codice, rispettando le linee guida di questa relazione.

### modulo INTERRUPT:

	L'handling degli interrupt comincia identificando la linea a maggior priorità che necessita di attenzione, estrapolando gli 8 bit dal registro cause e prendendo il bit di maggior priorità.

	Abbiamo sviluppato un handler per gli interrupt dei terminali, per device generici e per i due timer hardware. Data la specifica gestione dei terminali, nella funzione che si occupa di questi interrupt, abbiamo dato priorità prima al controllo della linea write (come richiesto). Dato che per ogni linea di interrupt ci sono 8 device, la funzione check_pending_interrupt controlla quali device necessitano di attenzione usando l'interrrupt bit map. Anche qui abbiamo dato priorità ai device con numero più basso. La gestione dei semafori è spiegata sopra, qui ci limitiamo a usare le macro per ottenere il semaforo giusto in base al registro. Per il resto, abbiamo sfruttato le macro e le costanti fornite dai file per gestire correttamente gli interrupt: salvare i risultati di I/O nei registri, resettare i timer, restituire il controllo ai giusti processi o allo scheduler. 

	La funzione SYSCALL_DOIO_return si occupa proprio di restituire i valori di una chiamata I/O, salvando il valore di ritorno (successo o errore) e i risultati dell'operazione nei registri appropriati.


### modulo EXCEPTION:
	Quando viene sollevata un'eccezione l'handler delle eccezioni per prima cosa vede che tipo di eccezione e' stata sollevata guardando, grazie a una macro, nel registro Cause e in particolare nel campo ExcCode, e in base all'eccezione sollevata agisce di conseguenza:

	- Interrupt: 
		- viene chiamato l'handler degli interrupt.
	- eccezioni TLB:
		- viene chiamata una Pass Up or Die passando come parametro il tipo di eccezione sollevata
	- Program Trap: 
		- viene chiamata una Pass Up or Die passando come parametro il tipo di eccezione sollevata
	- System Call:
		- viene chiamato il gestore delle system call

### Gestore delle System Call
	Nel gestore delle system call prima di tutto si leggono i parametri in a1, a2, a3; poi se in a0 é presente un valore superiore a 10 allora viene gestita come se fosse una eccezione TLB o Program Trap, mentre se si é in User Mode viene sollevata una eccezione Program Trap settando Cause.ExcCode a Reserved Instruction e invoncando l'handler delle Trap, altrimenti si usa la macro SYSCALL, che funge da dispatcher per invocare la specifica funzione con i giusti parametri ottenuti in precedenza (infatti ogni system call è implementata come una diversa funzione).

	Tutte le funzioni di gestione delle eccezioni lavorano con i valori dello stato del processore salvati nel SAVED_EXCEPTION_STATE (corrispondente allo stato al momento in cui è stata sollevata).

#### Elenco delle System Call:
	-	1: Create Process: Crea un nuovo processo; infatti, gli alloca un nuovo pcb, lo fa diventare figlio del processo corrente e lo inserisce nella lista dei processi nello stato ready; ritorna -1 se la lista dei pcb liberi è vuota.
	-	2: Terminate Process: Termina un processo relativo al pid passato come parametro e anche tutti i discendenti del processo maniera ricorsiva ed eseguendo le rimozioni in post-ordine; se il pid é zero termina il processo corrente.
	-	3: Passeren: Performa una P su un semaforo binario.
	-	4: Verhogen: Performa una V su un semaforo binario.
	-	5: DOIO: i/o.
	-	6: Get CPU Time: Ritorna il tempo dell'uso del processore da parte del processo corrente. Per calcolare questo tempo abbiamo usato il timer PLT (per informazioni piú dettagliate vedere in "scelte progettuali" la voce "Gestione del tempo accumulato dei processi", e "modulo SCHEDULER").  
	-	7: Wait for Clock: Blocca il processo corrente sul semaforo dell'Interval Timer.
	-	8: Get Support Data: Ritorna il puntatore alla struttura di supporto del processo corrente.
	-	9: Get Process Id: Ritorna il pid del processo corrente, o quello del padre se il parametro è TRUE e il processo corrente è nello stesso namespace PID del padre, altrimenti 0. (per la gestione del PID vedere "scelte progettuali" alla voce "gestione del pid" )
	-	10: Get Children: Itera sulla lista dei figli del processo corrente per aggiungere all'array children, passato come parametro, il pid di ogni figlio che appartiene allo stesso namespace del processo corrente; infine ritorna il numero di figli aggiunti all'array o che potevano essere aggiunti.

#### Ritorno da una system call
	Per le system call non bloccanti o che non fanno terminare processi, bisogna salvare nel registro v0 il valore di ritorno della system call e fare il load di questo processo, caricando il SAVED_EXCEPTION_STATE (infatti, non essendo bloccante, il processo non cambia) attraverso la funzione LDST.
	Invece per le system call bloccanti bisogna salvare nel suo pcb il SAVED_EXCEPTION_STATE e aggiungerlo alla coda dei processi bloccati del giusto semaforo.
	In entrambi i casi bisogna incrementare il suo PC di una word, per evitare un loop di chiamate di syscall quando ritorna alla sua normale esecuzione.
	
### modulo SCHEDULER:
	Lo scheduler è stato sviluppato come richiesto utilizzando un algoritmo round robin preemptive, con il time slice impostato a 5 millisecondi e utilizzando il PLT come system clock per generare interrupt.

	Il PLT viene aggiornato a TIMESLICE (5 millisecondi) nello scheduler con la funzione setTIMER prima di caricare lo stato nel processore.
	Del tempo di CPU accumulato teniamo traccia nel modulo delle exception, salvando il valore del PLT, prima che venga bloccato, in pcb->p_time. Calcolare l'intervallo di tempo di CPU utilizzata è stato più semplice di quanto ci era sembrato all'inizio: infatti abbiamo creato la macro TIMESLICE_USED (in scheduler_help) che sottrae al valore del time slice il valore del PLT nel momento prima del blocco e che quindi restituisce il tempo trascorso dal processo nella CPU nel corrente quanto di tempo. 
	In scheduler_h sono presenti anche altre due macro CPU_TIME_USED e PROC_TIME_UPDATE: la prima ritorna il valore di p_time del pcb passato in input più il tempo di utilizzo della CPU durante il quanto di tempo corrente, mentre la seconda esegue le stesse operazioni solamente che al posto di ritornare un valore lo salva/aggiorna direttamente nel campo p_time. Queste due macro sono utilizzate rispettivamente nel modulo exception (SYS 6) e modulo interrupt (nell'handler del interrupt PLT).

	Nel caso in cui non ci siano processi ready, prima di mettere il sistema in WAIT abbiamo deciso di disabilitare il PLT, per evitare che il primo interrupt dopo la WAIT sia un PLT interrupt.

### Modifiche alla phase1:
	In corso di sviluppo, sono anche state effettuate leggere modifiche ai moduli della prima fase, rese necessarie e/o convenienti anche per i seguenti motivi:
	-	lo struct `pcb_t` è stato ampliato con nuovi campi, e in più alcuni già presenti che non erano utilizzati prima, nella fase 2 si; di conseguenza abbiamo dovuto pensare alla loro inizializzazione e gestione;
	-	ricollegandoci al punto precedente, le reali funzioni di alcuni campi di `pcb_t` e altre strutture non erano chiari prima come ora, dove gestiamo noi stessi l'interno nucleus; pertanto ci siamo ritrovati a dover correggere alcuni dettagli (che magari non erano venuti fuori dal primo test) - per esempio, `addNamespace` è passato da un'implementazione iterativa a una ricorsiva.

## DIFFICOLTA' INCONTRATE:
	Una delle principali difficoltà incontrate nello sviluppo del progetto non riguardava la comprensione di ciò che dovevano svolgere i nostri moduli (concetti chiari dalle lezioni teoriche), quanto capire come implementarli sulla macchina a disposizione. Ad esempio, la comprensione dei diversi timer e delle loro funzionalità non sono risultate immediate. 

	Anche comprendere l'avvicendamento delle varie componenti del sistema operativo non è stato facile. Capire come si susseguono i vari gestori, scheduler e altri componenti, oppure sviluppare handler sincronizzati tra vari moduli è stata una sfida, come la systemcall DO_IO, che si articola sia nel modulo delle exception che in quello degli interrupt.

	Infine, come c'era da aspettarsi, il "debugging finché non funziona tutto il test" ha richiesto un certo impegno, dovendo lavorare, con il linguaggio c, a un livello abbastanza basso, analizzando lo stato di memoria e registri durante l'esecuzione, di istruzione in istruzione o chiamata in chiamata.
	Va comunque sottolineato che, con una macchina semplificata come umps3 e una GUI che lascia svariati strumenti di debugging, la "sfida" è risultata della giusta difficoltà e ha permesso di apprezzare il valore didattico e pratico del progetto.