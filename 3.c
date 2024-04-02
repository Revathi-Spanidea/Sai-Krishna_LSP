#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    char *path = getenv("PATH");
    if(path == NULL)
    {
	printf("Error: The PATH environmental variable is not found.\n");
	return 1;
    }
    printf("the path of the environmental variable is %s\n",path);
    char *my_var = "MY_PATH";
    char *my_var_path = "bin/other:/boot/dev";
    if(setenv(my_var, my_var_path, 1) != 0)
    {
	perror("setting envronmental variable\n");
	return 1;
    }
    else
    {
    	printf("The newly seted path for my_env_var is %s\n",my_var_path);
    }
    return 0;
}
