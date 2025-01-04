#ifndef SERVER_H
#define SERVER_H

#define MAX_USERS 100
#define MAX_USERNAME 32
#define MAX_PASSWORD 32


typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} User;

void start_server();
void handle_client(int client_socket);
int register_user(const char *username, const char *password);
int login_user(const char *username, const char *password);
int logout_user(const char *username);

#endif