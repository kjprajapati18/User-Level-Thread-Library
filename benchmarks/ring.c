#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../mypthread.h"

int* p;
mypthread_t *thread, *thread2, *thread0, *thread4;
mypthread_mutex_t *m;
int max = 1;


int killSomeTime(int a){
	a = a*a*a;
	int b = a -6541563;
	int c = b/3;
	int d = c++ *2;
	return d%7135;
}

void* foo(){
	mypthread_yield();
    printf("Foo waits for m\n");
	mypthread_mutex_lock(m);
	*p = 6;
	mypthread_mutex_unlock(m);
	int b = 0;
	while(b<max){
		printf("%d. foo %d\n", b, killSomeTime(b));
		b++;
	}
	b= killSomeTime(b);
	int* m = malloc(sizeof(int));
	*m = b;
	printf("Foo is sending %d\n", *m);
	mypthread_exit(m);
}
void* beef(){
	printf("Beef waits for m\n");
	mypthread_mutex_lock(m);
	*p = 7;
	mypthread_mutex_unlock(m);//
	int* a;
	mypthread_join(*thread, &a);
	printf("Beef got %d from foo", *a);
	int x =0;
	while(x < max){
		printf("%d. beef %d\n", x, killSomeTime(x));
		x++;
	}
}
void* miggy(){
    printf("Miggy waits for m\n");
	mypthread_mutex_lock(m);
	*p = 8;
	mypthread_mutex_unlock(m);
	int x = 0;
	int* a;
	mypthread_join(*thread, &a);
	printf("Miggy got %d from foo", *a);
	while(x<max){
		printf("%d. miggy\n", x, killSomeTime(x));
		x++;
	}
}
void* geet(){
    printf("Geet waits for m\n");
	mypthread_mutex_lock(m);
	*p = *p + 1;
	mypthread_mutex_unlock(m);
	int *a;
	mypthread_join(*thread, &a);
	printf("Geet got %d from foo", *a);
	int x = 0;
	while(x<max){
		printf("geet %d\n", killSomeTime(x));
		x++;
	}
	return;
}

int main(int argc, char **argv) {

	/* Implement HERE */
	m = malloc(sizeof(mypthread_mutex_t));
	mypthread_mutex_init(m, NULL);

	p = (int*) malloc(sizeof(int));
	*p = 5;

	thread = malloc(sizeof(mypthread_t));
	thread2 = malloc(sizeof(mypthread_t));
	thread0 = malloc(sizeof(mypthread_t));
	thread4 = malloc(sizeof(mypthread_t));

		
	mypthread_create(thread0, NULL, miggy, NULL);
	mypthread_create(thread2, NULL, beef, NULL);
	mypthread_create(thread, NULL, foo, NULL);
	mypthread_create(thread4, NULL, geet, NULL);

	mypthread_mutex_lock(m);
	mypthread_yield();
	mypthread_yield();
	mypthread_yield();
	mypthread_mutex_unlock(m);
    printf("Main unlocks m\n");
	int *a;
	mypthread_join(*thread, &a);
	printf("Main got %d from foo", *a);
	int x = 0;
	while(x< max){
		printf("%d. krish %d \n", x, killSomeTime(x));
		x++;
	}
	mypthread_join(*thread0, NULL);
	free(p);
	free(thread);
	free(thread2);
	free(thread4);
	free(thread0);
	mypthread_mutex_destroy(m);
	free(m);
	free(a);
	return 0;
}
