#include<pthread.h>
#include<stdio.h>


void * thread_func(void * args){
    int arg = *(int *)args;
    printf("I am thread %d\n", arg);
}

int main(){
    int N = 15;
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