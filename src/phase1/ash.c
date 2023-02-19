#include "ash.h"
#include "pcb_help.h"
/* include for debug */
#include "debug.h"


	/* enough to contain MAXPROC entries */
	
#define HASH_TABLE_SIZE (const_ilog2(MAXPROC) + 1)

static struct list_head semdFree_h;
static DECLARE_HASHTABLE(semd_h, HASH_TABLE_SIZE);


	/* AUXILIARY FUNCTIONS*/


/*
	function that iterates through a hashtable and returns
	the semaphore associated with the hash of the key
	if not present, it returns a pointer pointing to NULL
*/

HIDDEN inline semd_t* __hash_semaphore(int* key){
  semd_t *sem;

  hash_for_each_possible(semd_h, sem, s_link, *key){
    if (sem->s_key == key)
      return sem;
  }
    return NULL;
}

/*
	function that removes a pcb from the process queue
	of a semaphore and checks if it became empty.
	If so, removes the semaphore from the ash
*/

HIDDEN inline void __remove_proc_update_ash(pcb_t* p, semd_t* sem){
	__removeProcEntry_only(&p->p_list);															
	if (emptyProcQ(&sem->s_procq) == true){											//elimino e controllo se la coda dei proc è diventata vuota
		list_add_tail(&sem->s_freelink, &semdFree_h);								//se è così, tolgo il semaforo dalla ash e lo inserisco
		hash_del(&sem->s_link);														//nella lista dei vuoti
	}
	p->p_semAdd = NULL;
}

/*
	function that initializes a semaphore
	from the list of free ones: adds it to the ash, 
	removes it from the free list and sets the key
*/

HIDDEN inline semd_t* __init_free_sem(int* semAdd){
	semd_t* sem = list_first_entry(&semdFree_h, semd_t, s_freelink);
	sem->s_key = semAdd;          										//inizializzo la chiave
	mkEmptyProcQ(&sem->s_procq);     									//inizializzo la lista dei processi bloccati su quel semaforo
	hash_add(semd_h, &sem->s_link, *sem->s_key);     					//metto il semaforo nella hashtable
	list_del_init(&sem->s_freelink);      	 								//rimuovo il sem dalla lista di quelli liberi
	return sem;
}


	/* ASH INITIALIZATION*/


void initASH(){

    static semd_t semdTable[MAXPROC];

    //crea la lista di semdFree
    INIT_LIST_HEAD(&semdFree_h);
    for (int i = 0; i < MAXPROC; i++){
        list_add_tail(&semdTable[i].s_freelink, &semdFree_h);
    }

    //crea la hashtable
    hash_init(semd_h);

}


	/* ASH MANAGEMENT */


int insertBlocked(int *semAdd, pcb_t *p){
	semd_t *sem = __hash_semaphore(semAdd);

	if(sem == NULL) {
		if(list_empty(&semdFree_h))
			return true;
		sem = __init_free_sem(semAdd);             
	}

	p->p_semAdd = semAdd;
	insertProcQ(&sem->s_procq, p);									
	return false;
}


pcb_t* removeBlocked(int *semAdd){
	semd_t *sem = __hash_semaphore(semAdd);

	if (sem != NULL){													//se non trovo il semaforo ritorno NULL
    	pcb_t *p = list_first_entry(&sem->s_procq, pcb_t, p_list);	//altrimenti prendo il primo pcb dalla lista dei processi del sem
		__remove_proc_update_ash(p, sem);
    	return p;
  	}

	return NULL;
}


pcb_t* headBlocked(int *semAdd){															
	semd_t *sem = __hash_semaphore(semAdd);

	return sem == NULL ? NULL : list_first_entry(&sem->s_procq, pcb_t, p_list);	//se trovo un semaforo, ritorno il primo elemento della lista dei proc
}


pcb_t* outBlocked(pcb_t *p){
	semd_t *sem = __hash_semaphore((p->p_semAdd));										

	if(sem != NULL){
		__remove_proc_update_ash(p, sem);
		return p;
	}

	return NULL;
}