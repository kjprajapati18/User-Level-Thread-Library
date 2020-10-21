// File:	mypthread.c

// List all group member's name: Krishna Prajapati, Rakshay Kanthadai
// username of iLab: kjp189, rsk146
// iLab Server: ilab3

#include "mypthread.h"


//#define debug 1
// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
struct itimerval *timer = NULL;
struct sigaction *_sa;
heap* queue;
ucontext_t* schedctx;
ucontext_t* ex;
node** blockList;
tcb* current;
//asign to 0
uint count;

int RESET_TIME2 = 8000;


/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr,
                      void *(*function)(void*), void * arg) {
       // create Thread Control Block
       // create and initialize the context of this thread
       // allocate space of stack for this thread to run
       // after everything is all set, push this thread int
       // YOUR CODE HERE

		if(timer == NULL){
			atexit(freeGlob);

			queue = (heap*) malloc(sizeof(heap));
			queue->size = 0;
			count = 1;
			
			//Set up mypthread_exit context
			ex = (ucontext_t*) malloc(sizeof(ucontext_t));
			void* stackthree = malloc(STACK_SIZE);
			ex->uc_link = NULL;
			ex->uc_stack.ss_sp = stackthree;
			ex->uc_stack.ss_size = STACK_SIZE;
			ex->uc_stack.ss_flags = 0;
			if(getcontext(ex) < 0){
				perror("getcontextEXIT");
				return -1;
			}
		} 
		
		//Malloc space for tcb. Pointer in run queue
		tcb* block = (tcb*) malloc(sizeof(tcb));

		//Initialize variables
		*thread = count;
		count++;
		block->threadId = thread;
		block->status = WAITING;
		block->priority = 0;
		block->blocker = 0;
		block->mutex = NULL;
		block->returnValue = NULL;

		//Initialize Context variables
		ucontext_t *cctx = malloc(sizeof(ucontext_t));
		void* stack = malloc(STACK_SIZE);
    	cctx->uc_link = ex;
    	cctx->uc_stack.ss_sp = stack;
    	cctx->uc_stack.ss_size = STACK_SIZE;
    	cctx->uc_stack.ss_flags = 0;

		//Initialize context function
		if (getcontext(cctx) < 0){
			perror("getcontext");
			return -1;
		}
		makecontext(cctx, function, 1, arg);
		block->context = cctx;
		insert(queue, block);

		//timer setup
    	if(timer == NULL){
			//First thread is being created
			//Get main thread & set-up timer/signals
			_sa = malloc(sizeof(struct itimerval));
    		memset(_sa, 0, sizeof(struct itimerval));
    		_sa->sa_handler = &ring;
    		sigaction(SIGPROF, _sa, NULL);
			schedctx = (ucontext_t*) malloc(sizeof(ucontext_t));
			void* stacktwo = malloc(STACK_SIZE);
			schedctx->uc_link = NULL;
			schedctx->uc_stack.ss_sp = stacktwo;
			schedctx->uc_stack.ss_size = STACK_SIZE;
			schedctx->uc_stack.ss_flags = 0;
			
			if (getcontext(schedctx) < 0){
				perror("getcontextSCHED");
				return -1;
			}
			
			makecontext(schedctx, (void*) &schedule, 0);
			makecontext(ex, (void*) &mypthread_exit, 0);

			//Initialize timer vars
			timer = malloc(sizeof(struct itimerval));
			timer->it_interval.tv_usec = RESET_TIME2;
			timer->it_interval.tv_sec = 0;
			timer->it_value.tv_usec = RESET_TIME2;
    		timer->it_value.tv_sec = 0;
			
			//CREATE TCB FOR MAIN
			tcb* mblock = (tcb*) malloc(sizeof(tcb));
			mblock->threadId = NULL;
			mblock->status = RUNNING;
			mblock->priority = 0;
			mblock->blocker = 0;
			mblock->mutex = NULL;
			mblock->returnValue = NULL;
			ucontext_t *mcctx = malloc(sizeof(ucontext_t));
			void* mstack = malloc(STACK_SIZE);
			mcctx->uc_link = NULL;
			mcctx->uc_stack.ss_sp = mstack;
			mcctx->uc_stack.ss_size = STACK_SIZE;
			mcctx->uc_stack.ss_flags = 0;
			mblock->context = mcctx;
			current = mblock;
			//create block queue
			node** head = malloc(sizeof(node*));
			blockList = head;
			*blockList = NULL;
			//Start timer, set context of main
			setitimer(ITIMER_PROF, timer, NULL);
			if (getcontext(mcctx) < 0){
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

	signal(SIGPROF, SIG_IGN);
	swapcontext(current->context, schedctx);

	return 0;
};


//consider main exits
/* terminate a thread */
void mypthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread
	
	// YOUR CODE HERE

	signal(SIGPROF, SIG_IGN);

	pushBackThread(blockList, queue, current, value_ptr);
	#ifdef debug 
		printf("Succesful pushback\n");
	#endif
	free(current->context->uc_stack.ss_sp);
	free(current->context);

	free(current);
	current = NULL;
	setcontext(schedctx);
	return;
};


/* Wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr) {

	// wait for a specific thread to terminate
	// de-allocate any dynamic memory created by the joining thread

	// YOUR CODE HERE

	signal(SIGPROF, SIG_IGN);
	int threadExists = findThread(thread);

	if(threadExists){
		current->blocker = thread;
		current->status = BLOCKED;
		nodeInsert(blockList, current);
		tcb* temp = current;
		mypthread_t* id = current->threadId;
		#ifdef debug
			if(id == NULL) printf("Main calls schedule\n");
			else printf("%d calls schedule\n", *id);
		#endif
		current = NULL;
		swapcontext(temp->context, schedctx);
		//Current comes back into running. We pick up where we left off.
		if(value_ptr != NULL){
			*value_ptr = current->returnValue;
		}
	} else{
		#ifdef debug
			printf("Didnt find thread_id %d\n", thread);
		#endif
		sigaction(SIGPROF, _sa, NULL);
		
	}
	return 0;
};

/* initialize the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex,
                          const pthread_mutexattr_t *mutexattr) {
	//initialize data structures for this mutex

	// YOUR CODE HERE
	if(mutex != NULL && mutex->init == 2){
		return -1;
	}
	int* lock = malloc(sizeof(int));
	*lock = 0;
	mutex->init = 2;
	mutex->lock_status = lock;
	return 0;
};

/* aquire the mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, enter the critical section
        // if acquiring mutex fails, push current thread into block list and //
        // context switch to the scheduler thread

        // YOUR CODE HERE
		
		signal(SIGPROF, SIG_IGN);
		while(__sync_lock_test_and_set(mutex->lock_status, 1)){
			
			current->status = BLOCKED;
			current->blocker = 0;
			current->mutex = mutex;
			nodeInsert(blockList, current);
			tcb* temp = current;
			current = NULL;
			swapcontext(temp->context, schedctx);
		}
		
		sigaction(SIGPROF, _sa, NULL);
		
        return 0;
};

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex) {
	// Release mutex and make it available again.
	// Put threads in block list to run queue
	// so that they could compete for mutex later.

	// YOUR CODE HERE

	//Don't stop or restart the timer because the block list has HEAD Insert. The node that we get interrupted on will still exist in block queue, as well as everything after
	//Worst case is that not every thread gets to fairly compete for Mutex if we get interrupted, but everything afterward will get unblocked as if it wasnt interrupted
	//		If the mutex gets locked before we context switch back, we'll just end up putting a thread back in to run queue that should stay in block list
	//		However, it is stuck in the while loop in lock, and therefore will come back here eventually. In fact, it will have a better chance at competing for the next iteration
	//		Which is fair.
	
	*(mutex->lock_status) = 0;
	pushBackMutex(blockList, queue, mutex);
	
	return 0;
};


/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex) {
	// Deallocate dynamic memory created in mypthread_mutex_init

	mutex->init = 0; 
	free(mutex->lock_status);
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

	if(current != NULL){ 
		current->status = WAITING;
	}
	#ifdef debug
		printLL();
		printHeap();
	#endif
	// schedule policy
	#ifndef MLFQ
		// Choose STCF
		sched_stcf();
	#else
		// Choose MLFQ
		sched_mlfq();
	#endif
	if(current == NULL) setcontext(schedctx); //Equivalent to idle wait to let programmer know that he's deadlocked
	current->status = RUNNING;
	//restartTimer();
	sigaction(SIGPROF, _sa, NULL);
	setcontext(current->context);
}

/* Preemptive SJF (STCF) scheduling algorithm */
static void sched_stcf() {
	// Your own implementation of STCF
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
	tcb* next = pop(queue);
	//current is null and next is null we need to return to main to finish code
	//We actually don't need this since main should always join all threads
	if (current == NULL && next == NULL && *blockList == NULL){
		//No threads running, waiting, or blocked
		
		freeGlob();
		exit(0);
	}

	//Since current has run, even if its the only thing that can run, we should increase the quantum
	if (current != NULL){
		current->priority++;
	}
	
	//If next is NULL, keep current as is (nothing else can run). Otherwise switch to next.
	if (next != NULL){
		insert(queue, current);
		current = next;
	}
	return;
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

// Feel free to add any other functions you need

// YOUR CODE HERE


void ring(){
	//Pause Timer************************************************************************
	//Swap context just before return****************************************************
	#ifdef debug
		printf("brrriing brring\n");
	#endif
	//stopTimer();
	signal(SIGPROF, SIG_IGN);
	swapcontext(current->context, schedctx);
	return;
}

//heap functions

int insert(heap* queue, tcb* block){
	if(block == NULL) return -1;
    queue->q[queue->size] = block;
    queue->size++;
    int curr = queue->size-1;
	int parent = (curr-1)/2;
    while(queue->q[parent]->priority > block->priority && parent != curr){
        queue->q[curr] = queue->q[parent];
		curr = parent;
        parent = (curr-1)/2;
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
    for(curr = 0; curr*2+1<queue->size; curr = child){
        child = curr*2+1;
        if(child+1 < queue->size && queue->q[child+1]->priority < queue->q[child]->priority){
            child++;
        }
        if (child < queue->size && last->priority > queue->q[child]->priority){
            queue->q[curr] = queue->q[child];
        }
        else break;
    }
    queue->q[curr] = last;
	queue->size--;
    return out;
}

int findThread(mypthread_t threadId){
	int i;
	for(i = 0; i < queue->size; i++){
		mypthread_t* id = (queue->q[i])->threadId;
		if(id == NULL) continue;
		if(*id == threadId) return 1;
	}
	//Search through the block queue
	node* ptr = *blockList;
	while(ptr != NULL){
		mypthread_t* id2 = ptr->thread->threadId;
		#ifdef debug
			if (id2==NULL) printf("searching main\n");
			else printf("searching %d", *id2);
		#endif
		if(id2 != NULL && *id2 == threadId) return 1;
		ptr = ptr->next;
	}
	return 0;
}

//free blocklist, contexts, globs
//call when main exits
void freeGlob(){
	signal(SIGPROF, SIG_IGN);
	if(current != NULL){
		free(current->context->uc_stack.ss_sp);
		free(current->context);
		free(current);
	}
	#ifdef debug
		printf("In free glob");
	#endif
	free(timer);
	free(_sa);
	free(queue);
	free(schedctx->uc_stack.ss_sp);
	free(schedctx);
	free(ex->uc_stack.ss_sp);
	free(ex);
	free(blockList);
	return;
}


int nodeInsert(node** head, tcb* t){
	node* newNode = (node*) malloc(sizeof(node));
	newNode->thread = t;
	newNode->next = *head;
	*head = newNode;
	return 0;
}


int pushBackThread(node** head, heap* queue, tcb* t, void* returnPtr){
	if (*head == NULL){
		return -1;
	}
	node* ptr = (*head)->next;
	node* prev = *head;
	mypthread_t threadExitId = *(t->threadId);

	while(ptr != NULL){
		#ifdef debug
			printf("Looking thorugh pushback Thread\n");
		#endif
		if(ptr->thread->blocker == threadExitId){
			ptr->thread->status = WAITING;
			ptr->thread->blocker = 0;
			ptr->thread->returnValue = returnPtr;

			prev->next = ptr->next;
			insert(queue, ptr->thread);
			free(ptr);
		}
		else{
			prev = ptr;
		}
		ptr = ptr->next;
	}
	//checking head last
	ptr = *head;	
	if(ptr->thread->blocker == threadExitId){
		
		ptr->thread->status = WAITING;
		ptr->thread->blocker = 0;
		ptr->thread->returnValue = returnPtr;

		insert(queue, ptr->thread);
		*head = ptr->next;
		free(ptr);
	}
	return 0;
}

int pushBackMutex(node** head, heap* queue, mypthread_mutex_t* m){
	if (*head == NULL){
		return -1;
	}
	node* ptr = (*head)->next;
	node* prev = *head;

	while(ptr != NULL){
		#ifdef debug
			printf("Looking thorugh pushback Mutex\n");
		#endif
		if(ptr->thread->mutex == m){
			ptr->thread->status = WAITING;
			ptr->thread->blocker = 0;
			ptr->thread->mutex = NULL;
			prev->next = ptr->next;
			insert(queue, ptr->thread);
			free(ptr);
		}
		else{
			prev = ptr;
		}
		ptr = ptr->next;
	}
	//checking head last
	ptr = *head;	
	if(ptr->thread->mutex == m){
		
		ptr->thread->status = WAITING;
		ptr->thread->blocker = 0;
		ptr->thread->mutex = NULL;
		insert(queue, ptr->thread);
		*head = ptr->next;
		free(ptr);
	}
	return 0;
}

void printLL(){
	node* ptr = *blockList;
	printf("Block List: ");
	if(ptr == NULL) printf("NULL\n");
	else{
		while(ptr != NULL){
			mypthread_t *id = (ptr->thread->threadId);
			if(id == NULL) printf("MAIN -> ");
			else printf("%d -> ", *id);
			ptr = ptr->next;
		}
		printf("\n");
	}
	return;
}

void printHeap(){
	int i;
	printf("Run Queue: ");
	for(i = 0; i < queue->size; i++){
		mypthread_t* id = (queue->q[i])->threadId;
		if(id == NULL) printf("MAIN -> ");
		else printf("%d -> ", *id);
	}
	printf("END\n");
	
	return;
}
