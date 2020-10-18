// File:	mypthread_t.h

// List all group member's name: Krishna Prajapati, Rakshay Kanthadai
// username of iLab: kjp189, rsk146
// iLab Server: 

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1
#define RESET_TIME 1000
/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>
#include <string.h>
#include <signal.h>

#define STACK_SIZE SIGSTKSZ
#define SIZE 200

typedef uint mypthread_t;

typedef enum _state {BLOCKED, WAITING, RUNNING} state;

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	// thread Id
	// thread status
	// thread context
	// thread stack
	// thread priority
	// And more ...

	// YOUR CODE HERE
	mypthread_t* threadId;
	state status;
	ucontext_t *context;
	int priority;
	mypthread_t blocker;
	//Create a field to get blocker's return value;
} tcb;

/* mutex struct definition */
typedef struct mypthread_mutex_t {
	/* add something here */

	// YOUR CODE HERE
} mypthread_mutex_t;

/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)
typedef struct _heap{
	tcb* q[SIZE];
	int size;
}heap;

int insert(heap* queue, tcb* block);

tcb* pop(heap* queue);

int findThread(mypthread_t threadId);

typedef struct _node{
	tcb* thread;
	struct _node* next;
}node;

int nodeInsert(node** head, tcb* t);

int pushBack(node** head, heap* queue, tcb* t);
// YOUR CODE HERE

void stopTimer();

void ring();

void restartTimer();

void resumeTimer();
void printLL();
void printHeap();
/* Function Declarations: */

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void
    *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr);

/* initial the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t
    *mutexattr);

/* aquire the mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex);

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);
//scheduler
static void schedule();

//stcf
static void sched_stcf();

//mlfq
static void sched_mlfq();

//free global func
void freeGlob();

#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#endif

#endif
