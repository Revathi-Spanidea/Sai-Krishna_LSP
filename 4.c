#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    int num_commands = argc-1;
    for(int i=1;i<=num_commands;i++)
    {
	pid = fork();
	if(pid < 0)
	{
	    perror("fork");
	    return 1;
	}
	else if(pid == 0)
	{
            printf("the child process %d is created for the %s command\n",getpid(),argv[i]);
	    execlp(argv[i], argv[i], NULL);
	    perror("execlp");
	    exit(1);
	}
    }
    for(int i=0;i<num_commands;i++)
    {
	wait(NULL);
    }
    return 0;
}

