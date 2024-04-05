#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define FILE_PATH "example.txt"
#define FILE_SIZE 100

int main() 
{
    int fd;
    char *file_data;

    fd = open(FILE_PATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    if (lseek(fd, FILE_SIZE - 1, SEEK_SET) == -1) 
    {
        perror("Error calling lseek to 'stretch' the file");
        exit(EXIT_FAILURE);
    }
    if (write(fd, "", 1) == -1) 
    {
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    file_data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_data == MAP_FAILED) 
    {
        perror("Error mapping file to memory");
        exit(EXIT_FAILURE);
    }
    strcpy(file_data, "Hello, Memory Mapping!");
    if (munmap(file_data, FILE_SIZE) == -1) 
    {
        perror("Error unmapping file from memory");
        exit(EXIT_FAILURE);
    }
    if (close(fd) == -1) 
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
    printf("Memory mapping and modification successful!\n");
    return 0;
}
