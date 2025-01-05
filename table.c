//
// Created by dablkros on 5. 1. 2025.
//

#include "table.h"

Table *table_create(const char *name) {
    Table *table = (Table *)malloc(sizeof(Table));
    if (!table) {
        perror("Failed to allocate memory for table");
        return NULL;
    }
    table->name = strdup(name);
    table->columns = NULL;
    table->column_count = 0;
    return table;
}

void table_add_column(Table *table, const char *column_name, ColumnType type) {
    table->columns = realloc(table->columns, sizeof(Column) * (table->column_count + 1));
    if (!table->columns) {
        perror("Failed to allocate memory for columns");
        return;
    }
    table->columns[table->column_count].name = strdup(column_name);
    table->columns[table->column_count].type = type;
    table->column_count++;
}

void table_delete(Table *table) {
    if (!table) return;
    free(table->name);
    for (int i = 0; i < table->column_count; i++) {
        free(table->columns[i].name);
    }
    free(table->columns);
    free(table);
}

void table_print(const Table *table) {
    if (!table) return;
    printf("Table: %s\n", table->name);
    printf("Columns:\n");
    for (int i = 0; i < table->column_count; i++) {
        printf(" - %s (Type: %d)\n", table->columns[i].name, table->columns[i].type);
    }
}
