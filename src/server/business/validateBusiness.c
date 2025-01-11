#include "../../../headers/server/business/validateBusiness.h"

_Bool is_valid_int(const char* value) {
    for (int i = 0; value[i] != '\0'; i++) {
        if (!isdigit(value[i]) && value[i] != '-') {
            return false;
        }
    }
    return true;
}

_Bool is_valid_double(const char* value) {
    _Bool dot_found = false;
    for (int i = 0; value[i] != '\0'; i++) {
        if (value[i] == '.') {
            if (dot_found) return false;
            dot_found = true;
        } else if (!isdigit(value[i]) && value[i] != '-') {
            return false;
        }
    }
    return true;
}

_Bool is_valid_boolean(const char* value) {
    return strcmp(value, "true") == 0 || strcmp(value, "false") == 0;
}

_Bool is_valid_date(const char* value) {
    if (strlen(value) != 10) return false;
    return isdigit(value[0]) && isdigit(value[1]) && isdigit(value[2]) && isdigit(value[3]) &&
           value[4] == '-' &&
           isdigit(value[5]) && isdigit(value[6]) &&
           value[7] == '-' &&
           isdigit(value[8]) && isdigit(value[9]);
}

_Bool validate_record_values(Table* table, const char* record_values) {
    char* values_copy = strdup(record_values);
    char* token = strtok(values_copy, " ");
    int column_index = 0;

    while (token && column_index < table->column_count) {
        Column* column = &table->columns[column_index];

        switch (column->type) {
            case TYPE_STRING:
                break;
            case TYPE_INT:
                if (!is_valid_int(token)) {
                    free(values_copy);
                    return false;
                }
                break;
            case TYPE_DOUBLE:
                if (!is_valid_double(token)) {
                    free(values_copy);
                    return false;
                }
                break;
            case TYPE_BOOLEAN:
                if (!is_valid_boolean(token)) {
                    free(values_copy);
                    return false;
                }
                break;
            case TYPE_DATE:
                if (!is_valid_date(token)) {
                    free(values_copy);
                    return false;
                }
                break;
        }

        token = strtok(NULL, " ");
        column_index++;
    }

    free(values_copy);

    return column_index == table->column_count && token == NULL;
}
