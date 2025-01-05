#include "client.h"
#include "config.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <io.h>
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
    }

    while (running) {
        printf("Enter command: ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = 0; // Odstráni nový riadok

        if (strlen(buffer) == 0) {
            printf("Empty command, try again.\n");
            continue;
        }

        if (strcmp(buffer, "EXIT") == 0) {
            running = 0;
            printf("Exiting client...\n");
            break;
        }

        // Odosielanie príkazu serveru
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            running = 0;
            break;
        }
        printf("Sent command: %s\n", buffer);

        // Prijímanie odpovede od servera
        memset(buffer, 0, sizeof(buffer)); // Vyčistenie bufferu
        int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Ukončovací znak
            printf("Server response: %s\n", buffer);
        } else if (bytes_read == 0) {
            printf("Server closed the connection.\n");
            running = 0; // Ukončí slučku
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
