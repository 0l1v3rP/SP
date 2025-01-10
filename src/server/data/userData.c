#include "../../../headers/server/data/userData.h"
#include "../../../headers/server/data/genericData.h"
#include <string.h>

_Bool user_create(User *user) {
    return create_generic("users.dat", user, sizeof(User));
}

User* user_get(int id) {
    return (User *)get_by_id_generic("users.dat", id, offsetof(User, user_id), sizeof(User));
}

_Bool user_delete(int id) {
    return delete_generic("users.dat", id, offsetof(User, user_id), sizeof(User));
}
_Bool match_username(void *record, void *context) {
    char *record_username = (char *)((char *)record + offsetof(User, username));
    char *target_username = (char *)context;
    return strcmp(record_username, target_username) == 0;
}

User* user_get_by_name(const char *username) {
    return (User *)get_by_condition_generic("users.dat", sizeof(User), match_username, (void *)username);
}