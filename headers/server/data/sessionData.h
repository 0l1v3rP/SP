#include "../models/sessionModel.h"

#ifndef SESSIONDATA_H
#define SESSIONDATA_H

_Bool session_create(Session *table);
_Bool session_delete(int id);
Session * session_get(int id);

#endif //SESSIONDATA_H
