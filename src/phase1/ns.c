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
	list_del_init(tmp_Free->next);
	n->n_type=type;
	return(n);
}


void freeNamespace(nsd_t *ns){

	struct list_head *tmp_Free = type_nsFree(ns->n_type);

	list_add(&ns->n_link, tmp_Free);
}


int addNamespace(pcb_t *p, nsd_t *ns){
	
	//assegno a tmp_List la testa della lista degli NSD attivi di tipo type
	struct list_head *type_list=type_nsList(ns->n_type);
	list_add(&ns->n_link, type_list); //aggiungo ns alla lista degli NSD attivi

	__addNamespace(p ,ns);

	pcb_t *pos;
	pcb_for_each_child(pos, p) {
		if(getNamespace(pos, ns->n_type) == ns)
			addNamespace(pos, ns);
	}

	// ritorna sempre true, non essendo definiti casi di errore al momento
	return true;
}
