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
    int client_socket, file_size;
    char buffer[1024], file_name[256];

    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        error("Error creating socket");
    }

    // Initialize server address struct
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  // Use the server's IP address
    server_address.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error("Error connecting to server");
    }

    // Get the name of the file from the user
    printf("Enter the name of the file you want: ");
    scanf("%s", file_name);

    // Send file name to server
    send(client_socket, file_name, sizeof(file_name), 0);

    // Receive file size from server
    recv(client_socket, &file_size, sizeof(int), 0);
    printf("File size: %d bytes\n", file_size);

    // Receive and save file content
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        error("Error creating file");
    }

    int total_received = 0;
    while (total_received < file_size) {
        int received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            break;
        }
        fwrite(buffer, 1, received, file);
        total_received += received;
    }

    printf("File received successfully\n");

    // Close sockets and file
    fclose(file);
    close(client_socket);

    return 0;
}
