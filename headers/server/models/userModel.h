#ifndef USERMODEL_H
#define USERMODEL_H

#define MAX_USERNAME 32
#define MAX_PASSWORD 64

typedef struct {
    int user_id;
    char username[MAX_USERNAME]; // Fixed to hold a string
    char password[MAX_PASSWORD]; // Fixed to hold a string
} User;
#endif //USERMODEL_H
