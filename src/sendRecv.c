#include "../headers/sendRecv.h"
int send_chunked(int socket, const char* data, size_t total_size) {
    size_t sent = 0;
    while (sent < total_size) {
        size_t chunk_size = total_size - sent;
        if (chunk_size > MAX_CHUNK_SIZE) {
            chunk_size = MAX_CHUNK_SIZE;
        }

        int bytes = send(socket, data + sent, chunk_size, 0);
        if (bytes < 0) {
            return -1;
        }
        sent += bytes;
    }
    return sent;
}

int recv_chunked(int socket, char* buffer, size_t max_size) {
    size_t total_received = 0;
    while (total_received < max_size) {
        int bytes = recv(socket, buffer + total_received,
                        MAX_CHUNK_SIZE, 0);
        if (bytes <= 0) {
            return bytes;  // Error or connection closed
        }
        total_received += bytes;

        // Check for message completion
        if (bytes < MAX_CHUNK_SIZE) {
            break;
        }
    }
    buffer[total_received] = '\0';
    return total_received;
}