#include<pthread.h>
#include<stdio.h>

int counter=0;
pthread_mutex_t lock;

void *threadfun(void* arg){
    for(int i=0; i<1000; i++){
        pthread_mutex_lock(&lock);
        counter+=1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    pthread_t ps[1000];
    int rc = pthread_mutex_init(&lock,NULL);
    for(int i=0; i<1000; i++){
        pthread_create(&ps[i], NULL, threadfun, NULL);
    }
    for(int i=0; i<1000; i++){
        pthread_join(ps[i], NULL);
    }
    printf("counter = %d\n", counter);
    pthread_mutex_destroy(&lock);
}