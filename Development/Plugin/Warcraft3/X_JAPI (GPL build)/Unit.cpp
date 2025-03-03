#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t _cdecl X_IsUnitInvulnerable(uint32_t unit) {
    CUnit* pUnit = ConvertHandle<CUnit>(unit);
    
    if (!pUnit)
        return false;

    return (pUnit->agent_flag & CWidget::agent_flag_is_invulnerable) != 0;
}

init_L(Unit) {
    jass::japi_add((uint32_t)X_IsUnitInvulnerable,     "X_IsUnitInvulnerable",     "(Hunit;)B");
}