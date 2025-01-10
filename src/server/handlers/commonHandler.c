#include "../../../headers/server/handlers/commonHandler.h"
#include "../../../headers/server/models/sessionModel.h"
#include "../../../headers/server/business/userBusiness.h"

static const CommandMapping COMMANDS[] = {
    {"register", handle_register},
    {"login", handle_login},
    {"logout", handle_logout},
    {"delete_user", handle_delete_user},
    {NULL, NULL}
};

_Bool hasValidSession(Request* req, Response* res) {
    if (req->session_id != 0) {
        Session* session = get_session(req->session_id);
        if (session != NULL) {
            if (session->expiration_time < time(NULL)) {
                res->session_id = 0;
                logout_user(req->session_id);
                strncpy(res->data, "SESSION EXPIRED", MAX_CHUNK_SIZE);
                free(session);
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
    if(!hasValidSession(req, res)) return;

    for (const CommandMapping* cmd = COMMANDS; cmd->command != NULL; cmd++) {
        if (strcmp(req->command, cmd->command) == 0) {
            cmd->handler(req, res);
            return;
        }
    }
    snprintf(res->data, MAX_CHUNK_SIZE, "UNKNOWN COMMAND");
    res->session_id = req->session_id;
}

