#ifndef TABLEBUSINESS_H
#define TABLEBUSINESS_H

#include <stddef.h>
#include "../models/tableModel.h"
_Bool create_table(const Table* table, int user_id);
_Bool delete_table(const char* table_name, int user_id);
_Bool add_record_table(const char* table_name, int user_id, const char* record_values);
_Bool delete_record_table(const char* table_name, int user_id, int record_id);
_Bool list_all_tables(int user_id, char* result, size_t result_size);
ColumnType convert_type(const char* type);
_Bool list_records(const char* table_name, int user_id, const char* filter, char* result, size_t result_size);
_Bool sort_table(const char* table_name, int user_id, const char* sort_column, char* result, size_t result_size);



#endif //TABLEBUSINESS_H
