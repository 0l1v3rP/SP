#include "../models/tableModel.h"

#ifndef TABLEDATA_H
#define TABLEDATA_H

_Bool table_create(Table *table);
_Bool table_edit(Table *table);
_Bool table_delete(int id);
Table * table_get(int id);

#endif //TABLEDATA_H
