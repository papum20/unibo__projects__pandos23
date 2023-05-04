#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H

#include "pandos_types.h"

/*
	MACRO E FUNZIONI PER GESTIRE IL TEMPO CHE UN PROCESSO HA OCCUPATO LA CPU, IL CAMPO P_TIME
*/

	//variabile che serve a implementare p_time, essa è l'intervallo di inizio dall'ultima chiamata get_cpu_time
	cpu_t initIntervall;


	//macro da chiamare quando viene scelto un nuovo current process
	#define start_timer (STCK(initIntervall))
	/* qaule timer??
	già ci sono 4-5 timer in umps, cercherei almeno di far capire a quale si riferisce;
	inoltre in quanto macro, dovrebbe essere UPPERCASE
	*/

	//ritorna il tempo di esecuzione della cpu per il current process, serve alla system call get_cpu_time
	inline cpu_t get_cpu_time () {
		/*cpu_t tmp = initIntervall;
		start_timer;
		return initIntervall - tmp;
		*/
		cpu_t tmp;
		STCK(tmp);
		return tmp - initIntervall + current_proc->p_time;
	}


	/*serve ad aggiornare il valore p_time, viene usato ad esempio quando un processo viene bloccato*/
	inline void update_p_time(){
		cpu_t tmp;
		STCK(tmp);
		current_proc->p_time = current_proc->p_time + tmp - initIntervall;
	}


#endif /* SCHEDULER_HELP_H */