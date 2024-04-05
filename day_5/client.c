#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#define PORT 8080
int main()
{
    int sock = 0;
    struct sockaddr_in server_adr;
    char buffer[100];
    char message[100] = {0};
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket");
        exit(1);
    }
    server_adr.sin_family = AF_INET;
    server_adr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &server_adr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(1);
    }
    if(connect(sock, (struct sockaddr *)&server_adr,sizeof(server_adr)) < 0)
    {
        perror("connection failed");
        exit(1);
    }
    printf("Enter message to send to server :");
    scanf("%[^\n]",message);
    send(sock, message, strlen(message), 0);
    printf("Message sent to server %s\n",message);
    read(sock, buffer, sizeof(buffer));
    printf("The reversed message is %s\n",buffer);
    return 0;
} 

