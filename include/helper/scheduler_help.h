#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H

#include "pandos_types.h"

//variabile che serve a implementare p_time, essa è l'intervallo di inizio di quando viene scelto un nuovo current process
HIDDEN cpu_t initIntervall;

//macro da chiamare quando viene scelto un nuovo current process
#define start_timer (STCK(initIntervall))

//ritorna il tempo di esecuzione della cpu 
inline cpu_t get_cpu_time () {
	cpu_t tmp;
	STCK(tmp);
	return tmp - initIntervall;
}


#endif /* SCHEDULER_HELP_H */