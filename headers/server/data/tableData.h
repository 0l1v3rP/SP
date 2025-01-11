#include "../models/tableModel.h"

#ifndef TABLEDATA_H
#define TABLEDATA_H

char* trim(char* str);

_Bool data_table_create(const Table* table, int user_id);
_Bool data_table_delete(const char* table_name, int user_id);
Table * data_table_get(const char* table_name, int user_id);
_Bool data_table_add_record(const char* table_name, int user_id, const char* record_values);
_Bool data_table_delete_record(const char* table_name, int user_id, int index);
_Bool data_table_delete_all_user_tables(int user_id);
_Bool data_table_list(int user_id, char* result, size_t result_size);
_Bool list_all_tables(int user_id, char* result, size_t result_size);
_Bool data_table_list_records(const char* table_name, int user_id, const char* filter, char* result, size_t result_size);
_Bool data_table_sort(const char* table_name, int user_id, const char* sort_column, char* result, size_t result_size);


#endif //TABLEDATA_H