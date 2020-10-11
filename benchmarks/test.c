#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../mypthread.h"

/* A scratch program template on which to call and
 * test mypthread library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */
void* foo(){
	while(1){
		//printf("foo\n");
	}
}
void* beef(){
	while(1){
		//printf("beef\n");
	}
}
void* miggy(){
	while(1){
		//printf("miggy\n");
	}
}
void* geet(){
	while(1){
		//printf("geet\n");
	}
}

int main(int argc, char **argv) {

	/* Implement HERE */
	mypthread_t thread, thread2, thread0, thread4;
	mypthread_create(&thread, NULL, foo, NULL);
	mypthread_create(&thread2, NULL, beef, NULL);
	mypthread_create(&thread0, NULL, miggy, NULL);
	mypthread_create(&thread4, NULL, geet, NULL);

	while(1){
		//printf("bar\n");
	}

	return 0;
}
