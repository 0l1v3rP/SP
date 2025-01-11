#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#define MAX_COLUMN_NAME_LENGTH 50
#define MAX_TYPE_NAME_LENGTH 10
#define MAX_COLUMNS 10
#define  MAX_TABLE_NAME_LENGTH 15
#define MAX_RECORD_LENGTH 100
typedef enum {
    TYPE_STRING,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOLEAN,
    TYPE_DATE
} ColumnType;

typedef struct {
    char name[MAX_COLUMN_NAME_LENGTH];
    ColumnType type;
} Column;

typedef struct {
    char name[MAX_TABLE_NAME_LENGTH];
    Column columns[MAX_COLUMNS];
    int column_count;
} Table;

#endif //TABLEMODEL_H
