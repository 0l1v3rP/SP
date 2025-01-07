//
// Created by dablkros on 5. 1. 2025.
//

#ifndef TABLE_H_H
#define TABLE_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_STRING,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOLEAN,
    TYPE_DATE
} ColumnType;

typedef struct {
    char *name;
    ColumnType type;
} Column;

typedef struct {
    char *name;
    Column *columns;
    int column_count;
} Table;

// Funkcie na správu tabuľky
Table *table_create(const char *name);
void table_add_column(Table *table, const char *column_name, ColumnType type);
void table_delete(Table *table);
void table_print(const Table *table);


#endif //TABLE_H_H
