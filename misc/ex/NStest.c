#include <stdlib.h>
#include <stdio.h>
#include "../../include/pcb.h"

static struct list_head pid_nsFree_h;



#define type_nsFree(type) \
	((type==NS_PID) ? &pid_nsFree_h : NULL)

int main(){
    INIT_LIST_HEAD(&pid_nsFree_h);

    nsd_t *item;
    item=malloc(sizeof(nsd_t));
    item->n_type=1;
    list_add(&item->n_link, &pid_nsFree_h);

    nsd_t *item1;
    item1=malloc(sizeof(nsd_t));
    item1->n_type=0;
    list_add(&item1->n_link, &pid_nsFree_h);

    nsd_t *item2;
    item2=malloc(sizeof(nsd_t));
    item2->n_type=5;
    list_add(&item2->n_link, &pid_nsFree_h);

    nsd_t *pos;
    list_for_each_entry(pos, &pid_nsFree_h, n_link){
        printf("type: %d\n", pos->n_type);
    }

    struct list_head *tmpNS=type_nsFree(0);

    nsd_t *pos1;
    list_for_each_entry(pos1, tmpNS, n_link){
        printf("type: %d\n", pos1->n_type);
    }
}

