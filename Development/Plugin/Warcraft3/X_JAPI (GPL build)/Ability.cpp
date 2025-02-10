#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "CAgentTimer.h"
#include "util.h"

uint32_t searchCUnit_GetAbilityByID() {
    uint32_t ptr = get_war3_searcher().search_string("UnitRemoveAbility");
    ptr = *(uint32_t*)(ptr + 0x05);
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

uint32_t searchCAbility_RefreshUI() {
    uint32_t ptr = ReadMemory(get_vfn_ptr(".?AVCAbilityStoneForm@@") + 0x414);
    ptr = next_opcode(ptr, 0xE9, 5);
    return convert_function(ptr);
}

uint32_t __cdecl X_StartUnitAbilityCooldown(uint32_t unit, uint32_t abilID, float* cooldown) {
    if (!cooldown  || *cooldown <= 0)
        return false;

    uint32_t pUnit = handle_to_object(unit);
    if (!pUnit || !type_check_s(pUnit, '+w3u'))
        return false;

    static uint32_t pCUnit_GetAbilityByID = searchCUnit_GetAbilityByID();
    uint32_t pAbility = base::this_call<uint32_t>(pCUnit_GetAbilityByID, pUnit, abilID, 0, 1, 1, 1);
    if (!pAbility || !type_check_s(pAbility, 'AAsp'))
        return false;

    uint32_t pAgentTimer = pAbility + 0xD0;
    if (ReadMemory(pAbility + 0x20) & 0x200)
        // 当前已在冷却中
        CAgentTimer_Stop(pAgentTimer);
    else
        WriteMemory(pAbility + 0x20, ReadMemory(pAbility + 0x20) | 0x200);

    CAgentTimer_Start(pAgentTimer, cooldown, 0xD01BE, pAbility, 0, 0);
    WriteMemory(pAbility + 0xB4, *cooldown);

    static uint32_t pCAbility_RefreshUI = searchCAbility_RefreshUI();
    base::this_call<void>(pCAbility_RefreshUI, pAbility);
    return true;
}

init(Ability) {
    jass::japi_add((uint32_t)X_StartUnitAbilityCooldown,        "X_StartUnitAbilityCooldown",       "(Hunit;IR)B");
}