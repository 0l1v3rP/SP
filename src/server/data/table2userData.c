#include "../../../headers/server/data/table2userData.h"
#include "../../../headers/server/data/genericData.h"
#include <stdbool.h>
#include <stdio.h>

_Bool table2user_create(Table2User *table2user) {
    return create_generic("table2users.dat", table2user, sizeof(Table2User));
}

Table2User* table2user_get(int id) {
    return (Table2User *)get_by_id_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}

_Bool table2user_delete(int id) {
    return delete_generic("table2users.dat", id, offsetof(Table2User, table2user_id), sizeof(Table2User));
}

_Bool table2user_delete_by_user_id(int user_id) {
    FILE *file = fopen("table2users.dat", "rb");
    if (!file) return false;

    FILE *temp = fopen("table2users.tmp", "wb");
    if (!temp) {
        fclose(file);
        return false;
    }

    Table2User record;
    _Bool found = false;

    while (fread(&record, sizeof(Table2User), 1, file) == 1) {
        if (record.user_id != user_id) {
            fwrite(&record, sizeof(Table2User), 1, temp);
        } else {
            found = true;
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("table2users.dat");
        rename("table2users.tmp", "table2users.dat");
    } else {
        remove("table2users.tmp");
    }

    return found;
}