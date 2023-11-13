#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int log_socket, port = 54321;
    struct sockaddr_in log_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    if ((log_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&log_addr, 0, sizeof(log_addr));

    log_addr.sin_family = AF_INET;
    log_addr.sin_addr.s_addr = INADDR_ANY;
    log_addr.sin_port = htons(port);

    if (bind(log_socket, (const struct sockaddr*)&log_addr, sizeof(log_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Log_s listening on 127.0.0.1:%d\n", port);

    while (1) {
        ssize_t len = recvfrom(log_socket, (char*)buffer, sizeof(buffer), 0,
                               (struct sockaddr*)&client_addr, &client_len);

        printf("Received log from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        FILE* log_file = fopen("logs.txt", "a");
        if (log_file) {
            buffer[len] = '\0';
            fprintf(log_file, "%s\n", buffer);
            fclose(log_file);
        }
    }

    return 0;
}
