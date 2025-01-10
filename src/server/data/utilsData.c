#include "../../../headers/server/data/utilsData.h"
#include <stdio.h>

int generate_unique_id() {
    const char *counter_file = "id_counter.dat";
    FILE *file = fopen(counter_file, "r+");
    if (!file) {
        file = fopen(counter_file, "w");
        if (!file) return -1; // Failed to create the counter file
        int initial_id = 1;
        fprintf(file, "%d", initial_id);
        fclose(file);
        return initial_id;
    }

    int current_id = 0;
    fscanf(file, "%d", &current_id);

    rewind(file);
    fprintf(file, "%d", current_id + 1);
    fclose(file);

    return current_id + 1;
}