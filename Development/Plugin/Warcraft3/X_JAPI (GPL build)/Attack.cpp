#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <base/hook/replace_pointer.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <deque>
#include "TriggerEvent.h"
#include "util.h"

uint32_t searchCreateMissTextAtUnit() {
    war3_searcher& s = get_war3_searcher();
    uint32_t ptr = s.search_string("MISS", sizeof("MISS"));
    return s.current_function(ptr);
}
uint32_t searchCAbilityAttack_AttackTarget() {
    uint32_t ptr = ReadMemory(get_vfn_ptr(".?AVCAbilityRangerArrow@@") + 0x424);
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}
uint32_t searchSmartPosition_GetLocation() {
    uint32_t ptr = get_war3_searcher().search_string("SetUnitX");
    ptr = ReadMemory(ptr + 0x05);
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

defineEventData(X_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS, 0, bool hide;);

uint32_t real_CreateMissTextAtUnit = 0;
void __fastcall fake_CreateMissTextAtUnit(uint32_t _this) {
    if (TriggerEvent::IsEventRegistered(TriggerEvent::GetUnitOwner(_this), TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
        X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)create_by_typeid(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
        pEventData->hide = false;
        TriggerEvent::FirePlayerUnitEvent(pEventData, _this, NULL, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
        if (pEventData->hide)
            return;
    }
    base::this_call<void>(real_CreateMissTextAtUnit, _this);
}

uint32_t __cdecl X_TriggerRegisterPlayerUnitAttackMissEvent(uint32_t trigger, uint32_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS, NULL);
}

uint32_t __cdecl X_SetAttackMissEventTextHidden(bool hide) {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS) {
        if (X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
            pEventData->hide = hide;
            return 1;
        }
    }
    return 0;
}

uint32_t __cdecl X_IsAttackMissEventTextHidden() {
    switch (TriggerEvent::GetTriggerEventId()) {
    case TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS:
        if (X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS))
            return pEventData->hide;
        else
            return 0;
    default:
        return 0;
    }
}

defineEventData(X_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, 0, uint32_t weapon_index; uint32_t target_x; uint32_t target_y; uint32_t src_x; uint32_t src_y; bool isPointTarget; bool exposeSource;);

uint32_t real_CAbilityAttack_AttackTarget = 0;
uint32_t __fastcall fake_CAbilityAttack_AttackTarget(uint32_t _this, uint32_t, uint32_t target /* widget */, uint32_t weapon_index, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t exposeSource) {
    uint32_t pUnit = ReadMemory(_this + 0x30);
    if (!pUnit)
        pUnit = GetObjectByHash_agl(ReadMemory(_this + 0xC), ReadMemory(_this + 0x10));
    if (TriggerEvent::IsEventRegistered(TriggerEvent::GetUnitOwner(pUnit), TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)) {
        X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)create_by_typeid(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
        pEventData->weapon_index = weapon_index;
        pEventData->exposeSource = exposeSource;

        uint32_t loc[3];
        static uint32_t pSmartPosition_GetLocation = searchSmartPosition_GetLocation();
        base::this_call<void>(pSmartPosition_GetLocation, base::this_call_vf<uint32_t>(pUnit, 0xB8), loc);
        pEventData->src_x = loc[0];
        pEventData->src_y = loc[1];

        if (ReadMemory(_this + 0x20) & 0x200) { // 攻击地面
            uint32_t pCTaskPoint = GetObjectByHash(ReadMemory(pUnit + 0x174), ReadMemory(pUnit + 0x178));
            pEventData->isPointTarget = true;
            pEventData->target_x = ReadMemory(pCTaskPoint + 0x38);
            pEventData->target_y = ReadMemory(pCTaskPoint + 0x40);
        }
        else {
            base::this_call<void>(pSmartPosition_GetLocation, base::this_call_vf<uint32_t>(target, 0xB8), loc);
            pEventData->isPointTarget = false;
            pEventData->target_x = loc[0];
            pEventData->target_y = loc[1];
        }
        TriggerEvent::FirePlayerUnitEvent(pEventData, pUnit, target, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
        
        return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, _this, target, weapon_index, a4, a5, a6, (uint32_t)pEventData->exposeSource);
    }
    return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, _this, target, weapon_index, a4, a5, a6, exposeSource);
}

uint32_t __cdecl X_TriggerRegisterPlayerUnitAttackLaunchEvent(uint32_t trigger, uint32_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, NULL);
}

uint32_t __cdecl X_GetAttackLaunchEventWeaponIndex() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->weapon_index;
    return 0xFFFFFFFF;
}

uint32_t __cdecl X_GetAttackLaunchEventTarget() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            if (uint32_t pWidget = GetObjectByHash(pEventData->filterUnit.a, pEventData->filterUnit.b))
                return create_handle(pWidget);
    return 0;
}

template <uint32_t typeID>
uint32_t __cdecl X_GetAttackLaunchEventTargetTyped() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            if (uint32_t pWidget = GetObjectByHash(pEventData->filterUnit.a, pEventData->filterUnit.b))
                if (type_check_s(pWidget, typeID))
                    return create_handle(pWidget);
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventSourceX() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->src_x;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventSourceY() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->src_y;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventTargetX() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->target_x;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventTargetY() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->target_y;
    return 0;
}

uint32_t __cdecl X_IsAttackLaunchEventPointTarget() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->isPointTarget;
    return false;
}

uint32_t __cdecl X_IsAttackLaunchEventExposeSource() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
            return pEventData->exposeSource;
    return false;
}

uint32_t __cdecl X_SetAttackLaunchEventExposeSource(bool flag) {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)) {
            pEventData->exposeSource = flag;
            return true;
        }
    return false;
}
init(UnitEvent_Attack) {
    // 玩家单位攻击丢失事件
    setupEventData_s(X_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
    real_CreateMissTextAtUnit = searchCreateMissTextAtUnit();
    base::hook::install(&real_CreateMissTextAtUnit, (uint32_t)fake_CreateMissTextAtUnit);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitAttackMissEvent,        "X_TriggerRegisterPlayerUnitAttackMissEvent",       "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uint32_t)X_SetAttackMissEventTextHidden,                    "X_SetAttackMissEventTextHidden",                   "(B)B");
    jass::japi_add((uint32_t)X_IsAttackMissEventTextHidden,                     "X_IsAttackMissEventTextHidden",                    "()B");
    
    // 玩家单位发起攻击事件
    setupEventData_s(X_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
    real_CAbilityAttack_AttackTarget = searchCAbilityAttack_AttackTarget();
    base::hook::install(&real_CAbilityAttack_AttackTarget, (uint32_t)fake_CAbilityAttack_AttackTarget);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitAttackLaunchEvent,      "X_TriggerRegisterPlayerUnitAttackLaunchEvent",     "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventWeaponIndex,                 "X_GetAttackLaunchEventWeaponIndex",                "()I");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTarget,                      "X_GetAttackLaunchEventTarget",                     "()Hwidget;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<'+w3u'>,         "X_GetAttackLaunchEventTargetUnit",                 "()Hunit;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<'item'>,         "X_GetAttackLaunchEventTargetItem",                 "()Hitem;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<'+w3d'>,         "X_GetAttackLaunchEventTargetDestructable",         "()Hdestructable;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventSourceX,                     "X_GetAttackLaunchEventSourceX",                    "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventSourceY,                     "X_GetAttackLaunchEventSourceY",                    "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetX,                     "X_GetAttackLaunchEventTargetX",                    "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetY,                     "X_GetAttackLaunchEventTargetY",                    "()R");
    jass::japi_add((uint32_t)X_IsAttackLaunchEventPointTarget,                  "X_IsAttackLaunchEventPointTarget",                 "()B");
    jass::japi_add((uint32_t)X_IsAttackLaunchEventExposeSource,                 "X_IsAttackLaunchEventExposeSource",                "()B");
    jass::japi_add((uint32_t)X_SetAttackLaunchEventExposeSource,                "X_SetAttackLaunchEventExposeSource",               "(B)B");
}