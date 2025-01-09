#include "../models/userModel.h"
#include "../models/sessionModel.h"

#ifndef USERBUSINESS_H
#define USERBUSINESS_H

_Bool register_user(const char *username, const char *password);
_Bool login_user(const char *username, const char *password);
_Bool logout_user(const int user_id);
_Bool delete_user(const int session_id);
User get_logged_in_user(const int session_id);
User get_logged_in_user_id(const int session_id);
Session get_session(const int session_id);

#endif //USERBUSINESS_H



