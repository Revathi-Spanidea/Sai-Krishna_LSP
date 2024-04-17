#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 5
#define DATA_SIZE 1000000 
#define NUM_ITERATIONS 10000 

int shared_data[DATA_SIZE]; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void process_data(int thread_id) 
{
    for (int i = 0; i < NUM_ITERATIONS; i++) 
    {
        int index = rand() % DATA_SIZE; 
        pthread_mutex_lock(&mutex);
        shared_data[index]++; 
        pthread_mutex_unlock(&mutex);
    }
}
void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
    process_data(thread_id);
    pthread_exit(NULL);
}

int main() 
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    struct timeval start_time, end_time;
    double total_time;

    for (int i = 0; i < DATA_SIZE; i++) {
        shared_data[i] = 0;
    }

    gettimeofday(&start_time, NULL);

    for (int i = 0; i < NUM_THREADS; i++) 
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]) != 0) 
	{
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) 
    {
        if (pthread_join(threads[i], NULL) != 0) 
	{
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&end_time, NULL);

    total_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0; 
    total_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0; 

    printf("Total time taken: %.2f milliseconds\n", total_time);

    pthread_mutex_destroy(&mutex);

    return 0;
}

