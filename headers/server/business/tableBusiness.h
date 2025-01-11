#ifndef TABLEBUSINESS_H
#define TABLEBUSINESS_H
#include "../models/tableModel.h"
_Bool create_table(const Table* table, int user_id);
_Bool delete_table(const char* table_name, int user_id);
_Bool add_record_table(const char* table_name, int user_id, const char* record_values);
_Bool delete_record_table(const char* table_name, int user_id, int record_id);
ColumnType convert_type(const char* type);
#endif //TABLEBUSINESS_H
