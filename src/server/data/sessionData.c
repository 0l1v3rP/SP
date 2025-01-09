#include "../../../headers/server/data/sessionData.h"
#include "../../../headers/server/data/genericData.h"

_Bool session_create(Session *session) {
    return create_generic("sessions.dat", session, sizeof(Session));
}

Session* session_get(int id) {
    return (Session *)get_by_id_generic("sessions.dat", id, offsetof(Session, session_id), sizeof(Session));
}

_Bool session_delete(int id) {
    return delete_generic("sessions.dat", id, offsetof(Session, session_id), sizeof(Session));
}