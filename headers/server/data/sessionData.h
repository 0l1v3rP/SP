#include "../models/sessionModel.h"

#ifndef SESSIONDATA_H
#define SESSIONDATA_H

_Bool * session_create(Session *table);
_Bool session_delete(int id);
Session * session_get(int id);
Session * user_session_get(int user_id);

#endif //SESSIONDATA_H
