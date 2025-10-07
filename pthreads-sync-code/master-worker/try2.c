#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int item_to_produce = 0, curr_buf_size = 0;
int total_items, max_buf_size, num_workers, num_masters;
int item_consumed = 0;

int *buffer;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;

void print_produced(int num, int master) {
    printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {
    printf("Consumed %d by worker %d\n", num, worker);
}

// Producer function
void *generate_requests_loop(void *data) {
    int thread_id = *((int *)data);

    while (1) {
        pthread_mutex_lock(&lock);

        // Stop when all items have been produced
        if (item_to_produce >= total_items) {
            pthread_mutex_unlock(&lock);
            break;
        }

        // Wait if buffer is full
        while (curr_buf_size >= max_buf_size) {
            pthread_cond_wait(&cond_full, &lock);
        }

        // Produce an item
        buffer[curr_buf_size] = item_to_produce;
        print_produced(item_to_produce, thread_id);
        item_to_produce++;
        curr_buf_size++;

        // Signal a worker that an item is available
        pthread_cond_signal(&cond_empty);

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Consumer function
void *worker_consume(void *data) {
    int thread_id = *((int *)data);

    while (1) {
        pthread_mutex_lock(&lock);

        // Stop when all items have been consumed
        if (item_consumed >= total_items) {
            pthread_mutex_unlock(&lock);
            break;
        }

        // Wait if buffer is empty
        while (curr_buf_size == 0) {
            pthread_cond_wait(&cond_empty, &lock);
        }

        // Consume an item
        int consumed_item = buffer[curr_buf_size - 1];
        buffer[curr_buf_size - 1] = -1;
        print_consumed(consumed_item, thread_id);
        item_consumed++;
        curr_buf_size--;

        // Signal a producer that space is available
        pthread_cond_signal(&cond_full);

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: ./master-worker <total_items> <max_buf_size> <num_workers> <num_masters>\n");
        exit(1);
    }

    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
    num_workers = atoi(argv[3]);
    num_masters = atoi(argv[4]);

    buffer = (int *)malloc(sizeof(int) * max_buf_size);
    for (int i = 0; i < max_buf_size; i++) buffer[i] = -1;

    pthread_t master_threads[num_masters], worker_threads[num_workers];
    int master_thread_id[num_masters], worker_thread_id[num_workers];

    // Create master threads
    for (int i = 0; i < num_masters; i++) {
        master_thread_id[i] = i;
        pthread_create(&master_threads[i], NULL, generate_requests_loop, &master_thread_id[i]);
    }

    // Create worker threads
    for (int i = 0; i < num_workers; i++) {
        worker_thread_id[i] = i;
        pthread_create(&worker_threads[i], NULL, worker_consume, &worker_thread_id[i]);
    }

    // Join master threads first
    for (int i = 0; i < num_masters; i++) {
        pthread_join(master_threads[i], NULL);
        printf("Master %d joined\n", i);
    }

    // Join worker threads after producers finish
    for (int i = 0; i < num_workers; i++) {
        pthread_join(worker_threads[i], NULL);
        printf("Worker %d joined\n", i);
    }

    free(buffer);
    return 0;
}
