#include "ns.h"
#include "pcb_help.h"
#include "helper/ns_help.h"

//lista dei PID NSD liberi
static struct list_head pid_nsFree_h;
//lista dei PID NSD attivi
static struct list_head pid_nsList_h;



void initNamespaces(){
	//dichiaro un array static per allocare MAXPROC nsd_t
	static nsd_t pid_nsd[MAXPROC];

	//crea la lista pid_nsFree_h
	INIT_LIST_HEAD(&pid_nsFree_h);
	for(int i=0; i < MAXPROC; i++){
		list_add(&pid_nsd[i].n_link, &pid_nsFree_h);
	}
	//inizializza la lista pid_nsFree_h
	INIT_LIST_HEAD(&pid_nsList_h);
}



nsd_t *getNamespace(pcb_t *p, int type){
	
	for(int i = 0; i < NS_TYPE_MAX; i++){
		if(p->namespaces[i] != NULL && p->namespaces[i]->n_type==type) return p->namespaces[i];
	}
	return NULL;
}


nsd_t *allocNamespace(int type){
	//assegno a tmp_Free la testa della lista degli NSD liberi di tipo type
	struct list_head *tmp_Free=type_nsFree(type);

	if(list_empty(tmp_Free))
		return NULL;

	nsd_t *n = container_of(tmp_Free->next, nsd_t, n_link);
	list_del(tmp_Free->next);
	n->n_type=type;
	return(n);
}


void freeNamespace(nsd_t *ns){

	struct list_head *tmp_Free = type_nsFree(ns->n_type);

	list_add(&ns->n_link, tmp_Free);
}


int addNamespace(pcb_t *p, nsd_t *ns){
	//assegno a tmp_List la testa della lista degli NSD attivi di tipo type
	struct list_head *tmp_List=type_nsList(ns->n_type);
	list_add(&ns->n_link, tmp_List); //aggiungo ns alla lista degli NSD attivi

	pcb_t *px;
	struct list_head queue;//creo una coda per fare una visita in ampiezza dell'albero
	mkEmptyProcQ(&queue);

	if(p!=NULL) insertProcQ(&queue, p);//inserisco il processo in coda
	while(emptyProcQ(&queue)!=1){
		px=removeProcQ(&queue);
		__addNamespace(px ,ns);//associa ns al processo rimosso dalla coda
		pcb_t *pos;
		pcb_for_each_child(pos, px){  //scorro tutti i figli di px
			insertProcQ(&queue, pos);
		}
	}

	return TRUE;
}
