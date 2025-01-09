#ifndef TABLE2USER_H
#define TABLE2USER_H

#include "./tableModel.h"
#include "./userModel.h"

typedef struct {
    int table2user_id;
    int user_id;
    int table_id;
    Table *table;
    User *user;
} Table2User;

#endif //TABLE2USER_H

