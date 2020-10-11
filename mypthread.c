// File:	mypthread.c

// List all group member's name: Krishna Prajapati, Rakshay Kanthadai
// username of iLab: kjp189, rsk146
// iLab Server:

#include "mypthread.h"

// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
struct itimerval *timer = NULL;
struct sigaction _sa;
tcb* queue[10];
int queueSize = 0;

//CREATE A SCHEDULER FUNCTION. IMPLEMENT YIELD. IMPLEMENT HEAP FOR RUNQUEUE.*********************************

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr,
                      void *(*function)(void*), void * arg) {
       // create Thread Control Block
       // create and initialize the context of this thread
       // allocate space of stack for this thread to run
       // after everything is all set, push this thread int
       // YOUR CODE HERE

		//Malloc space for tcb. Pointer in run queue
		queue[queueSize] = malloc(sizeof(tcb));
		queueSize++;
		tcb* block = queue[queueSize];

		//Initialize variables
		block->threadId = thread;
		block->status = WAITING;
		block->priority = 0;

		//Initialize Context variables
		ucontext_t *cctx = malloc(sizeof(ucontext_t));
		void* stack = malloc(STACK_SIZE);
    	cctx->uc_link = NULL;
    	cctx->uc_stack.ss_sp = stack;
    	cctx->uc_stack.ss_size = STACK_SIZE;
    	cctx->uc_stack.ss_flags = 0;

		//Initialize context function
		if (getcontext(&cctx) < 0){
			perror("getcontext");
			return -1;
		}
		makecontext(cctx, function, 1, arg);
		block->context = cctx;

		//timer setup
    	if(timer == NULL){
			//First thread is being created
			//Get main thread & set-up timer/signals
    		memset(&_sa, 0, sizeof(_sa));
    		_sa.sa_handler = &schedule;
    		sigaction(SIGPROF, &_sa, NULL);

			//Initialize timer vars
			timer->it_interval.tv_usec = 0;
			timer->it_interval.tv_sec = 0;
			timer->it_value.tv_usec = 0;
    		timer->it_value.tv_sec = RESET_TIME;
			
			//CREATE TCB FOR MAIN
			queue[queueSize] = malloc(sizeof(tcb));
			queueSize++;
			tcb* mblock = queue[queueSize];
			mblock->threadId = thread;
			mblock->status = RUNNING;
			mblock->priority = 0;
			ucontext_t *mcctx = malloc(sizeof(ucontext_t));
			void* mstack = malloc(STACK_SIZE);
			mcctx->uc_link = NULL;
			mcctx->uc_stack.ss_sp = mstack;
			mcctx->uc_stack.ss_size = STACK_SIZE;
			mcctx->uc_stack.ss_flags = 0;
			mblock->context = mcctx;
			//Start timer, set context of main
			setitimer(ITIMER_PROF, timer, NULL);
			if (getcontext(&mcctx) < 0){
				perror("getcontextMAIN");
				return -1;
			}
		}

    return 0;
};

/* give CPU possession to other user-level threads voluntarily */
int mypthread_yield() {

	// change thread state from Running to Ready
	// save context of this thread to its thread control block
	// wwitch from thread context to scheduler context

	// YOUR CODE HERE

	//Pause Timer (so we cant get interrupted while calling yield)***********************
	//Swap context to scheduler context just before the return***************************
	return 0;
};

/* terminate a thread */
void mypthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread

	// YOUR CODE HERE
};


/* Wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr) {

	// wait for a specific thread to terminate
	// de-allocate any dynamic memory created by the joining thread

	// YOUR CODE HERE
	return 0;
};

/* initialize the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex,
                          const pthread_mutexattr_t *mutexattr) {
	//initialize data structures for this mutex

	// YOUR CODE HERE
	return 0;
};

/* aquire the mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, enter the critical section
        // if acquiring mutex fails, push current thread into block list and //
        // context switch to the scheduler thread

        // YOUR CODE HERE
        return 0;
};

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex) {
	// Release mutex and make it available again.
	// Put threads in block list to run queue
	// so that they could compete for mutex later.

	// YOUR CODE HERE
	return 0;
};


/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex) {
	// Deallocate dynamic memory created in mypthread_mutex_init

	return 0;
};

/* scheduler */
static void schedule() {
	// Every time when timer interrup happens, your thread library
	// should be contexted switched from thread context to this
	// schedule function

	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();
	// else if (sched == MLFQ)
	// 		sched_mlfq();

	// YOUR CODE HERE
	stopTimer();


// schedule policy
#ifndef MLFQ
	// Choose STCF
	sched_stcf();
#else
	// Choose MLFQ
	sched_mlfq();
#endif

}

/* Preemptive SJF (STCF) scheduling algorithm */
static void sched_stcf() {
	// Your own implementation of STCF
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE

	//CALL RESTART TIMER RIGHT BEFORE A CONTEXT SWITCH***************************************
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

// Feel free to add any other functions you need

// YOUR CODE HERE
void stopTimer()
{
    struct itimerval zeroTimer;
	memset(&zeroTimer, 0, sizeof(struct itimerval));
    setitimer(ITIMER_PROF, &zeroTimer, NULL);
}

void ring(){
	//Pause Timer************************************************************************
	//Swap context just before return****************************************************

	return 0;
}

void restartTimer()
{
    setitimer(ITIMER_PROF, timer, NULL);
}