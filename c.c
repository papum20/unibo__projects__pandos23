#include <stdio.h>

#define INIT_NAMESPACE(type) \
	nsd_t type_nsFree

int main() {
	INIT_NAMESPACE(pid);
}