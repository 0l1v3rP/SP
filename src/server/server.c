#include "../../headers/server/server.h"
#include "../../headers/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef _WIN32
    void close_socket(int socket) {
        closesocket(socket);
    }
#else
    void close_socket(int socket) {
        close(socket);
    }
#endif

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

void* handle_client_connection(void* arg) {
        ClientArgs* args = (ClientArgs*)arg;
        int client_socket = args->client_socket;
        pthread_mutex_t* db_mutex = args->db_mutex;
        free(args);

        Request req;
        Response res;
        while (1) {
            memset(&req, 0, sizeof(Request));
            memset(&res, 0, sizeof(Response));
            int bytes_read = recv_chunked(client_socket, (char*)&req, sizeof(Request));
            if (bytes_read <= 0) {
                printf("Client disconnected. Socket descriptor: %d\n", client_socket);
                break;
            }


            pthread_mutex_lock(db_mutex);
            handle_request(&req, &res);
            pthread_mutex_unlock(db_mutex);
            if (send_chunked(client_socket, (char*)&res, sizeof(Response)) < 0) {
                perror("Send failed");
                break;
            }
        }

        close_socket(client_socket);
        return NULL;
    }

void start_server() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            perror("WSAStartup failed");
            exit(1);
        }
    #endif

    int server_socket;
    struct sockaddr_in server_addr;
    pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Enable address reuse
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close_socket(server_socket);
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close_socket(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close_socket(server_socket);
        exit(1);
    }

    printf("Server running on port %d...\n", PORT);
    initialize_files();
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        printf("Waiting for a client...\n");
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected. Socket descriptor: %d\n", client_socket);

        // Create arguments structure
        ClientArgs* args = malloc(sizeof(ClientArgs));
        if (args == NULL) {
            perror("Memory allocation failed");
            close_socket(client_socket);
            continue;
        }
        args->client_socket = client_socket;
        args->db_mutex = &db_mutex;

        // Create new thread for client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client_connection, args) != 0) {
            perror("Thread creation failed");
            free(args);
            close_socket(client_socket);
            continue;
        }
        pthread_detach(thread);
    }

    close_socket(server_socket);
    pthread_mutex_destroy(&db_mutex);

    #ifdef _WIN32
        WSACleanup();
    #endif
}