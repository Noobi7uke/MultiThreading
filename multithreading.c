#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

volatile long int count = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct thread_msg{
    int start;
    int end;
    char msg[50];
};



void *func(void *ptr){
    
    struct thread_msg* T = (struct thread_msg *)ptr;
    
    for(int i=T->start;i<T->end;i++){
        pthread_mutex_lock( &mutex1 );
        long int tmp = count;
        tmp = tmp+i;
        count = tmp;
	    pthread_mutex_unlock( &mutex1 );
    }
    return NULL;
}

int main(){
    pthread_t th1, th2;
    int iret1, iret2;
    char msg1[] = "Thread 1\0";
    char msg2[] = "Thread 2\0";

    printf("%s< >%s \n",msg1, msg2);

    struct thread_msg *t1, *t2;
    t1 = (struct thread_msg *)malloc(sizeof(struct thread_msg));
    t2 = (struct thread_msg *)malloc(sizeof(struct thread_msg));

    t1->start = 0;
    t1->end = 500000;
    strcpy(t1->msg, msg1);

    t2->start = 500000;
    t2->end = 100000000;
    strcpy(t2->msg, msg2);



    iret1 = pthread_create(&th1, NULL, func, (void *)t1);
    iret2 = pthread_create(&th2, NULL, func, (void *)t2);

    if(iret1){
	    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
	    exit(EXIT_FAILURE);
	}
    if(iret2){
	    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
	    exit(EXIT_FAILURE);
	}

    pthread_join(th1, NULL);
    printf("Thread1 completed %ld\n", count);
    pthread_join(th2, NULL);
    printf("Thread2 completed %ld\n", count);

    printf("Finally - %ld", count);

    exit(EXIT_SUCCESS);
}