#ifndef SHAREDCOM_H
#define SHAREDCOM_H

#define MAX_CHUNK_SIZE 8192
#define MAX_COMMAND_LENGTH 32
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>

typedef struct {
    char command[MAX_COMMAND_LENGTH];
    char data[MAX_CHUNK_SIZE];
    int session_id;
} Request;

typedef struct {
    char data[MAX_CHUNK_SIZE];
    int session_id;
} Response;

int send_chunked(int socket, const char* data, size_t total_size);
int recv_chunked(int socket, char* buffer, size_t max_size);
#endif //SHAREDCOM_H
