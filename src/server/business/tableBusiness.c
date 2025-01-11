#include "../../../headers/server/business/tableBusiness.h"
#include "../../../headers/server/business/validateBusiness.h"
#include "../../../headers/server/data/tableData.h"
#include "../../../headers/server/data/table2userData.h"
#include "../../../headers/server/models/table2userModel.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../../headers/server/data/utilsData.h"

_Bool create_table(const Table* table, int user_id) {
    Table* existingTable = data_table_get(table->name, user_id);
    if (existingTable != NULL || !data_table_create(table, user_id)) {
        return false;
    }

    Table2User table2user;
    table2user.table2user_id = generate_unique_id();
    table2user.user_id = user_id;
    table2user.table_name = table->name;
    return table2user_create(&table2user);
}

_Bool delete_table(const char* table_name, int user_id) {
    return data_table_delete(table_name, user_id) && table2user_delete_by_user_id(user_id);
}

_Bool add_record_table(const char* table_name, int user_id, const char* record_values) {
    Table* existingTable = data_table_get(table_name, user_id);
    return existingTable != NULL && validate_record_values(existingTable ,record_values) && data_table_add_record(table_name, user_id, record_values);
}

_Bool delete_record_table(const char* table_name, int user_id, int record_id) {
    Table* existingTable = data_table_get(table_name, user_id);
    return existingTable != NULL && data_table_delete_record(table_name, user_id, record_id);
}

ColumnType convert_type(const char* type) {
    if (strcmp(type, "string") == 0) return TYPE_STRING;
    if (strcmp(type, "int") == 0) return TYPE_INT;
    if (strcmp(type, "double") == 0) return TYPE_DOUBLE;
    if (strcmp(type, "boolean") == 0) return TYPE_BOOLEAN;
    if (strcmp(type, "date") == 0) return TYPE_DATE;
    return -1;
}
