#include "../../../headers/server/handlers/userHandler.h"
#include "../../../headers/server/models/sessionModel.h"
#include "../../../headers/server/business/userBusiness.h"

void handle_register(Request* req, Response* res, Session* session) {
    char username[MAX_USERNAME] = {0};
    char password[MAX_PASSWORD] = {0};
    sscanf(req->data, "%s %s", username, password);
    if(req->session_id != 0 ) {
        snprintf(res->data, MAX_CHUNK_SIZE, "%s", "REGISTRATION FAILED");
    }
    else if(register_user(username, password)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "%s","REGISTRATION SUCCESS");
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "%s", "REGISTRATION FAILED");
    }

    res->session_id = req->session_id;
}

void handle_login(Request* req, Response* res, Session* session) {
    char username[MAX_USERNAME] = {0};
    char password[MAX_PASSWORD] = {0};
    sscanf(req->data, "%s %s", username, password);
    int session_id;

    if (req->session_id == 0 && login_user(username, password, &session_id)) {
        Session* usersession = get_session(session_id);
        if (usersession == NULL) {
            snprintf(res->data, MAX_CHUNK_SIZE, "%s", "LOGIN FAILED");
            res->session_id = 0;
            return;
        }
        snprintf(res->data, MAX_CHUNK_SIZE, "%s", "LOGIN SUCCESS");
        res->session_id = session_id;
        free(usersession); // Správne uvoľnenie pamäte
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "LOGIN FAILED");
        res->session_id = req->session_id;
    }
}


void handle_logout(Request* req, Response* res, Session* session) {
    if (req->session_id == 0) {
        snprintf(res->data, MAX_CHUNK_SIZE, "LOGOUT FAILED");
    } else if (logout_user(req->session_id)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "LOGOUT SUCCESS");
        req->session_id = 0;
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "LOGOUT FAILED");
    }

    res->session_id = req->session_id;
}

void handle_delete_user(Request* req, Response* res, Session* session) {
    if (req->session_id == 0) {
        snprintf(res->data, MAX_CHUNK_SIZE, "USER DELETION FAILED");
    } else if (delete_user(session)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "USER DELETION SUCCESS");
        req->session_id = 0;
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "USER DELETION FAILED");
    }
    res->session_id = req->session_id;
}