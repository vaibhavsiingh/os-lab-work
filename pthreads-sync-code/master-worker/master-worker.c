#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
// #include <wait.h>
#include <pthread.h>

int item_to_produce, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;
int item_consumed;
int buffer_full;

int *buffer;

pthread_mutex_t lock, cond_lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {
      pthread_mutex_lock(&lock);

      for(int i=0; i<max_buf_size; i++){
        if(buffer[i]==-1){
          if(item_to_produce >= total_items) {       
            pthread_mutex_unlock(&lock);
            return 0;
          }
          buffer[i] = item_to_produce;
          curr_buf_size++;
          print_produced(item_to_produce, thread_id);
          item_to_produce++;
        }
      }
      if(curr_buf_size>= max_buf_size){
        // pthread_mutex_unlock(&lock);
        pthread_cond_wait(&cond,&lock);
        
      }
      pthread_mutex_unlock(&lock);
    }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item
void* worker_consume(void* arg){
  while(1){
    int thread_id = *((int *)arg);
    pthread_mutex_lock(&lock);
  
    if(item_consumed >= total_items) {

      pthread_mutex_unlock(&lock);
      break;
    }
    for(int i=0; i<max_buf_size; i+=1){

      if(buffer[i]!=-1){
        print_consumed(buffer[i],thread_id);
        buffer[i] = -1;  
        item_consumed++;
        curr_buf_size--;
        pthread_cond_broadcast(&cond); 
      }
    }
    pthread_mutex_unlock(&lock);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int *master_thread_id;
  pthread_t *master_thread;
  item_to_produce = 0;
  curr_buf_size = 0;

  int rc = pthread_mutex_init(&lock, NULL);
  rc = pthread_mutex_init(&cond_lock, NULL);
  
  int i;
  
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

   buffer = (int *)malloc (sizeof(int) * max_buf_size);

   for(int i=0; i<max_buf_size; i++) buffer[i] = -1;

   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  //create worker consumer threads
  int *worker_thread_id;
  pthread_t *worker_thread;
  //printf("Reached\n");
  worker_thread_id = (int *)malloc(sizeof(int)*num_workers);
  worker_thread = (pthread_t *)malloc(sizeof(pthread_t)*num_workers);
  for (i = 0; i < num_workers; i++)
    worker_thread_id[i] = i+num_masters;
  
  for (i = 0; i < num_workers; i++)
    pthread_create(&worker_thread[i], NULL, worker_consume, (void *)&worker_thread_id[i]);
  
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }
    for (i = 0; i < num_workers; i++)
    {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", worker_thread_id[i]);
    }
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  free(worker_thread_id);
  free(worker_thread);
  
  return 0;
}
