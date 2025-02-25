
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../headers/server/business/userBusiness.h"
#include "../../../headers/server/data/userData.h"
#include "../../../headers/server/data/sessionData.h"
#include "../../../headers/server/data/tableData.h"
#include "../../../headers/server/data/table2userData.h"
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
    if(user == NULL || strcmp(user->password, password) != 0) {
        free(user);
        return false;
    }

    Session *existing_session = session_get_by_user_id(user->user_id);
    if (existing_session) {
        session_delete(existing_session->session_id);
        free(existing_session);
    }

    Session new_session = {0};
    int session_id_generated = generate_unique_id();
    new_session.session_id = session_id_generated;
    *session_id = session_id_generated; ;
    new_session.user_id = user->user_id;
    new_session.expiration_time = time(NULL) + SESSION_EXPIRATION;
    session_create(&new_session);
    free(user);
    return true;
}

_Bool logout_user(int session_id) {
    return session_delete(session_id);
}
_Bool delete_user(const Session *session) {
    if (!session) return false;

    int user_id = session->user_id;
    if (!session_delete(session->session_id)) return false;
    return user_delete(user_id) && data_table_delete_all_user_tables(user_id) && table2user_delete_by_user_id(user_id);
}
User * get_logged_in_user(const Session *session) {
    if (!session) return NULL;
    User *user = user_get(session->user_id);
    return  !user ? NULL : user;
}

Session * get_session(int session_id) {
    return session_get(session_id);
}

