#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX_MSG_SIZE 1024

#define READ_REQUEST 0
#define WRITE_REQUEST 1
#define READ_RESPONSE 2
#define WRITE_RESPONSE 3

typedef struct 
{
    int type;
    int address;
    int data;
} Message;

int main() 
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_MSG_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) 
    {
        perror("listen Failed\n");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) 
    {
        perror("accept is failed\n");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        Message msg;
        if (recv(new_socket, &msg, sizeof(msg), 0) < 0) 
	{
            perror("Failed to read from client\n");
            exit(EXIT_FAILURE);
        }

        switch (msg.type) 
	{
            case READ_REQUEST:
                printf("Received read request for address %d\n", msg.address);
                break;
            case WRITE_REQUEST:
                printf("Received write request for address %d with data %d\n", msg.address, msg.data);
                break;
            default:
                printf("Invalid message type\n");
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}

