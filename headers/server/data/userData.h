#include "../models/userModel.h"

#ifndef USERDATA_H
#define USERDATA_H

_Bool user_create(User *user);
_Bool user_edit(User *user);
_Bool user_delete(int id);
User* user_get(int id);

#endif //USERDATA_H
