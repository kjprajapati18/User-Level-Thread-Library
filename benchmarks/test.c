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
struct thing{
	int a;
	int b;
	int c;
	int d;

};

int* p;
mypthread_t *thread, *thread2, *thread0, *thread4;
mypthread_mutex_t *m;
int max = 4;

int killSomeTime(int a){
	a = a*a*a;
	int b = a -6541563;
	int c = b/3;
	int d = c++ *2;
	return d%7135;
}

void* foo(){
	struct thing* x = malloc(sizeof(struct thing));
	x->a = 1;
	x->b = 1;
	x->c = 2;
	x->d = 4;
	int b = 0;
	mypthread_mutex_lock(m);
	*p = 6;
	mypthread_mutex_unlock(m);
	while(b < max){
		printf("%d. foo %d\n", b, killSomeTime(b));
		
		b++;
	}
	
	mypthread_exit(x);
}
void* beef(){
	printf("Beef waits for foo\n");
	mypthread_mutex_lock(m);
	*p = 7;
	mypthread_mutex_unlock(m);//
	struct thing* c;
	mypthread_join(*thread, &c);
	printf("Beef sees foo is finished. Beef received the value %d%d%d%d, Beef starts\n", (*c).a, (*c).b,(*c).c, (*c).d);
	free(c);
	int x = 0;
	while(x<max){
		printf("%d. beef %d\n", x, killSomeTime(x));
		x++;
	}
	int* d = malloc(sizeof(int));
	*d = 10; 
	pthread_exit(d);
}
void* miggy(){
	// printf("Miggy waits for foo\n");
	// mypthread_join(*thread, NULL);
	// printf("Miggy sees foo is finished. Miggy waits for beef\n");
	printf("asdf\n");
	mypthread_mutex_lock(m);
	*p = 8;
	mypthread_mutex_unlock(m);
	int* a;
	mypthread_join(*thread2, &a);
	if(a == NULL) printf("NULL MIG\n");
	printf("Miggy sees beef is finished %d. Miggy starts \n", *a);
	free(a);
	int x =0;
	while(x < max){
		printf("%d. miggy\n", x);
		x++;
	}
}
void* geet(){
	int x = 0;
	mypthread_mutex_lock(m);
	*p = *p + 1;
	mypthread_mutex_unlock(m);
	// printf("Geet waits for foo\n");
	// mypthread_join(*thread, NULL);
	// printf("Geet sees foo finished. Geet waits for beef\n");
	// mypthread_join(*thread2, NULL);
	// printf("Geet sees beef finished. Geet waits for miggy\n");
	mypthread_join(*thread0, NULL);
	printf("Geet sees miggy finished. Geetsauce\n");
	while(x < max){
		printf("geet\n");
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

	int x = 0;
	// printf("Foo: %d\nBeef: %d\nMiggy: %d\n", *thread, *thread2, *thread0);
	// printf("main waits for foo\n");
	// mypthread_join(*thread, NULL);
	// printf("main sees foo finished. main waits for beef\n");
	// mypthread_join(*thread2, NULL);
	// printf("main sees beef finished. main waits for miggy\n");
	// mypthread_join(*thread0, NULL);
	// printf("main sees miggy finished. main waits for geet\n");
	//mypthread_join(*thread4, NULL);
	mypthread_mutex_lock(m);
	mypthread_yield();
	mypthread_yield();
	mypthread_yield();
	mypthread_mutex_unlock(m);
	mypthread_join(*thread4, NULL);

	printf("main sees geet finished. main starts\n");

	while(x < max){
		printf("%d. krish\n", x);
		x++;
	}
	printf("P: %d\n", *p);
	free(thread);
	free(thread2);
	free(thread0);
	free(thread4);
	free(p);
	mypthread_mutex_destroy(m);
	free(m);
	return 0;
}
