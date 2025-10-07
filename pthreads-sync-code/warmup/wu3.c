#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/syscall.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
int done = 0;

void * thread_func(void * args){
    int x = rand()%10;
    sleep(x);
    int arg = *(int *)args;
    pthread_mutex_lock(&lock);
    while(done!=arg-1){
     //   printf("I, %d, waiting, current done = %d \n",arg,done);
        pthread_cond_wait(&cond,&lock);
    }
    pthread_mutex_unlock(&lock);
    
    
    printf("I am thread %d\n", arg);
    pthread_mutex_lock(&lock);
    done = arg;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
}

int main(){
    int N = 10;
    int arr[N];
    for(int i=0; i<N; i++){
        arr[i] = i+1;
    }

    pthread_t threads[N];

    for(int i=0; i<N; i++){
        pthread_create(&threads[i],NULL,thread_func,&arr[i]);
    }
    for(int i=0; i<N; i++){
        pthread_join(threads[i],NULL);
    }
    printf("I am the main thread\n");
}