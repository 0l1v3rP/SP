#include "../../../headers/server/data/table2userData.h"
#include "../../../headers/server/data/genericData.h"

_Bool table2user_create(Table2User *table2user) {
    return create_generic("table2users.dat", table2user, sizeof(Table2User));
}

Table2User* table2user_get(int id) {
    return (Table2User *)get_by_id_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}

_Bool table2user_delete(int id) {
    return delete_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}
