#ifndef COM_HANDLE_H
#define COM_HANDLE_H
#define MAX_CHUNK_SIZE 8192
#define MAX_USERNAME 64
#define MAX_PASSWORD 64
#include <stddef.h>

typedef struct {
    char command[32];
    char data[MAX_CHUNK_SIZE];
    size_t data_size;
    int session_id;
} Request;

typedef void (*CommandHandler)(Request, char*);

typedef struct {
    const char* command;
    CommandHandler handler;
} CommandMapping;



void handle_request(Request req, char response[MAX_CHUNK_SIZE]);
void handle_register(Request req, char* response);
void handle_login(Request req, char* response);
void handle_logout(Request req, char* response);
void handle_session_check(Request req, char* response);
void handle_delete_user(Request req, char* response);

static const CommandMapping COMMANDS[] = {
    {"register", handle_register},
    {"login", handle_login},
    {"logout", handle_logout},
    {"session_check", handle_session_check},
    {"delete_user", handle_delete_user},
};
#endif //COM_HANDLE_H
