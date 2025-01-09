#include "../../../headers/server/data/table2userData.h"
#include "../../../headers/server/data/genericData.h"

_Bool table2user_create(Table2User *table2user) {
    // Call the generic create function
    return create_generic("table2users.dat", table2user, sizeof(Table2User));
}

Table2User* table2user_get(int id) {
    // Call the generic get_by_id function
    return (Table2User *)get_by_id_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}

_Bool table2user_delete(int id) {
    // Call the generic delete function
    return delete_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}
