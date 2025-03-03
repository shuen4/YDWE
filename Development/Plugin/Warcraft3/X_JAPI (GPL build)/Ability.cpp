#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_StartUnitAbilityCooldown(uint32_t unit, uint32_t abilID, float* cooldown) {
    if (*cooldown <= 0)
        return false;

    CUnit* pUnit = ConvertHandle<CUnit>(unit);
    if (!pUnit)
        return false;

    CAbilitySpell* pAbilitySpell = ConvertAgent<CAbilitySpell>(pUnit->GetAbilityByID(abilID, 0, 1, 1, 1));
    if (!pAbilitySpell)
        return false;

    if (*cooldown > 300) // 实际上 CAgentTimer 也不会允许超过 300 秒
        *cooldown = 300.f;

    pAbilitySpell->StartCD(cooldown);
    return true;
}

uint32_t __cdecl X_IsUnitAbilityInCooldown(uint32_t unit, uint32_t abilID) {
    CUnit* pUnit = ConvertHandle<CUnit>(unit);
    if (!pUnit)
        return false;

    CAbilitySpell* pAbilitySpell = ConvertAgent<CAbilitySpell>(pUnit->GetAbilityByID(abilID, 0, 1, 1, 1));
    if (!pAbilitySpell)
        return false;

    return !!(pAbilitySpell->agent_flag & CAbilitySpell::agent_flag_in_cooldown);
}

uint32_t __cdecl X_EndUnitAbilityCooldown(uint32_t unit, uint32_t abilID) {
    CUnit* pUnit = ConvertHandle<CUnit>(unit);
    if (!pUnit)
        return false;

    CAbilitySpell* pAbilitySpell = ConvertAgent<CAbilitySpell>(pUnit->GetAbilityByID(abilID, 0, 1, 1, 1));
    if (!pAbilitySpell)
        return false;

    pAbilitySpell->StopCD();
    return true;
}

init_L(Ability) {
    jass::japi_add((uint32_t)X_StartUnitAbilityCooldown,        "X_StartUnitAbilityCooldown",       "(Hunit;IR)B");
    jass::japi_add((uint32_t)X_IsUnitAbilityInCooldown,         "X_IsUnitAbilityInCooldown",        "(Hunit;I)B");
    jass::japi_add((uint32_t)X_EndUnitAbilityCooldown,          "X_EndUnitAbilityCooldown",         "(Hunit;I)B");
}