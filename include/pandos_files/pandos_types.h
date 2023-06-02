#ifndef PANDOS_TYPES_H_INCLUDED
#define PANDOS_TYPES_H_INCLUDED

/****************************************************************************
 *
 * This header file contains utility types definitions.
 *
 ****************************************************************************/

/*#include <umps3/umps/types.h>
*/
#include <pandos_const.h>
#include <list.h>


/* Device register type for disks, flash devices and printers (dtp) */
typedef struct dtpreg {
	unsigned int status;
	unsigned int command;
	unsigned int data0;
	unsigned int data1;
} dtpreg_t;

/* Device register type for terminals */
typedef struct termreg {
	unsigned int recv_status;
	unsigned int recv_command;
	unsigned int transm_status;
	unsigned int transm_command;
} termreg_t;

typedef union devreg {
	dtpreg_t dtp;
	termreg_t term;
} devreg_t;

/* Pass Up Vector */
typedef struct passupvector {
	unsigned int tlb_refill_handler;
	unsigned int tlb_refill_stackPtr;
	unsigned int exception_handler;
	unsigned int exception_stackPtr;
} passupvector_t;

#define STATE_GPR_LEN 29

/* Processor state */
typedef struct state {
	unsigned int entry_hi;
	unsigned int cause;
	unsigned int status;
	unsigned int pc_epc;
	unsigned int gpr[STATE_GPR_LEN];
	unsigned int hi;
	unsigned int lo;
} state_t;

#define reg_at  gpr[0]
#define reg_v0  gpr[1]
#define reg_v1  gpr[2]
#define reg_a0  gpr[3]
#define reg_a1  gpr[4]
#define reg_a2  gpr[5]
#define reg_a3  gpr[6]
#define reg_t0  gpr[7]
#define reg_t1  gpr[8]
#define reg_t2  gpr[9]
#define reg_t3  gpr[10]
#define reg_t4  gpr[11]
#define reg_t5  gpr[12]
#define reg_t6  gpr[13]
#define reg_t7  gpr[14]
#define reg_s0  gpr[15]
#define reg_s1  gpr[16]
#define reg_s2  gpr[17]
#define reg_s3  gpr[18]
#define reg_s4  gpr[19]
#define reg_s5  gpr[20]
#define reg_s6  gpr[21]
#define reg_s7  gpr[22]
#define reg_t8  gpr[23]
#define reg_t9  gpr[24]
#define reg_gp  gpr[25]
#define reg_sp  gpr[26]
#define reg_fp  gpr[27]
#define reg_ra  gpr[28]
#define reg_HI  gpr[29]
#define reg_LO  gpr[30]


/* PID namespace */
#define NS_PID 0
#define NS_TYPE_LAST NS_PID
#define NS_TYPE_MAX (NS_TYPE_LAST + 1)

typedef signed int   cpu_t;
typedef unsigned int memaddr;
typedef unsigned int devregtr;

/* Page Table Entry descriptor */
typedef struct pteEntry_t {
    unsigned int pte_entryHI;
    unsigned int pte_entryLO;
} pteEntry_t;

/* Support level context */
typedef struct context_t {
    unsigned int stackPtr;
    unsigned int status;
    unsigned int pc;
} context_t;

/* Support level descriptor */
typedef struct support_t {
    int        sup_asid;                        /* process ID					*/
    state_t    sup_exceptState[2];              /* old state exceptions			*/
    context_t  sup_exceptContext[2];            /* new contexts for passing up	*/
    pteEntry_t sup_privatePgTbl[USERPGTBLSIZE]; /* user page table				*/
} support_t;


typedef struct nsd_t {
    /* Namespace type */
    int n_type;

    /* Namespace list */
    struct list_head n_link;
} nsd_t, *nsd_PTR;

/* process table entry type */
typedef struct pcb_t {
    /* process queue  */
    struct list_head p_list;

    /* process tree fields */
    struct pcb_t    *p_parent; /* ptr to parent	*/
    struct list_head p_child;  /* children list */
    struct list_head p_sib;    /* sibling list  */

    /* process status information */
    state_t p_s;    /* processor state */
    cpu_t   p_time; /* cpu time used by proc */

    /* Pointer to the semaphore the process is currently blocked on */
    int *p_semAdd;

    /* Pointer to the support struct */
    support_t *p_supportStruct;

    /* Namespace list */
    nsd_t *namespaces[NS_TYPE_MAX];

    /* Process ID */
    int p_pid;
} pcb_t, *pcb_PTR;


/* semaphore descriptor (SEMD) data structure */
typedef struct semd_t {
    /* Semaphore key */
    int *s_key;
    /* Queue of PCBs blocked on the semaphore */
    struct list_head s_procq;

    /* Semaphore hash table */
    struct hlist_node s_link;
    /* Free Semaphores list */
    struct list_head s_freelink;
} semd_t, *semd_PTR;

#endif /* PANDOS_TYPES_H_INCLUDED */