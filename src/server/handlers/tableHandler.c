#include "../../../headers/server/handlers/tableHandler.h"
#include "../../../headers/server/business/tableBusiness.h"

void handle_create_table(Request* req, Response* res, Session* session) {
    if(!signed_in(req, res)) return;

    char* input = req->data;
    if (strlen(input) == 0) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: No data provided for create_table");
        return;
    }

    Table table = {0};

    char* token = strtok(input, " ");
    if (!token) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: No table name provided");
        return;
    }

    if (strlen(token) >= MAX_TABLE_NAME_LENGTH) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Table name too long");
        return;
    }
    strncpy(table.name, token, MAX_TABLE_NAME_LENGTH - 1);
    table.name[MAX_TABLE_NAME_LENGTH - 1] = '\0';

    while ((token = strtok(NULL, "[]")) != NULL) {
        if (strspn(token, " \t\n") == strlen(token)) {
            continue;
        }

        char column_name[256];  // Temporary buffer
        char column_type[256];  // Temporary buffer

        if (sscanf(token, "%s %s", column_name, column_type) != 2) {
            snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Invalid format for column: %s", token);
            return;
        }

        if (strlen(column_name) >= MAX_COLUMN_NAME_LENGTH) {
            snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Column name too long: %s", column_name);
            return;
        }

        strncpy(table.columns[table.column_count].name, column_name, MAX_COLUMN_NAME_LENGTH - 1);
        table.columns[table.column_count].name[MAX_COLUMN_NAME_LENGTH - 1] = '\0';

        ColumnType type = convert_type(column_type);
        if (type == -1) {
            snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Invalid column type '%s'", column_type);
            return;
        }

        table.columns[table.column_count].type = type;
        table.column_count++;

        if (table.column_count >= MAX_COLUMNS) {
            snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Too many columns specified");
            return;
        }
    }

    if (table.column_count == 0) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: No columns specified");
        return;
    }

    if (create_table(&table, session->user_id)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "Table created successfully");
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Failed to create table");
    }

}

void handle_delete_table(Request* req, Response* res, Session* session) {
  if(!signed_in(req, res)) return;
  if(delete_table(req->data, session->user_id)) {
      snprintf(res->data, MAX_CHUNK_SIZE, "Table deleted successfully");
  } else {
      snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Failed to delete table");
  }
  req->session_id = session->session_id;
}

void handle_add_record(Request* req, Response* res, Session* session) {
    if (!signed_in(req, res)) return;

    char table_name[MAX_TABLE_NAME_LENGTH];
    char record_values[MAX_RECORD_LENGTH];

    if (sscanf(req->data, "%s %[^\n]", table_name, record_values) < 2) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Invalid format. Expected {table_name} {value1} {value2}...");
        return;
    }

    if (add_record_table(table_name, session->user_id, record_values)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "Record added successfully to table '%s'", table_name);
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Failed to add record to table '%s'", table_name);
    }

}

void handle_delete_record(Request* req, Response* res, Session* session) {
    if (!signed_in(req, res)) return;

    char table_name[MAX_TABLE_NAME_LENGTH];
    int record_index;

    if (sscanf(req->data, "%s %d", table_name, &record_index) != 2) {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Invalid format. Expected {table_name} {index}");
        return;
    }

    if(record_index > 0 && delete_record_table(table_name, session->user_id, record_index)) {
        snprintf(res->data, MAX_CHUNK_SIZE, "Record deleted successfully from table '%s'", table_name);
    } else {
        snprintf(res->data, MAX_CHUNK_SIZE, "ERROR: Failed to delete record from table '%s'", table_name);
    }
}