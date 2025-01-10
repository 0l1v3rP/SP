#ifndef USERHANDLER_H
#define USERHANDLER_H
#include "../../sendRecv.h"
#include "./commonHandler.h"

void handle_register(Request* req, Response* res);
void handle_login(Request* req, Response* res);
void handle_logout(Request* req, Response* res);
void handle_delete_user(Request* req, Response* res);
#endif //USERHANDLER_H
