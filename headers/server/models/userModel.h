#include "./table2UserModel.h"

#ifndef USERMODEL_H
#define USERMODEL_H

typedef struct {
    int user_id;
    char username;
    char password;
    Table2User *tables;
} User;

#endif //USERMODEL_H