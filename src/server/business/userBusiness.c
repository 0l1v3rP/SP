
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../headers/server/business/userBusiness.h"
#include "../../../headers/server/data/userData.h"
#include "../../../headers/server/data/sessionData.h"
#include "../../../headers/server/data/utilsData.h"


_Bool register_user(const char *username, const char *password) {
    User *existing_user = user_get_by_name(username);
    if (existing_user != NULL) {
        return false;
    }
    User new_user = {0};
    new_user.user_id = generate_unique_id();
    strncpy(new_user.username, username, sizeof(new_user.username) - 1);
    strncpy(new_user.password, password, sizeof(new_user.password) - 1);

    return user_create(&new_user);
}

_Bool login_user(const char *username, const char *password, int *session_id) {
    User *user = user_get_by_name(username);
    if(user == NULL || strcmp(user->password, password) == 1) {
        free(user);
        return false;
    }
    Session new_session = {0};
    int session_id_generated = generate_unique_id();
    new_session.session_id = session_id_generated;
    session_id = &session_id_generated;
    new_session.user_id = user->user_id;
    new_session.expiration_time = time(NULL) + SESSION_EXPIRATION;
    session_create(&new_session);
    free(user);
    return true;
}

_Bool logout_user(const int session_id) {
    return session_delete(session_id);
}
_Bool delete_user(const int session_id) {
    Session *session = session_get(session_id);
    if (!session) return false;

    int user_id = session->user_id;
    free(session);
    //TODO: delete associated table2user
    if (!session_delete(session_id)) return false;

    return user_delete(user_id);
}
User * get_logged_in_user(const int session_id) {
    Session *session = session_get(session_id);
    if (!session) return NULL;
    User *user = user_get(session->user_id);
    free(session);
    return  !user ? NULL : user;
}

Session * get_session(const int session_id) {
    return session_get(session_id);
}

