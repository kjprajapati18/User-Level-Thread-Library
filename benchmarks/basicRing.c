#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../mypthread.h"

int interval = 200000;
void idle(){
    int x =0;
    while(1){
        if(x%interval == 0){
            //printf("%d. In idle\n", x/interval);
            puts("Im in idle");
        }
        x++;
    }
}

int main(){

    mypthread_t thread;
    mypthread_create(&thread, NULL, &idle, NULL);
    int x =0;
    struct itimerval thing;
    while(1){
        if(x%interval == 0){
            getitimer(ITIMER_PROF, &thing);
            //printf("%d. In main. Time left = %d\n", x/interval, thing.it_value.tv_usec);
            puts("Im in main");
        }
        x++;
    }
}