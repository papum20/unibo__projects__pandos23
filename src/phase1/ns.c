#include "include/ns.h"

static struct list_head pid_nsFree_h;
static struct list_head pid_nsList_h;

#define type_nsFree(type) \
	((type==NS_PID) ? &pid_nsFree_h : NULL)


#define type_nsList(type) \
	((type==NS_PID) ? &pid_nsList_h : NULL)



void initNamespace(){

	static nsd_t pid_nsd[MAXPROC];

	//inizializza la lista pid_nsFree_h
	INIT_LIST_HEAD(&pid_nsFree_h);
	for(int i=0; i < MAXPROC; i++){
		list_add(&pid_nsd[i].n_link, &pid_nsFree_h);
	}

	INIT_LIST_HEAD(&pid_nsList_h);
}



nsd_t *getNamespace(pcb_t *p, int type){

	for(int i = 0; i < NS_TYPE_MAX; i++){
		if(p->namespaces[i]->n_type==type) return p->namespaces[i];
	}
	return NULL;
}


nsd_t *allocNamespace(int type){

	struct list_head *tmp_Free=type_nsFree(type);

	if(list_empty(tmp_Free))
		return NULL;

	nsd_t *n=container_of(tmp_Free->next, nsd_t, n_link);
	list_del(tmp_Free->next);
	n->n_type=type;
	return(n);
}


void freeNamespace(nsd_t *ns){
	
}


// questo addNamespace è completamente sbagliato, l'avevo fatto essendo convinto di alcune cose ma sicuramente non è corretto  
int addNamespace(pcb_t *p, nsd_t *ns){
	//verifico se a p è gia associato una namespace di tipo type, se non è così scorro l'array dei namespace associati a p e lo inserisco
	int ok=0;
	if(getNamespace(p, ns->n_type)==NULL){
		for(int i = 0; i < NS_TYPE_MAX && ok; i++){
			if(p->namespaces[i]==NULL){
				p->namespaces[i]=ns;
				ok=1;
			}
		}
	}
	else return(0);

	//faccio la stessa cosa per ogni figlio di p, usando list_for_each_entry
	pcb_t *pos;
	list_for_each_entry(pos, &(p->p_child), p_child){
		ok=0;
		if(getNamespace(pos, ns->n_type)==NULL){
			for(int i = 0; i < NS_TYPE_MAX && ok; i++){
				if(pos->namespaces[i]==NULL){
					pos->namespaces[i]=ns;
					ok=1;
				}
			}
		}
	}
}