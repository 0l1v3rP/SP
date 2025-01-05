#include "server.h"
#include "client.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server|client> [server_ip]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        start_server();
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s client <server_ip>\n", argv[0]);
            return 1;
        }
        start_client(argv[2]);
    } else {
        fprintf(stderr, "Invalid mode. Use 'server' or 'client'.\n");
        return 1;
    }

    return 0;
}