#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../../headers/server/comHandler.h"

#include <stdlib.h>

#include "../../headers/server/business/table2userBusiness.h"
#include "../../headers/server/business/userBusiness.h"
#include "../../headers/server/business/tableBusiness.h"

// Command handlers
void handle_register(Request req, char* response) {
    char username[MAX_USERNAME] = {0};
    char password[MAX_PASSWORD] = {0};
    sscanf(req.data, "%s %s", username, password);
    bool success = register_user(username, password);
    snprintf(response, MAX_CHUNK_SIZE, "%d", success);
}

void handle_login(Request req, char* response) {
    char username[MAX_USERNAME] = {0};
    char password[MAX_PASSWORD] = {0};
    sscanf(req.data, "%s %s", username, password);

    if (login_user(username, password)) {
        Session * session = get_session(req.session_id);
        snprintf(response, MAX_CHUNK_SIZE, "%d", session->session_id);
        free(session);
    } else {
        snprintf(response, MAX_CHUNK_SIZE, "0");
    }
}

void handle_logout(Request req, char* response) {
    bool success = logout_user(req.session_id);
    snprintf(response, MAX_CHUNK_SIZE, "%d", success);
}

void handle_session_check(Request req, char* response) {
    Session * session = get_session(req.session_id);
    snprintf(response, MAX_CHUNK_SIZE, "%d", session != NULL && session->session_id ? session->session_id : 0);
    free(session);
}

void handle_delete_user(Request req, char* response) {
    bool success = delete_user(req.session_id);
    snprintf(response, MAX_CHUNK_SIZE, "%d", success);
}

void handle_request(Request req, char response[MAX_CHUNK_SIZE]) {
    for (const CommandMapping* cmd = COMMANDS; cmd->command != NULL; cmd++) {
        if (strcmp(req.command, cmd->command) == 0) {
            cmd->handler(req, response);
            return;
        }
    }
    snprintf(response, MAX_CHUNK_SIZE, "Unknown command");
}
