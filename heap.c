#include "heap.h"

int insert(heap* queue, tcb* block){
    queue->q[queue->size] = block;
    queue->size++;
    int curr = queue->size-1;
    while(queue->q[(curr-1)/2]->priority > block->priority){
        queue->q[curr] = queue->q[(curr-1)/2];
        curr = (curr-1)/2;
    }
    queue->q[curr] = block;
    return 0;
}

tcb* pop(heap* queue){
    if(queue->size == 0){
        return NULL;
    }
    tcb* out = queue->q[0];
    tcb* last = queue->q[queue->size-1];
    int curr, child;
    for(curr = 0; curr*2+1<=queue->size; curr = child){
        child = curr*2+1;
        if(child < queue->size && queue->q[child+1]->priority < queue->q[child]->priority){
            child++;
        }
        if (child < queue->size && last->priority > queue->q[child]->priority){
            queue->q[curr] = queue->q[child];
        }
        else break;
    }
    queue->q[curr] = last;
    return out;
}
