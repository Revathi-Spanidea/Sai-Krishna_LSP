#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/resource.h>
#define NUM_PROCESSES 3
int main() 
{
    pid_t pid;
    int i, status;
    int priorities[NUM_PROCESSES] = {10, 5, 0}; 
    printf("Parent process ID: %d\n", getpid());
    for (i = 0; i < NUM_PROCESSES; i++) 
    {
        pid = fork();
        if (pid < 0) 
	{
            perror("fork");
            exit(EXIT_FAILURE);
        } 
	else if (pid == 0) 
	{ 
            if (nice(priorities[i]) == -1) 
	    {
                perror("nice");
                exit(EXIT_FAILURE);
            }
            printf("Child process ID: %d, Priority: %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
            for (int j = 0; j < 10000000; j++) {}

            exit(EXIT_SUCCESS);
        }
    }
    for (i = 0; i < NUM_PROCESSES; i++) 
    {
        if (wait(&status) == -1) 
	{
            perror("wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) 
	{
            printf("Child process with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
        } 
	else 
	{
            printf("Child process with PID %d exited abnormally\n", pid);
        }
    }

    return 0;
}
