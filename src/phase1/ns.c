#include "ns.h"

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

	struct list_head *tmp_Free=type_nsFree(ns->n_type);

	list_add(&ns->n_link, tmp_Free);
}


void addNamespace_h(pcb_t *p, nsd_t *ns){
	int ok=0;

	//verifico se a p è gia associato una namespace di tipo type, se non è così scorro l'array dei namespace associati a p 
	//e lo inserisco
	if(getNamespace(p, ns->n_type)==NULL){
		for(int i = 0; i < NS_TYPE_MAX && !ok; i++){
			if(p->namespaces[i]==NULL){
				p->namespaces[i]=ns;
				ok=1;
			}
		}
	}
	else{
		for(int i = 0; i < NS_TYPE_MAX && !ok; i++){
			if(p->namespaces[i]->n_type==ns->n_type){
				freeNamespace(p->namespaces[i]);
				p->namespaces[i]=ns;
				ok=1;
			}
		}
	}

}


int addNamespace(pcb_t *p, nsd_t *ns){

	struct list_head *tmp_List=type_nsList(ns->n_type);
	list_add(&ns->n_link, tmp_List);

	pcb_t *px;
	struct list_head *queue;
	mkEmptyProcQ(queue);

	if(p!=NULL) insertProcQ(queue, p);
	while(emptyProcQ(queue)!=1){
		px=removeProcQ(queue);
		addNamespace_h(px ,ns);
		pcb_t *pos;
		list_for_each_entry(pos, &(px->p_child), p_child){
			insertProcQ(queue, pos);
		}
	}
}
