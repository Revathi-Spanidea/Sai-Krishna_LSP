#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
void create_Directory(const char *dirname)
{
    if(mkdir(dirname, 0777) == -1)
    {
	perror("mkdir");
	exit(1);
    }
    printf("Directory %s created successfully\n",dirname);
}
void list_Directory(const char *dirname)
{
    int count=0;
    DIR *dir;
    struct dirent *entry;
    if((dir = opendir(dirname)) == NULL)
    {
	perror("open");
	exit(1);
    }
    printf("contents of directory %s\n",dirname);
    while((entry = readdir(dir)) != NULL)
    {
	printf("%s\n",entry->d_name);
	count++;
    }
    closedir(dir);
}
void delete_File(const char *filename)
{
    if(unlink(filename) == -1)
    {
	perror("unlink");
	exit(1);
    }
    printf("The file %s deleted successfully\n",filename);
}

int main()
{
    const char *dirname = "test_dir";
    const char *filename = "test_dir/file.txt";
    create_Directory(dirname);
    list_Directory(dirname);
    FILE *file = fopen(filename,"w");
    if(file == NULL)
    {
	printf("Error in opening file\n");
	return 1;
    }
    printf("file %s created in write mode successfully\n",filename);
    fclose(file);
    list_Directory(dirname);
    delete_File(filename);
    list_Directory(dirname);
    return 0;
}

