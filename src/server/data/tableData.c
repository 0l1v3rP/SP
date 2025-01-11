#include <stdbool.h>
#include <stdio.h>
#include "../../../headers/server/data/tableData.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <windows.h>
#endif

_Bool data_table_create(const Table* table, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table->name);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    for (int i = 0; i < table->column_count; i++) {
        fprintf(file, "[%s %d]", table->columns[i].name, table->columns[i].type);
        if (i < table->column_count - 1) {
            fprintf(file, " ");
        }
    }
    fprintf(file, "\n");

    fclose(file);
    return true;
}

_Bool data_table_delete(const char* table_name, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);
    return remove(filename) == 0;
}

Table* data_table_get(const char* table_name, int user_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Table* table = malloc(sizeof(Table));
    if (table == NULL) {
        fclose(file);
        return NULL;
    }

    table->column_count = 0;
    char line[1024];
    if (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, "[] ");
        while (token != NULL && strcmp(token, "\n") != 0) {

            strncpy(table->columns[table->column_count].name, token, sizeof(table->columns[table->column_count].name) - 1);
            table->columns[table->column_count].name[sizeof(table->columns[table->column_count].name) - 1] = '\0';
            token = strtok(NULL, "[] ");
            table->columns[table->column_count].type = atoi(token);
            table->column_count++;
            token = strtok(NULL, "[] ");
        }
    }

    fclose(file);
    return table;
}

_Bool data_table_add_record(const char* table_name, int user_id, const char* record_values) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%s\n", record_values);
    fclose(file);
    return true;
}

_Bool data_table_delete_record(const char* table_name, int user_id, int index) {
    char filename[256];
    snprintf(filename, sizeof(filename), "table_%d_%s.dat", user_id, table_name);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "table_%d_%s_temp.dat", user_id, table_name);

    FILE* temp_file = fopen(temp_filename, "w");
    if (temp_file == NULL) {
        fclose(file);
        return false;
    }

    char* line = NULL;
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
    }

    free(line);
    fclose(file);
    fclose(temp_file);

    if (deleted) {
        remove(filename);
        rename(temp_filename, filename);
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
        DIR* dir;
        struct dirent* entry;
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
