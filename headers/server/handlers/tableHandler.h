#ifndef TABLEHANDLER_H
#define TABLEHANDLER_H
#include "../../sendRecv.h"
#include "./commonHandler.h"
void handle_create_table(Request* req, Response* res, Session* session);
void handle_delete_table(Request* req, Response* res, Session* session);
void handle_add_record(Request* req, Response* res, Session* session);
void handle_delete_record(Request* req, Response* res, Session* session);
#endif //TABLEHANDLER_H
