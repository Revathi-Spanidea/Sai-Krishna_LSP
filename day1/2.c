#include <stdlib.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
    char *source_file = argv[1];
    int fd;
    fd = open(source_file,O_CREAT|O_RDONLY,0444);
    if(fd == -1)
    {
	printf("the file permission not changed\n");
	return 1;
    }
    close(fd);
    if(chmod(source_file,0644)==-1)
    {
	perror("chomd");
	return 1;
    }
    printf("The file %s created read only for owner and after changed to write\n",argv[1]);

    return 0;
}

