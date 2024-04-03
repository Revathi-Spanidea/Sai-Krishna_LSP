#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define NUM_THREADS 5
#define NUM_INCRIMENTS 100000
int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *incriment_counter(void *thread_id)
{
    long tid = (long)thread_id;
    for(int i=0;i<NUM_INCRIMENTS;i++)
    {
	pthread_mutex_lock(&mutex);
	counter++;
	pthread_mutex_unlock(&mutex);
    }
    printf("Thread %ld finished. counter value %d\n",tid,counter);
    pthread_exit(NULL);
}
int main()
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long i;
    for(i=0;i<NUM_THREADS;i++)
    {
	rc = pthread_create(&threads[i], NULL, incriment_counter, (void *)i);
        if(rc != 0)
	{
	    printf("ERROR: in creating threads\n");
	    exit(1);
	}
    }
    for(i=0;i<NUM_THREADS;i++)
    {
	pthread_join(threads[i], NULL);
    }
    printf("Final counter value %d\n",counter);
    pthread_exit(NULL);

    return 0;
}

