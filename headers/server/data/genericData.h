//
// Created by Acer on 09/01/2025.
//

#ifndef GENERIC_DATA_H
#define GENERIC_DATA_H
#include <stddef.h>

_Bool delete_generic(const char *filename, int id, size_t offset, size_t struct_size);
_Bool create_generic(const char *filename, void *data, size_t struct_size) ;
void* get_by_id_generic(const char *filename, int id, size_t id_offset, size_t struct_size);
void* get_by_condition_generic(const char *filename,
                               size_t struct_size,
                               _Bool (*condition)(void*, void*),
                               void *context);
#endif //GENERIC_DATA_H
