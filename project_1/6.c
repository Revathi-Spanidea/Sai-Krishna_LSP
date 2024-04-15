#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_TASKS 50
#define NUM_SLAVES 2

pthread_mutex_t lock;                
int tasks_completed = 0;            

void* slave(void* arg) 
{
    int id = *((int*)arg);
    while (1) 
    {
        pthread_mutex_lock(&lock);              
        if (tasks_completed >= NUM_TASKS) 
	{
            pthread_mutex_unlock(&lock);        
            break;
        }
        printf("Slave %d is working on task %d\n", id, tasks_completed); 
        tasks_completed++; 
        pthread_mutex_unlock(&lock);
    }
    printf("Slave %d has finished its tasks\n", id);
    pthread_exit(NULL);
}

int main() 
{
    pthread_t threads[NUM_SLAVES];
    struct timeval start, end; 
    double execution_time;

    pthread_mutex_init(&lock, NULL);  
    gettimeofday(&start, NULL);  
    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        int* id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, slave, (void*)id);
    }

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    pthread_mutex_destroy(&lock);

    execution_time = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 50;
    printf("Total execution time: %.6f seconds\n", execution_time);

    return 0;
}

