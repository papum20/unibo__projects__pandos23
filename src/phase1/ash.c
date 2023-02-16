#include "ash.h"


/* enough to contain MAXPROC entries */
#define HASH_TABLE_SIZE (const_ilog2(MAXPROC) + 1)

static struct list_head semdFree_h;
static DECLARE_HASHTABLE(semd_h, HASH_TABLE_SIZE);




static inline semd_t* hash_semaphore(int* key){
  semd_t *sem;
  int found = false;

  hash_for_each_possible(semd_h, sem, s_link, *key){
    if (sem->s_key == key){
      found = true;
      break;
    }
  }

  if (found == true)
    return sem;
  else
    return NULL;
}



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

int insertBlocked(int *semAdd, pcb_t *p){
  semd_t *sem = hash_semaphore(semAdd);

  if (sem == NULL){               // non ho trovato un semaforo con quella chiave
    sem = list_first_entry_or_null(&semdFree_h, semd_t, s_freelink);    //vedo se ce n'è uno disponibile
    if (sem == NULL)        //se non c'è ritorno true
      return true;
    else{                        
      sem->s_key = semAdd;          //inizializzo la chiave
      INIT_LIST_HEAD(&sem->s_procq);     //inizializzo la lista dei processi bloccati su quel semaforo
      list_add_tail(&p->p_list, &sem->s_procq);       //ci aggiungo il pcb 
      hash_add(semd_h, &sem->s_link, *sem->s_key);     //metto il semaforo nella hashtable
      list_del(&sem->s_freelink);       //rimuovo il sem dalla lista di quelli liberi               
      return false;
    }
  }
  else{
    list_add_tail(&p->p_list, &sem->s_procq);
    return false;
  }
}

pcb_t* removeBlocked(int *semAdd){
  semd_t *sem = hash_semaphore(semAdd);

  if (sem == NULL){
    return NULL;
  }
  else{
    pcb_t *pcb = list_first_entry(&sem->s_procq, pcb_t, p_list);
    list_del(&pcb->p_list);
    if (list_empty(&sem->s_procq) == true){
      list_add_tail(&sem->s_freelink, &semdFree_h);
      hash_del(&sem->s_link);
    }
    return pcb;
  }
}

pcb_t* headBlocked(int *semAdd){
  semd_t *sem = hash_semaphore(semAdd);

  if (sem == NULL)
    return NULL;
  else{
	if (list_empty(&sem->s_procq) == true)
		return NULL;
	else
		return list_first_entry(&sem->s_procq, pcb_t, p_list);
		//return list_first_entry(&sem->s_procq, semd_t, p_list);
  }
    
}


pcb_t* outBlocked(pcb_t *p){

  semd_t *sem = hash_semaphore((p->p_semAdd));

  pcb_t *pcb;

  list_for_each_entry(pcb, &sem->s_procq, p_list){
    if (p == pcb)
      break;
  }

  if (pcb != NULL){
    list_del(&pcb->p_list);
    if (list_empty(&sem->s_procq) == true){
      list_add_tail(&sem->s_freelink, &semdFree_h);
      hash_del(&sem->s_link);
    }
  }

  return pcb;
}


/*

list functions:

 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)


 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.


#define list_first_entry_or_null(ptr, type, member) ({ \
	struct list_head *head__ = (ptr); \
	struct list_head *pos__ = READ_ONCE(head__->next); \
	pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
})



*/

/*
hash_table_entry is a type used to represent an entry in a hash table in the Linux kernel. 
The exact definition of this type depends on the specific implementation, but in general, it's a struct that contains the key, value, and metadata required to manage the entries within the hash table.

In the Linux kernel, hash tables are implemented using the hashtable.h header file, which provides a set of macros for creating and manipulating hash tables. 
hash_table_entry is one of the types defined in this header, and is used to represent an individual entry within a hash table.

Each entry in the hash table typically contains a key and a value, which are used to associate a value with a specific key. 
The value might be a pointer to an object, or a simple integer or data structure, depending on the requirements of the application. 
In addition to the key and value, hash_table_entry might also contain metadata, such as a pointer to the next entry in a linked list of entries with the same hash value, or flags indicating the status of the entry.

To use a hash table in the Linux kernel, you would typically declare a hash table using the DECLARE_HASHTABLE macro, and define it using the DEFINE_HASHTABLE macro. 
You would then use a set of functions and macros provided by the hashtable.h header to add, remove, and query entries in the hash table.

For example, you might add an entry to the hash table using the hash_add macro:

hash_add(my_hash_table, &my_entry->node, my_entry_key);
//qui la key è semAdd

hash_del(&my_entry->node);



struct my_entry {
  int key;
  int value;
  struct hlist_node node;
};

struct my_entry *result;
struct hlist_node *tmp;
int key = 42;

hash_for_each_possible(my_hash_table, result, node, key) {
  if (result->key == key) {
    // Key was found, process result
    break;
  }
}

In this example, my_hash_table is a pointer to the hash table, and my_entry is a struct type representing an entry in the hash table. 
The hash_for_each_possible macro is used to iterate over the entries in the bucket corresponding to the provided key, and the callback function compares the key of each entry with the key being searched for.
When a match is found, the desired entry is returned in the result variable.

hlist_node è la hash table entry che usiamo
*/