#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc,char *argv[])
{
    if(argc < 2)
    {
	printf("Enter the arguments correctly\n");
	return 1;
    }
    int fd,fd1;
    char *source_file = argv[1];
    char *dest_file = argv[2];
    char buff[1024];
    ssize_t read_bytes,write_bytes;
    fd = open(source_file,O_RDONLY);
    if(fd == -1)
    {
	perror("opening file");
	close(fd);
	return 1;
    }
    fd1 = open(dest_file,O_CREAT|O_WRONLY|O_TRUNC,0644);
    if(fd1 == -1)
    {
	perror("open");
	close(fd1);
	return 1;
    }
    
    while((read_bytes = read(fd,buff,sizeof(buff))) > 0)
    {
	write_bytes = write(fd1,buff,read_bytes);
	if(write_bytes != read_bytes)
	{
	    perror("write");
	    close(fd);
	    close(fd1);
	    return 1;
	}
    }
    if(read_bytes == -1)
    {
	perror("read");
	close(fd);
	close(fd1);
	return 1;
    }
    close(fd);
    close(fd1);
    return 0;
}

