#ifndef HANDLE_COM_H
#define HANDLE_COM_H

#define MAX_CHUNK_SIZE 8192

typedef struct {
    char command[32];
    char data[MAX_CHUNK_SIZE];
    size_t data_size;
    int session_id;  // 0 means not signed in
} Request;



int send_chunked(int socket, const char* data, size_t total_size);
int recv_chunked(int socket, char* buffer, size_t max_size);
void print_manual();

#endif //HANDLE_COM_H
