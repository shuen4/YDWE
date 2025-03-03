#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_GroupGetSize(uint32_t group) {
    CGroup* pGroup = ConvertHandle<CGroup>(group);
    if (!pGroup)
        return NULL;

    return pGroup->units.count;
}

uint32_t __cdecl X_GroupUnitAt(uint32_t group, uint32_t index) {
    CGroup* pGroup = ConvertHandle<CGroup>(group);
    if (!pGroup)
        return NULL;

    CUnit* pUnit = pGroup->units.At(index);
    if (!pUnit)
        return NULL;

    return CGameWar3::GetInstance()->GetGameState()->ToHandle(pUnit, 0);
}

init_L(Group) {
    jass::japi_add((uint32_t)X_GroupGetSize,       "X_GroupGetSize",    "(Hgroup;)I");
    jass::japi_add((uint32_t)X_GroupUnitAt,        "X_GroupUnitAt",     "(Hgroup;I)Hunit;");
}