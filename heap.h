#include "mypthread.c"
#define SIZE 200

typedef struct _heap{
	tcb* q[SIZE];
	int size = 0;
}heap;

int insert(heap* queue, tcb* block);

tcb* pop(heap* queue);
