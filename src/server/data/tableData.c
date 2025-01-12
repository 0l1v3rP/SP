#include <stdbool.h>
#include <stdio.h>
#include "../../../headers/server/data/tableData.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    char *line;
} Record;

typedef struct {
    int sort_index;
    int column_type;
} SortContext;

static SortContext sort_context;

_Bool data_table_create(const Table *table, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table->name);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    for (int i = 0; i < table->column_count; i++) {
        fprintf(file, "[%s %d]", table->columns[i].name, table->columns[i].type); // Zápis správneho typu
        if (i < table->column_count - 1) {
            fprintf(file, " ");
        }
    }
    fprintf(file, "\n");

    fclose(file);
    return true;
}

_Bool data_table_delete(const char *table_name, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);
    return remove(filename) == 0;
}

Table *data_table_get(const char *table_name, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        fclose(file);
        return NULL;
    }

    table->column_count = 0;
    char line[1024];
    if (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, "[] ");
        while (token != NULL && strcmp(token, "\n") != 0) {
            strncpy(table->columns[table->column_count].name, token,
                    sizeof(table->columns[table->column_count].name) - 1);
            table->columns[table->column_count].name[sizeof(table->columns[table->column_count].name) - 1] = '\0';
            token = strtok(NULL, "[] ");
            if (token == NULL) break;  // Prevent null pointer dereference
            table->columns[table->column_count].type = atoi(token);
            table->column_count++;
            token = strtok(NULL, "[] ");
        }
    }

    fclose(file);
    return table;
}

_Bool data_table_add_record(const char *table_name, int user_id, const char *record_values) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%s\n", record_values);
    fclose(file);
    return true;
}

_Bool data_table_delete_record(const char *table_name, int user_id, int index) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "table_%d_%s_temp.dat", user_id, table_name);

    FILE *temp_file = fopen(temp_filename, "w");
    if (temp_file == NULL) {
        fclose(file);
        return false;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int current_index = 0;
    _Bool deleted = false;

    while ((read = getline(&line, &len, file)) != -1) {
        if (current_index != index) {
            fwrite(line, 1, read, temp_file);
        } else {
            deleted = true;
        }
        current_index++;
        free(line);
        line = NULL;
    }

    // Uvoľni pamäť, ak cyklus skončil predčasne
    if (line != NULL) {
        free(line);
    }

    fclose(file);
    fclose(temp_file);


    // Ak rename zlyhá, odstráň dočasný súbor
    if (deleted) {
        if (remove(filename) != 0 || rename(temp_filename, filename) != 0) {
            remove(temp_filename);
            return false;
        }
    } else {
        remove(temp_filename);
    }

    return deleted;
}

_Bool data_table_delete_all_user_tables(int user_id) {
    char command[512];
    _Bool success = true;

#ifdef _WIN32
    // Windows version
    snprintf(command, sizeof(command), "del table_%d_*.dat 2>nul", user_id);
#else
    // Unix/Linux version
    snprintf(command, sizeof(command), "rm -f table_%d_*.dat 2>/dev/null", user_id);
#endif

    int result = system(command);

    // Check if command executed successfully
    if (result != 0) {
        // Alternative manual approach if system command fails
        DIR *dir;
        struct dirent *entry;
        char filename[256];
        char pattern[32];

        snprintf(pattern, sizeof(pattern), "table_%d_", user_id);
        dir = opendir(".");

        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (strncmp(entry->d_name, pattern, strlen(pattern)) == 0 &&
                    strstr(entry->d_name, ".dat") != NULL) {
                    snprintf(filename, sizeof(filename), "%s", entry->d_name);
                    if (remove(filename) != 0) {
                        success = false;
                    }
                }
            }
            closedir(dir);
        } else {
            success = false;
        }
    }

    return success;
}

_Bool data_table_list(int user_id, char *result, size_t result_size) {
    char pattern[64];
    snprintf(pattern, sizeof(pattern), "table_%d_", user_id);

    DIR *dir = opendir(".");
    if (dir == NULL) {
        snprintf(result, result_size, "ERROR: Failed to open directory.");
        return false;
    }

    struct dirent *entry;
    size_t used_size = 0;
    result[0] = '\0';

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, pattern, strlen(pattern)) == 0 &&
            strstr(entry->d_name, ".dat") != NULL) {
            char *table_name = entry->d_name + strlen(pattern);
            char *dot = strstr(table_name, ".dat");
            if (dot != NULL) *dot = '\0';

            size_t name_length = strlen(table_name);
            if (used_size + name_length + 2 >= result_size) {
                closedir(dir);
                return false;
            }

            strcat(result, table_name);
            strcat(result, ", ");
            used_size += name_length + 2;
        }
    }
    closedir(dir);

    if (used_size > 2) {
        result[used_size - 2] = '\0';
    }

    return true;
}

_Bool
data_table_list_records(const char *table_name, int user_id, const char *filter, char *result, size_t result_size) {
    if (table_name == NULL || result == NULL || result_size == 0) {
        return false; // Neplatné vstupy
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        snprintf(result, result_size, "ERROR: Table '%s' does not exist.", table_name);
        return false;
    }

    char *line = NULL;
    size_t len = 0;

    // Preskočte hlavičku (prvý riadok obsahuje definíciu stĺpcov)
    if (getline(&line, &len, file) == -1) {
        snprintf(result, result_size, "ERROR: Table '%s' is empty or corrupt.", table_name);
        fclose(file);
        free(line);
        return false;
    }

    size_t used_size = 0;
    result[0] = '\0';

    // Čítajte záznamy riadok po riadku
    while (getline(&line, &len, file) != -1) {
        if (filter == NULL || strstr(line, filter) != NULL) {
            size_t line_length = strlen(line);
            if (used_size + line_length + 1 >= result_size) {
                snprintf(result, result_size, "ERROR: Result buffer too small.");
                free(line);
                fclose(file);
                return false;
            }
            strcat(result, line);
            used_size += line_length;
        }
    }

    fclose(file);
    free(line);

    if (used_size == 0) {
        snprintf(result, result_size, "No records found%s.", filter ? " matching the filter" : "");
        return true;
    }

    return true;
}


char *trim(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char) *str)) str++;

    if (*str == 0) return str; // Ak je prázdny string, vráťte ho

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    // Null-terminate
    *(end + 1) = '\0';

    return str;
}

// Funkcia na získanie hodnoty stĺpca podľa indexu
char *get_column_value(char *line, int column_index) {
    char *token = strtok(line, " ");
    for (int i = 0; i < column_index && token != NULL; i++) {
        token = strtok(NULL, " ");
    }
    return token;
}

//porovnávacia funkcia
int compare_records_context(const void *a, const void *b) {
    const Record *recA = (const Record *) a;
    const Record *recB = (const Record *) b;

    char *valA = strdup(recA->line);
    char *valB = strdup(recB->line);

    if (valA == NULL || valB == NULL) {
        printf("ERROR: Memory allocation failed.\n");
        free(valA);
        free(valB);
        return 0;
    }

    char *tokenA = get_column_value(valA, sort_context.sort_index);
    char *tokenB = get_column_value(valB, sort_context.sort_index);

    int cmp = 0;

    if (tokenA == NULL || tokenB == NULL) {
        printf("ERROR: Invalid column index in record comparison.\n");
        free(valA);
        free(valB);
        return 0;
    }

    switch (sort_context.column_type) {
        case TYPE_STRING: {
            char *trimmedA = trim(tokenA);
            char *trimmedB = trim(tokenB);
            cmp = strcasecmp(trimmedA, trimmedB);
            break;
        }

        case TYPE_INT: {
            int intA = atoi(tokenA);
            int intB = atoi(tokenB);
            cmp = (intA > intB) - (intA < intB);
            break;
        }

        case TYPE_DOUBLE: {
            double doubleA = atof(tokenA);
            double doubleB = atof(tokenB);
            cmp = (doubleA > doubleB) - (doubleA < doubleB);
            break;
        }

        case TYPE_BOOLEAN: {
            int boolA = (strcasecmp(tokenA, "true") == 0 || strcmp(tokenA, "1") == 0) ? 1 : 0;
            int boolB = (strcasecmp(tokenB, "true") == 0 || strcmp(tokenB, "1") == 0) ? 1 : 0;
            cmp = boolA - boolB;
            break;
        }

        case TYPE_DATE: {
            cmp = strcmp(tokenA, tokenB);
            break;
        }

        default:
            printf("ERROR: Unsupported column type: %d\n", sort_context.column_type);
            cmp = 0;
            break;
    }

    free(valA);
    free(valB);
    return cmp;
}

_Bool data_table_sort(const char *table_name, int user_id, const char *sort_column, char *result, size_t result_size) {
    if (table_name == NULL || sort_column == NULL || result == NULL || result_size == 0) {
        snprintf(result, result_size, "ERROR: Invalid input parameters.");
        return false;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        snprintf(result, result_size, "ERROR: Table '%s' does not exist.", table_name);
        return false;
    }

    char *line = NULL;
    size_t len = 0;

    // Načítajte hlavičku
    if (getline(&line, &len, file) == -1) {
        snprintf(result, result_size, "ERROR: Table '%s' is empty or corrupt.", table_name);
        fclose(file);
        free(line);
        return false;
    }

    // Spracovanie hlavičky
    char columns[10][256];
    int column_types[10];
    int column_count = 0;
    char *token = strtok(line, "[] ");
    while (token != NULL) {
        strncpy(columns[column_count], token, sizeof(columns[column_count]) - 1);
        token = strtok(NULL, "[] ");
        if (token == NULL) break;
        column_types[column_count] = atoi(token);
        column_count++;
        token = strtok(NULL, "[] ");
    }

    // Určenie indexu stĺpca na triedenie
    int sort_index = -1;
    for (int i = 0; i < column_count; i++) {
        if (strcmp(columns[i], sort_column) == 0) {
            sort_index = i;
            break;
        }
    }

    if (sort_index == -1) {
        snprintf(result, result_size, "ERROR: Column '%s' not found in table '%s'.", sort_column, table_name);
        fclose(file);
        free(line);
        return false;
    }

    sort_context.sort_index = sort_index;
    sort_context.column_type = column_types[sort_index];

    // Načítajte záznamy
    Record *records = NULL;
    size_t record_count = 0;
    while (getline(&line, &len, file) != -1) {
        records = realloc(records, sizeof(Record) * (record_count + 1));
        if (records == NULL) {
            snprintf(result, result_size, "ERROR: Memory allocation failed.");
            fclose(file);
            free(line);
            return false;
        }
        records[record_count].line = strdup(line);
        if (records[record_count].line == NULL) {
            snprintf(result, result_size, "ERROR: Memory allocation failed.");
            fclose(file);
            free(line);
            return false;
        }
        records[record_count].line[strcspn(records[record_count].line, "\n")] = '\0';
        record_count++;
    }
    fclose(file);
    free(line);

    // Zotriedenie záznamov
    qsort(records, record_count, sizeof(Record), compare_records_context);

    // Zápis záznamov späť do súboru
    FILE *sorted_file = fopen(filename, "w");
    if (sorted_file == NULL) {
        snprintf(result, result_size, "ERROR: Unable to open table '%s' for writing.", table_name);
        for (size_t i = 0; i < record_count; i++) {
            free(records[i].line);
        }
        free(records);
        return false;
    }

    // Zápis hlavičky
    for (int i = 0; i < column_count; i++) {
        fprintf(sorted_file, "[%s %d]", columns[i], column_types[i]);
        if (i < column_count - 1) {
            fprintf(sorted_file, " ");
        }
    }
    fprintf(sorted_file, "\n");

    // Zápis zotriedených záznamov
    for (size_t i = 0; i < record_count; i++) {
        fprintf(sorted_file, "%s\n", records[i].line);
        free(records[i].line);
    }
    free(records);
    fclose(sorted_file);

    snprintf(result, result_size, "Table '%s' successfully sorted by column '%s'.", table_name, sort_column);
    return true;
}
