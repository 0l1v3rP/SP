#ifndef SERVER_H
#define SERVER_H

#define MAX_USERS 100
#define MAX_USERNAME 32
#define MAX_PASSWORD 32
#define MAX_TABLES_PER_USER 10

#include "table.h" // Súbor, kde bude definovaná štruktúra Table

typedef struct {
    int user_id;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    Table *tables[MAX_TABLES_PER_USER]; // Ukazovatele na tabuľky
    int table_count;                   // Počet tabuliek
} User;

// Funkcie pre server
void start_server();
void handle_client(int client_socket);
int register_user(const char *username, const char *password);
int login_user(const char *username, const char *password);
int logout_user(const char *username);

// Funkcie pre správu tabuliek používateľa

int add_table_to_user(User *user, Table *table);
int remove_table_from_user(User *user, const char *table_name);

#endif
