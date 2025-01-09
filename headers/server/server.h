#ifndef SERVER_H
#define SERVER_H

#define MAX_USERS 100
#define MAX_USERNAME 32
#define MAX_PASSWORD 32
#define MAX_TABLES_PER_USER 10

void start_server();
void handle_client(int client_socket);

#endif
