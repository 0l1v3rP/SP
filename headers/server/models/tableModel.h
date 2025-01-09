#ifndef TABLEMODEL_H
#define TABLEMODEL_H

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

#endif //TABLEMODEL_H
