#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2]; 
    pid_t pid;
    char buffer[BUFFER_SIZE];
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) 
    { 
        close(pipefd[1]); 
        ssize_t num_bytes = read(pipefd[0], buffer, BUFFER_SIZE);
        if (num_bytes == -1) 
	{
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Child process received message: %s", buffer);
        close(pipefd[0]); 
        exit(EXIT_SUCCESS);
    }
    else 
    { 
        close(pipefd[0]); 
        char message[] = "Hello from the parent process!";
        ssize_t num_bytes_written = write(pipefd[1], message, sizeof(message));
        if (num_bytes_written == -1) 
	{
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); 
    }
    return 0;
}
