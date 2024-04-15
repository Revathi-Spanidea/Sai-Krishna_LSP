#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdint.h>

#define SHM_KEY 12345 
#define SHM_SIZE 1024

typedef struct 
{
    int value;
    char message[64];
} SharedData;

int main() 
{
    int shmid;
    pid_t pid;
    SharedData *shared_data;
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Shared memory segment creation success\n");
    }
    shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) 
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    else
    {
	    printf("Shared memory attach was successfull\n");
    }
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("Child process writing to shared memory\n");
        shared_data->value = 42;
        sprintf(shared_data->message, "Hope for the best");
        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(NULL); 
        printf("Parent process reading from shared memory\n");
        printf("Value: %d\n", shared_data->value);
        printf("Message: %s\n", shared_data->message);
        if (shmdt(shared_data) == -1) 
	{
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
	else
	{
		printf("Shared memory Deatch was successfull\n");
	}
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
       	{
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

