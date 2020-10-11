#include "heap.h"

int insert(heap* queue, tcb* block){
    queue->size++;
    queue->q[queue->size] = block;
    int curr = queue->size;
    while(queue->q[curr/2]->priority > block->priority){
        queue->q[curr] = queue->q[curr/2];
        curr/= 2;
    }
    queue->q[curr] = block;
    return 0;
}

tcb* pop(heap* queue){
    tcb* out;
    tcb* last;
    memcpy(out, (queue->q[0]), sizeof(tcb*));
    memcpy(last, (queue->q[queue->size-1]), sizeof(tcb*));
    int curr, child;
    for(curr = 0; curr*2+1<=queue->size; curr = child){
        child = curr*2+1;
        if(child!= queue->size && queue->q[child+1]->priority < queue->q[child]->priority){
            child++;
        }
        if (last->priority > queue->q[child]->priority){
            queue->q[curr] = queue->q[child];
        }
        else break;
    }
    queue->q[curr] = last;
    return out;
}
