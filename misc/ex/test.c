#include <stdlib.h>
#include <stdio.h>
#include "../../include/pcb.h"

LIST_HEAD(ldata);

struct testitem
{
	int value;
	struct list_head list;
};

int main()
{
	struct testitem *item;
	item = malloc(sizeof(struct testitem));
	item->value = 5;
	printf("la lista è %d",list_empty(&(ldata)));
	list_add(&item->list, ldata.next);
	printf(" la lista è %d",list_empty(&(item->list)));

	/*
	if (hash_test(argc, argv) != 0) {
		perror("hash_test");
		return 1;
	}
*/

	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include "../../include/pcb.h"

LIST_HEAD(ldata); //Creazione di una nuova lista con sentinella LDATA

struct testitem
{
	int value;
	struct list_head list;
};

int main()
{	
	//LIST_ADD aggiunge in testa
	struct testitem *item;
	item = malloc(sizeof(struct testitem));
	item->value = 5;
	printf("la lista è %d \n",list_empty(&(ldata)));
	list_add(&item->list, &ldata);  //come secondo elemento dobbiamo mettere sempre per forza la sentinella
	printf("la lista è %d \n",list_empty(&(item->list)));


	//CONTAINER_OF
	//container of ritorna il puntatore alla struttura che contiene quel member, in questo caso il member è list
	//e la struttura che lo contiene è testitem
	struct testitem *item1 = container_of(&item->list, struct testitem, list); 
	int d = item1==item;
	printf("i due puntatori sono identici?  %d \n", d);

	//esempio pratico di uso di container of
	struct testitem *item2;
	item2 = malloc(sizeof(struct testitem));
	item2->value = 10;
	list_add(&item2->list, &ldata);

	struct testitem *tmp = container_of(item2->list.next, struct testitem, list); 
	printf("il value del nodo next di item2(nodo in testa) è %d \n", tmp->value);
	//container serve ad acceder ai campi value quando noi però abbiamo solo list.next e list.prev, quindi ad esempio per accedere
	//al value di list.next usiamo container_of
	
	//USO DI LIST_DEL
	struct testitem *tmp1 = container_of(item2->list.next, struct testitem, list); 
	list_del(item2->list.next);  //eliminare dalla lista l'elemento successivo
	//IMPORTANTE
	free(tmp1); //bisogna sempre poi deallocare 
	
	//printf(" %d", item->value);  //non esiste più il nodo

	//aggiungo qualche elemento alla lista
//--------------------------
	struct testitem *item4;
	item4 = malloc(sizeof(struct testitem));
	item4->value = 20;
	list_add(&item4->list, &ldata);
	

	struct testitem *item5;
	item5 = malloc(sizeof(struct testitem));
	item5->value = 30;
	list_add(&item5->list, &ldata);

	struct testitem *item6;
	item6 = malloc(sizeof(struct testitem));
	item6->value = 40;
	list_add(&item6->list, &ldata);


	struct testitem *item7;
	item7 = malloc(sizeof(struct testitem));
	item7->value = 50;
	list_add(&item7->list, &ldata);
//--------------------------

	//LIST FOR EACH
	struct list_head *pos;
	int i=0;
	list_for_each(pos, &ldata){ //scorre tutta la lista
		i++;
		if(i==1){
			list_del(pos->next);
		}
	}

	//LIST FOR EACH ENTRY
	struct testitem *pos1;
	list_for_each_entry(pos1, &ldata, list){ //scorre tutta la lista grande (quella pure con i value)
		printf("il value del nodo corrente è %d \n", pos1->value);
	}

	return 0;
}
