#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h> 

#define NUM_TASKS 10
#define NUM_SLAVES 3

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
        sleep(1);
    }

    printf("Slave %d has finished its tasks\n", id);
    pthread_exit(NULL);
}
int main()
{
    pthread_t threads[NUM_SLAVES];
    int slave_ids[NUM_SLAVES];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        slave_ids[i] = i;
        pthread_create(&threads[i], NULL, slave, &slave_ids[i]);
    }

    for (int i = 0; i < NUM_SLAVES; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("All tasks completed\n");

    return 0;
}

