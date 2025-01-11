#include "../../../headers/server/handlers/commonHandler.h"
#include "../../../headers/server/models/sessionModel.h"
#include "../../../headers/server/business/userBusiness.h"

static const CommandMapping COMMANDS[] = {
    {"register", handle_register},
    {"login", handle_login},
    {"logout", handle_logout},
    {"delete_user", handle_delete_user},
    {"create_table", handle_create_table},
    {"delete_table", handle_delete_table},
    {"add_record", handle_add_record},
    {"delete_record", handle_delete_record},
    {"list_tables", handle_list_tables},
    {"list_records", handle_list_records},
    {"sort_table", handle_sort_table},

    {NULL, NULL}
};

_Bool hasValidSession(Request* req, Response* res, Session** session) {
    if (req->session_id != 0) {
        *session = get_session(req->session_id);
        if (*session != NULL) {
            if ((*session)->expiration_time < time(NULL)) {
                res->session_id = 0;
                logout_user(req->session_id);
                strncpy(res->data, "SESSION EXPIRED", MAX_CHUNK_SIZE);
                free(*session);
                *session = NULL; // Clear the session pointer to avoid dangling pointers
                return false;
            }
        } else {
            res->session_id = 0;
            strncpy(res->data, "SESSION EXPIRED", MAX_CHUNK_SIZE);
            return false;
        }
    }
    return true;
}



void handle_request(Request* req, Response* res) {
    Session* session = NULL;
    if(!hasValidSession(req, res, &session)) return;

    for (const CommandMapping* cmd = COMMANDS; cmd->command != NULL; cmd++) {
        if (strcmp(req->command, cmd->command) == 0) {
            cmd->handler(req, res, session);
            return;
        }
    }
    snprintf(res->data, MAX_CHUNK_SIZE, "UNKNOWN COMMAND");
    res->session_id = req->session_id;
}

_Bool signed_in(Request* req, Response* res) {
    res->session_id = req->session_id;
    if(req->session_id != 0) return true;
    strncpy(res->data, "NOT LOGGED IN", MAX_CHUNK_SIZE);
    return false;
}