#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    const char* message = "Hello, Echo Server!";
    sendto(client_socket, message, strlen(message), 0, (const struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Message sent to Echo Server\n");

    ssize_t len = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
    buffer[len] = '\0';

    printf("Echoed data: %s\n", buffer);

    close(client_socket);

    return 0;
}
