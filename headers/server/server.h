#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif
#include <pthread.h>

#include "./handlers/commonHandler.h"
#include <bits/pthreadtypes.h>

typedef struct {
    int client_socket;
    pthread_mutex_t* db_mutex;
} ClientArgs;

// Prototypes
void* handle_client_connection(void* arg);
void start_server(void);
#endif
