#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t _cdecl X_IsUnitInvulnerable(uint32_t unit) {
    uint32_t pUnit = handle_to_object(unit);

    if (pUnit && type_check_s(pUnit, '+w3u'))
        return !!(ReadMemory<uint8_t>(pUnit + 0x20) & 0b00001000);

    return false;
}

init(Unit) {
    jass::japi_add((uint32_t)X_IsUnitInvulnerable,     "X_IsUnitInvulnerable",     "(Hunit;)B");
}