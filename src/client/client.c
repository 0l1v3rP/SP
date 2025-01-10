#include "../../headers/client/client.h"
#include "../../headers/client/comHandler.h"
#include "../../headers/config.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void start_client(const char *server_ip) {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[256];
    int running = 1;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(1);
    }
#endif

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        exit(1);
    } else {
        printf("Connected to server at %s:%d\n", server_ip, PORT);
        printf("press:\n\t 'help' for manual\n\t 'exit' for exiting program\n");

    }

    Request req;
    req.session_id = 0;  // Start unsigned
    char response[MAX_CHUNK_SIZE];

    // Initial session check
    req.data[0] = '\0';
    strcpy(req.command, "session_check");
    req.data_size = 0;

    if (send_chunked(client_socket, (char*)&req, sizeof(Request)) < 0) {
        perror("Initial session check failed");
        exit(1);
    }

    int bytes_read = recv_chunked(client_socket, response, MAX_CHUNK_SIZE);
    if (bytes_read > 0) {
        req.session_id = atoi(response);
    }

    while (running) {
        printf("Enter Command: ");
        fgets(req.command, sizeof(req.command), stdin);
        req.command[strcspn(req.command, "\n")] = 0;


        if (strcmp(req.command, "exit") == 0) {
            running = 0;
            printf("Exiting client...\n");
            break;
        }

        if (strcmp(req.command, "help") == 0) {
            print_manual();
            continue;
        }

        printf("Enter Data: ");
        fgets(req.data, sizeof(req.data), stdin);

        req.data[strcspn(req.data, "\n")] = 0;
        req.data_size = strlen(req.data);

        if (send_chunked(client_socket, (char*)&req, sizeof(Request)) < 0) {
            perror("Send failed");
            running = 0;
            break;
        }
        printf("Sent command: %s\n", req.command);

        int bytes_read = recv_chunked(client_socket, response, MAX_CHUNK_SIZE);
        if (bytes_read > 0) {
            printf("Server response: %s\n", response);
        } else if (bytes_read == 0) {
            printf("Server closed the connection.\n");
            running = 0;
        } else {
            perror("Recv failed");
            running = 0;
        }
    }


#ifdef _WIN32
    closesocket(client_socket);
    WSACleanup();
#else
    close(client_socket);
#endif
}
