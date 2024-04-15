#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define SHM_SIZE 1024
int main() 
{
    key_t key = 1234;
    int shmid;
    char *shmaddr;
    char *message = "Hello, shared memory!";
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
    {
        perror("shmget");
        exit(1);
    }
    else
    {
	printf("Shared memory created\n");
    }
    if ((shmaddr = shmat(shmid, NULL, 0)) == (char *) -1) 
    {
        perror("shmat");
        exit(1);
    }
    else
    {
	printf("Shared memory is attached to sender\n");
    }
    strcpy(shmaddr, message);
    pid_t pid = fork();
    if (pid == 0) 
    {
        printf("Child process reads: %s\n", shmaddr);
    } 
    else if (pid > 0) 
    {
        wait(NULL);
        printf("Parent process reads: %s\n", shmaddr);

        shmdt(shmaddr);
        shmctl(shmid, IPC_RMID, NULL);
    } 
    else 
    {
        perror("fork");
        exit(1);
    }

    return 0;
}

