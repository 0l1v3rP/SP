#include "../models/userModel.h"

#ifndef USERDATA_H
#define USERDATA_H

_Bool user_create(User *user);
_Bool user_delete(int id);
User* user_get(int id);
User* user_get_by_name(const char *username);
#endif //USERDATA_H
