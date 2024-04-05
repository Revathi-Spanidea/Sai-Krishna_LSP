#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080
void reverse_str(char *str)
{
    int i,j;
    for(i=0,j=strlen(str)-1;i<j;i++,j--)
    {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}
int main()
{
    int opt = 1;
    int new_socket;
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[100] = {0};
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("creating socket");
        exit(1);
    }
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if(bind(server_fd, (struct sockaddr *)&address,sizeof(address)) < 0)
    {
        perror("bind");
        exit(1);
    }
    if((listen(server_fd,3)) < 0)
    {
        perror("listen");
        exit(1);
    }
    printf("server listening on port %d\n",PORT);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if(new_socket < 0) 
    {
        perror("accept");
        exit(1);
    }
    read(new_socket, buffer, sizeof(buffer));
    printf("message received by the client %s\n",buffer);
    reverse_str(buffer);
    send(new_socket, buffer, strlen(buffer), 0);
    printf("The reversed message has been sent to client\n");
    return 0;
}
