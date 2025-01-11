#include "../models/table2userModel.h"

#ifndef TABLE2USERDATA_H
#define TABLE2USERDATA_H

_Bool table2user_create(Table2User *table2user);
_Bool table2user_delete(int id);
Table2User* table2user_get(int id);
_Bool table2user_delete_by_user_id(int user_id);
#endif //TABLE2USERDATA_H
