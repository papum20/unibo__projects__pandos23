#ifndef NS_HELP_H
#define NS_HELP_H

#include "ns.h"
#include "const.h"


//la macro type_nsFree() ti indirizzza in base a type alla lista di NSD liberi di tipo type
#define type_nsFree(type) \
	((type==NS_PID) ? &pid_nsFree_h : NULL)

//la macro type_nsList() ti indirizza in base a type alla lista di NSD attivi di tipo type
#define type_nsList(type) \
	((type==NS_PID) ? &pid_nsList_h : NULL)


//associa a p il namespace ns
HIDDEN inline void __addNamespace(pcb_t *p, nsd_t *ns){
	int ok=0;

	//verifico se a p è già associato un namespace di tipo type, se non è così scorro l'array dei namespace associati a p 
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
		for(int i = 0; i < NS_TYPE_MAX && !ok; i++){//altrimenti se è gia presente lo sostituisco e faccio free
			if(p->namespaces[i]->n_type==ns->n_type){
				freeNamespace(p->namespaces[i]);
				p->namespaces[i]=ns;
				ok=1;
			}
		}
	}

}


#endif /* NS_HELP_H */