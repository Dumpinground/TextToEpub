//
// Created by hasee on 2021/12/13.
//

#include "uuid.h"

std::string uuid4() {
    UUID4_STATE_T state;
    UUID4_T uuid;

    uuid4_seed(&state);
    uuid4_gen(&state, &uuid);

    char buffer[UUID4_STR_BUFFER_SIZE];
    if (!uuid4_to_s(uuid, buffer, sizeof(buffer)))
        return "failed";
    return buffer;
}