#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int client_socket;
    char command[1024], buffer[1024];
    struct sockaddr_in server_address;


    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        error("Error creating socket");
    }


    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_address.sin_port = htons(PORT);

    while (1) {
        printf("Enter command (or 'exit' to quit): ");
        fgets(command, sizeof(command), stdin);


        command[strcspn(command, "\n")] = '\0';

        sendto(client_socket, command, strlen(command), 0, (struct sockaddr*)&server_address, sizeof(server_address));

  
        if (strcmp(command, "exit") == 0) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        printf("Server response: %s\n", buffer);
    }


    close(client_socket);

    return 0;
}
