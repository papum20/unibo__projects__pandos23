#include "ns.h"


/*
	for each type of NSD, the following structures are defined:

	type_nsd[MAX_PROC];
	/* array of NSDs with type type, with max size=MAX_PROC
	
	type_nsFree_h;
	/* list of free or unused NSDs with type type
	
	type_nsList_h;
	/* list of active NSDs with type type
*/