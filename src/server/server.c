#include "../../headers/server/server.h"
#include "../../headers/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

User users[MAX_USERS];
int user_count = 0;

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

        // Volanie spracovania klienta
        handle_client(client_socket);
    }

    close(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
}



void handle_client(int client_socket) {
    char buffer[256];
    char response[256];
    memset(buffer, 0, sizeof(buffer));

    printf("Waiting to receive data from client...\n");
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read < 0) {
        perror("Recv failed");
        printf("Client socket descriptor: %d\n", client_socket);
        close(client_socket);
        return;
    } else if (bytes_read == 0) {
        printf("Client disconnected. Socket descriptor: %d\n", client_socket);
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0'; // Ukončovací znak pre reťazec
    printf("Data received: %s (Bytes read: %d)\n", buffer, bytes_read);

    snprintf(response, sizeof(response), "ACK: %s\n", buffer);
    int bytes_written = send(client_socket, response, strlen(response), 0);
    if (bytes_written < 0) {
        perror("Send failed");
    } else {
        printf("Response sent: %s (Bytes written: %d)\n", response, bytes_written);
    }

    close(client_socket); // Uzavretie klientského socketu
}


// toto bolo funkčne isto

/*int register_user(const char *username, const char *password) {
    if (strlen(username) == 0 || strlen(password) == 0) {
        return 0; // Invalid input
    }


    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0; // Užívateľské meno už existuje
        }
    }

    // Bezpečné kopírovanie používateľského mena
    strncpy(users[user_count].username, username, MAX_USERNAME - 1);
    users[user_count].username[MAX_USERNAME - 1] = '\0'; // Ukončovací znak

    // Bezpečné kopírovanie hesla
    strncpy(users[user_count].password, password, MAX_PASSWORD - 1);
    users[user_count].password[MAX_PASSWORD - 1] = '\0'; // Ukončovací znak

    user_count++;
    return 1; // Registrácia úspešná
}*/

int register_user(const char *username, const char *password) {
    if (user_count >= MAX_USERS) {
        printf("User limit reached.\n");
        return -1;
    }

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0; // Užívateľské meno už existuje
        }
    }

    User *user = &users[user_count++];
    user->user_id = user_count; // Jednoduchý prístup k ID
    strncpy(user->username, username, MAX_USERNAME);
    strncpy(user->password, password, MAX_PASSWORD);
    user->table_count = 0;
    return 0;
}



int login_user(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

int logout_user(const char *username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_table_to_user(User *user, Table *table) {
    if (user->table_count >= MAX_TABLES_PER_USER) {
        printf("User %s already has the maximum number of tables.\n", user->username);
        return -1; // Chyba: Prekročený limit
    }
    user->tables[user->table_count++] = table;
    return 0; // Úspech
}

int remove_table_from_user(User *user, const char *table_name) {
    for (int i = 0; i < user->table_count; i++) {
        if (strcmp(user->tables[i]->name, table_name) == 0) {
            // Uvoľnenie pamäte tabuľky
            table_delete(user->tables[i]);
            // Posunutie zostávajúcich tabuliek
            for (int j = i; j < user->table_count - 1; j++) {
                user->tables[j] = user->tables[j + 1];
            }
            user->table_count--;
            return 0; // Úspech
        }
    }
    printf("Table %s not found for user %s.\n", table_name, user->username);
    return -1; // Chyba: Tabuľka nenájdená
}
