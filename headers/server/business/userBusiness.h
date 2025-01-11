#include "../models/userModel.h"
#include "../models/sessionModel.h"

#ifndef USERBUSINESS_H
#define USERBUSINESS_H
#define SESSION_EXPIRATION 3600;
_Bool register_user(const char *username, const char *password);
_Bool login_user(const char *username, const char *password, int *session_id);
_Bool logout_user(int session_id);
_Bool delete_user(const Session *session);
User * get_logged_in_user(const Session *session);
Session * get_session(int session_id);

#endif //USERBUSINESS_H



