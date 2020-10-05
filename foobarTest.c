#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <ucontext.h>
#include <string.h>
#include <sys/time.h>

#define STACK_SIZE SIGSTKSZ

ucontext_t cctx;
int counter = 0;
void foo(){
    while(1){
        puts("foo");
    }
}

void bar(){
    while(1){
        puts("bar");
    }
}
void swap(int signum){
    counter++;
    //printf("RING RING! The timer has gone off\n");
    if(counter %2){
        makecontext(&cctx, (void*)&bar, 0);
        setcontext(&cctx);
    }
    else{
        makecontext(&cctx, (void*)&foo, 0);
        setcontext(&cctx);
    }
}

void ring(int signum){
    printf("RING RING RING");
}

void simple(){
    sleep(1);
    printf("Yello\n");
}


int main(int argc, char** argv){
    if (argc != 1) {
		printf(": USAGE Program Name and no Arguments expected\n");
		exit(1);
	}
    if (getcontext(&cctx) < 0){
		perror("getcontext");
		exit(1);
	}
    //timer setup
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &swap;
    sigaction(SIGPROF, &sa, NULL);
    struct itimerval timer;

    timer.it_interval.tv_usec = 0;
    timer.it_interval.tv_sec = 1;

    timer.it_value.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    //set timer
    setitimer(ITIMER_PROF, &timer, NULL);
    //set up ucontext
    void* stack = malloc(STACK_SIZE);
    cctx.uc_link = NULL;
    cctx.uc_stack.ss_sp = stack;
    cctx.uc_stack.ss_size = STACK_SIZE;
    cctx.uc_stack.ss_flags = 0;

    makecontext(&cctx, (void*)&foo, 0);
    puts("Successfully modified context");
    setcontext(&cctx);
}