#ifndef VALIDATEBUSINESS_H
#define VALIDATEBUSINESS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../models/tableModel.h"

_Bool is_valid_int(const char* value);
_Bool is_valid_double(const char* value);
_Bool is_valid_boolean(const char* value);
_Bool is_valid_date(const char* value);
_Bool validate_record_values(Table* table, const char* record_values);
#endif //VALIDATEBUSINESS_H
