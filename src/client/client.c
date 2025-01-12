#include "../../headers/client/client.h"
#include "../../headers/sendRecv.h"
#include "../../headers/config.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_manual() {
    printf(
        "---------------------------------MANUAL_START--------------------------------------\n"
        "USER:\n"
        "\tCOMMAND: register\n"
        "\t\tData: {username} {password} \n\n"
        "\tCOMMAND: login\n"
        "\t\tData: {username} {password} \n\n"
        "\tCOMMAND: logout\n"
        "\t\tData: ENTER \n\n"
        "\tCOMMAND: delete_user\n"
        "\t\tData: ENTER \n\n"
        "TABLE:\n"
        "\tCOMMAND: create_table\n"
        "\t\tData: {table_name} {[{column_name1} {column_type1}] [{column_name2} {column_type2}]...} \n"
        "\t\t\tType:  {string} {int} {double} {boolean(e.g. false true)} {date(e.g. 2023-01-01)}\n\n"
        "\tCOMMAND: delete_table\n"
        "\t\tData: {table_name} \n\n"
        "\tCOMMAND: add_record\n"
        "\t\tData: {table_name} {value1} {value2} {value3}... \n\n"
        "\tCOMMAND: delete_record\n"
        "\t\tData: {table_name} {index} \n\n"
        "\tCOMMAND: list_tables\n"
        "\t\tData: ENTER \n\n"
        "\tCOMMAND: list_records\n"
        "\t\tData: {table_name} {list_all_records_that_contains_this_value}\n\n"
        "\tCOMMAND: sort_table\n"
        "\t\tData: {table_name} {sort_records_by_this_value}\n\n"
        "-----------------------------------MANUAL_END--------------------------------------\n"
    );
}


void start_client(const char *server_ip) {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[256];
    int running = 1;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(1);
    }
#endif

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(client_socket);
        exit(1);
    }

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to server at %s:%d\n", server_ip, PORT);
    printf("Press:\n\t 'help' for manual\n\t 'exit' for exiting program\n");

    Request req = {0};
    Response res = {0};

    while (running) {
        printf("Enter Command: ");
        if (!fgets(req.command, sizeof(req.command), stdin)) {
            perror("Failed to read command");
            break;
        }
        req.command[strcspn(req.command, "\n")] = 0;

        if (strcmp(req.command, "exit") == 0) {
            running = 0;
            printf("Exiting client...\n");
            break;
        }

        if (strcmp(req.command, "help") == 0) {
            print_manual();
            continue;
        }

        printf("Enter Data: ");
        if (!fgets(req.data, sizeof(req.data), stdin)) {
            perror("Failed to read data");
            break;
        }
        req.data[strcspn(req.data, "\n")] = 0;

        if (send_chunked(client_socket, (char*)&req, sizeof(Request)) < 0) {
            perror("Send failed");
            break;
        }

        printf("Sent command: %s\n", req.command);

        int bytes_read = recv_chunked(client_socket, (char*)&res, sizeof(Response));
        if (bytes_read > 0) {
            printf("Server response: %s\n", res.data);
            req.session_id = res.session_id;
        } else if (bytes_read == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            perror("Receive failed");
            break;
        }
    }

#ifdef _WIN32
    closesocket(client_socket);
    WSACleanup();
#else
    close(client_socket);
#endif
}

