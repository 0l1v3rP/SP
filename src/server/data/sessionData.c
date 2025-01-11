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
_Bool match_user_id(void *record, void *context) {
    int *record_user_id = (int *)((char *)record + *(size_t *)context);
    int *target_user_id = (int *)((char *)context + sizeof(size_t));
    return *record_user_id == *target_user_id;
}

Session* session_get_by_user_id(int user_id) {
    size_t context[2] = { offsetof(Session, user_id), user_id };
    return (Session *)get_by_condition_generic("sessions.dat", sizeof(Session), match_user_id, context);
}