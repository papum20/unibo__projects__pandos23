#include <stdlib.h>
#include <stdio.h>


typedef struct S {
	int a;
	int b;
} s;
typedef struct Q {
	int *a;
	int *b;
} q;

#undef NULL
#define NULL (void*)(1)


int main() {
	s t;
	printf("%d %d\n", t.a, t.b);
	t = (s){0};
	printf("%d %d\n", t.a, t.b);
	t = (s){1,2};
	printf("%d %d\n", t.a, t.b);
	t = (s){0};
	printf("%d %d\n", t.a, t.b);

	q t2;
	q t3 = {malloc(sizeof(int)), malloc(sizeof(int))};
	printf("%p %p\n", t2.a, t2.b);
	printf("%p %p\n", t3.a, t3.b);
	t2 = (q){NULL};
	printf("%p %p {NULL}\n", t2.a, t2.b);
	t2 = t3;
	printf("%p %p t3\n", t2.a, t2.b);
	t2 = (q){0};
	printf("%p %p {0}\n", t2.a, t2.b);
	t2 = t3;
	printf("%p %p t3\n", t2.a, t2.b);
	t2 = (q){};
	printf("%p %p {}\n", t2.a, t2.b);

	free(t3.a);
	free(t3.b);
}