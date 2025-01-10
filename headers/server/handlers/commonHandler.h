
#ifndef COMMONHANDLER_H
#define COMMONHANDLER_H
#include "../../sendRecv.h"
#include "./userHandler.h"
#include "./tableHandler.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*CommandHandler)(Request*, Response*);

typedef struct {
    const char* command;
    CommandHandler handler;
} CommandMapping;

void handle_request(Request* req, Response* res);
#endif //COMMONHANDLER_H
