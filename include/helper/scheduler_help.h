#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H

#include "pandos_types.h"

/*
 * MACRO E FUNZIONI PER GESTIRE IL TEMPO CHE UN PROCESSO HA OCCUPATO LA CPU, IL CAMPO P_TIME
 */

	/* variabile che serve a implementare p_time, essa è l'intervallo di inizio dall'ultima chiamata get_cpu_time */
	/*	l'italiano: "essa è...": stai usando una proposizione logicamente separata dalla precedente, quindi dovresti
		usare una punteggiatura opportuna, che separi (./;)... tuttavia è comunque 1. brutto da leggere,
		2. inutilmente verboso, 3. difficile da leggere perché slegato. Ergo: usa le congiunzioni ("ovvero/cioè/ossia...") */
	cpu_t initIntervall;


	/* macro da chiamare quando viene scelto un nuovo current process */
	/* nella documentazione devi scrivere cosa fa una funzione, non quando la devi usare,
	senza sapere perché */
	#define START_PROCESS_TIMER (STCK(initIntervall))



	/* ritorna il tempo di esecuzione della cpu per il current process, serve alla system call get_cpu_time */
	inline cpu_t get_cpu_time () {
		cpu_t tmp;
		STCK(tmp);
		return tmp - initIntervall + current_proc->p_time;
	}


	/* serve ad aggiornare il valore p_time, viene usato ad esempio quando un processo viene bloccato*/
	inline void update_p_time(){
		cpu_t tmp;
		STCK(tmp);
		current_proc->p_time = current_proc->p_time + tmp - initIntervall;
	}


#endif /* SCHEDULER_HELP_H */