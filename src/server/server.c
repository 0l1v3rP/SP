#include "../../headers/server/server.h"
#include "../../headers/server/comHandler.h"
#include "../../headers/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

void start_server() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            perror("WSAStartup failed");
            exit(1);
        }
    #endif

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Server running on port %d...\n", PORT);

    while (1) {
        printf("Waiting for a client...\n");
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue; // Pokračuj, ak accept zlyhá
        }

        printf("Client connected. Socket descriptor: %d\n", client_socket);
        handle_client(client_socket);
    }

    close(server_socket);
    #ifdef _WIN32
        WSACleanup();
    #endif
}

void handle_client(int client_socket) {
    Request req;
    char response[MAX_CHUNK_SIZE];

    while (1) {
        memset(&req, 0, sizeof(Request));
        int bytes_read = recv(client_socket, (char*)&req, sizeof(Request), 0);
        if (bytes_read <= 0) break;

        handle_request(req, response);

        if (send(client_socket, response, strlen(response), 0) < 0) {
            break;
        }
    }
    close(client_socket);
}


void initialize_files() {
    const char *const filenames[] = {
        "users.dat",
        "sessions.dat",
        "table2user.dat"
    };

    for (size_t i = 0; i < sizeof(filenames) / sizeof(*filenames); ++i) {
        FILE *file = fopen(filenames[i], "ab+");
        if (file == NULL) {
            fprintf(stderr, "Failed to create or open %s\n", filenames[i]);
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }
}