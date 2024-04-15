#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void slaveProcess(int id)
{
    printf("Slave process %d started\n", id);
                                                     
    sleep(1);      
                                
    printf("Slave process %d completed\n", id);
}

int main()
{
    int numSlaves = 3;                         

                                                 
    for (int i = 0; i < numSlaves; i++)
    {
        pid_t pid = fork();
        if (pid < 0) 
	{
            perror("fork");
            exit(EXIT_FAILURE);
        } 
	else if (pid == 0)
       	{
                                                      
            slaveProcess(i);
            exit(EXIT_SUCCESS);
        }
    }
                                                        
    for (int i = 0; i < numSlaves; i++) 
    {
        wait(NULL);
    }

                                                            
    printf("All slave processes completed\n");

    return 0;
}

