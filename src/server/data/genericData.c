#include "../../../headers/server/data/genericData.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

_Bool delete_generic(const char *filename, int id, size_t offset, size_t struct_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) return false;

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        fclose(file);
        return false;
    }

    void *buffer = malloc(struct_size);
    if (!buffer) {
        fclose(file);
        fclose(temp);
        return false;
    }

    _Bool success = false;
    while (fread(buffer, struct_size, 1, file)) {
        // Assume ID is at the offset
        int *current_id = (int *)((char *)buffer + offset);
        if (*current_id != id) {
            fwrite(buffer, struct_size, 1, temp);
        } else {
            success = true;
        }
    }

    free(buffer);
    fclose(file);
    fclose(temp);

    remove(filename);
    rename("temp.dat", filename);

    return success;
}

_Bool create_generic(const char *filename, void *data, size_t struct_size) {
    FILE *file = fopen(filename, "ab");
    if (!file) return false;

    if (fwrite(data, struct_size, 1, file) != 1) {
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

void* get_by_condition_generic(const char *filename,
                               size_t struct_size,
                               _Bool (*condition)(void*, void*),
                               void *context) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    void *buffer = malloc(struct_size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    while (fread(buffer, struct_size, 1, file)) {
        if (condition(buffer, context)) {
            fclose(file);
            return buffer; // Found a matching record
        }
    }

    free(buffer);
    fclose(file);
    return NULL; // No match found
}

_Bool match_id(void *record, void *context) {
    int *record_id = (int *)((char *)record + *(size_t *)context);
    int *target_id = (int *)((char *)context + sizeof(size_t));
    return *record_id == *target_id;
}

void* get_by_id_generic(const char *filename, int id, size_t id_offset, size_t struct_size) {
    size_t context[2] = { id_offset, id };
    return get_by_condition_generic(filename, struct_size, match_id, context);
}