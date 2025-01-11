#include "../models/tableModel.h"

#ifndef TABLEDATA_H
#define TABLEDATA_H

_Bool data_table_create(const Table* table, int user_id);
_Bool data_table_delete(const char* table_name, int user_id);
Table * data_table_get(const char* table_name, int user_id);
_Bool data_table_add_record(const char* table_name, int user_id, const char* record_values);
_Bool data_table_delete_record(const char* table_name, int user_id, int index);
_Bool data_table_delete_all_user_tables(int user_id);
#endif //TABLEDATA_H