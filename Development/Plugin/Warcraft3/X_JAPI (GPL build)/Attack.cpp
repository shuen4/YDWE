#include <deque>

#include <warcraft3/jass/hook.h>
#include <base/hook/inline.h>

#include "TriggerEvent.h"
#include "util.h"

definePlayerUnitEventData(X_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS, 0, bool hide;);

void(__thiscall* real_CreateMissTextAtUnit)(CUnit* pUnit);
void __fastcall fake_CreateMissTextAtUnit(CUnit* _this) {
    if (_this->GetOwner()->IsEventRegistered(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
        X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)Agile::CreateObject(X_PlayerUnitAttackMissEventData::typeID, false);
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
        if (X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackMissEventData::typeID)) {
            pEventData->hide = hide;
            return 1;
        }
    }
    return 0;
}

uint32_t __cdecl X_IsAttackMissEventTextHidden() {
    switch (TriggerEvent::GetTriggerEventId()) {
    case TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS:
        if (X_PlayerUnitAttackMissEventData* pEventData = (X_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackMissEventData::typeID))
            return pEventData->hide;
        else
            return 0;
    default:
        return 0;
    }
}

definePlayerUnitEventData(X_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, 0, uint32_t weapon_index; uint32_t target_x; uint32_t target_y; uint32_t src_x; uint32_t src_y; bool isPointTarget; bool exposeSource;);

uint32_t(__thiscall* real_CAbilityAttack_AttackTarget)(CAbilityAttack* pAbilityAttack, CWidget* pTargetWidget, uint32_t weapon_index, uint32_t, uint32_t, uint32_t, uint32_t exposeSource);
uint32_t __fastcall fake_CAbilityAttack_AttackTarget(CAbilityAttack* pAbilityAttack, uint32_t, CWidget* target /* widget */, uint32_t weapon_index, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t exposeSource) {
    CUnit* pUnit = pAbilityAttack->GetOwner();
    if (pUnit && pUnit->GetOwner()->IsEventRegistered(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)) {
        X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)Agile::CreateObject(X_PlayerUnitAttackLaunchEventData::typeID, false);
        pEventData->weapon_index = weapon_index;
        pEventData->exposeSource = exposeSource;

        NTempest::C3Vector loc = pUnit->GetPosition()->GetLocation();
        pEventData->src_x = loc.x;
        pEventData->src_y = loc.y;

        if (pAbilityAttack->agent_flag & CAbilityAttack::agent_flag_attack_ground) { // 攻击地面
            CTaskPoint* pCTaskPoint = UTIL::GetAgentByTag<CTaskPoint>(pUnit->task);
            pEventData->isPointTarget = true;
            pEventData->target_x = pCTaskPoint->x.value;
            pEventData->target_y = pCTaskPoint->y.value;
        }
        else {
            loc = target->GetPosition()->GetLocation();
            pEventData->isPointTarget = false;
            pEventData->target_x = loc.x;
            pEventData->target_y = loc.y;
        }
        TriggerEvent::FirePlayerUnitEvent(pEventData, pUnit, (CUnit*)target, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
        
        return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, pAbilityAttack, target, weapon_index, a4, a5, a6, (uint32_t)pEventData->exposeSource);
    }
    return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, pAbilityAttack, target, weapon_index, a4, a5, a6, exposeSource);
}

uint32_t __cdecl X_TriggerRegisterPlayerUnitAttackLaunchEvent(uint32_t trigger, uint32_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, NULL);
}

uint32_t __cdecl X_GetAttackLaunchEventWeaponIndex() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->weapon_index;
    return 0xFFFFFFFF;
}

uint32_t __cdecl X_GetAttackLaunchEventTarget() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            if (CWidget* pWidget = UTIL::GetAgentByTag<CWidget>(pEventData->agent))
                return CGameWar3::GetInstance()->GetGameState()->ToHandle(pWidget, 0);
    return 0;
}

template <class T>
uint32_t __cdecl X_GetAttackLaunchEventTargetTyped() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            if (T* pAgent = UTIL::GetAgentByTag<T>(pEventData->agent))
                    return CGameWar3::GetInstance()->GetGameState()->ToHandle(pAgent, 0);
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventSourceX() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->src_x;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventSourceY() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->src_y;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventTargetX() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->target_x;
    return 0;
}

uint32_t __cdecl X_GetAttackLaunchEventTargetY() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->target_y;
    return 0;
}

uint32_t __cdecl X_IsAttackLaunchEventPointTarget() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->isPointTarget;
    return false;
}

uint32_t __cdecl X_IsAttackLaunchEventExposeSource() {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID))
            return pEventData->exposeSource;
    return false;
}

uint32_t __cdecl X_SetAttackLaunchEventExposeSource(bool flag) {
    if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
        if (X_PlayerUnitAttackLaunchEventData* pEventData = (X_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(X_PlayerUnitAttackLaunchEventData::typeID)) {
            pEventData->exposeSource = flag;
            return true;
        }
    return false;
}
init_L(UnitEvent_Attack) {
    // 玩家单位攻击丢失事件
    setupPlayerUnitEventData_s(X_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
    real_CreateMissTextAtUnit = FUNC::CUnit_DisplayMissText;
    base::hook::install((uint32_t*)&real_CreateMissTextAtUnit, (uint32_t)fake_CreateMissTextAtUnit);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitAttackMissEvent,                "X_TriggerRegisterPlayerUnitAttackMissEvent",                   "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uint32_t)X_SetAttackMissEventTextHidden,                            "X_SetAttackMissEventTextHidden",                               "(B)B");
    jass::japi_add((uint32_t)X_IsAttackMissEventTextHidden,                             "X_IsAttackMissEventTextHidden",                                "()B");
    
    // 玩家单位发起攻击事件
    setupPlayerUnitEventData_s(X_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
    real_CAbilityAttack_AttackTarget = FUNC::CAbilityAttack_AttackTarget;
    base::hook::install((uint32_t*)&real_CAbilityAttack_AttackTarget, (uint32_t)fake_CAbilityAttack_AttackTarget);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitAttackLaunchEvent,              "X_TriggerRegisterPlayerUnitAttackLaunchEvent",                 "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventWeaponIndex,                         "X_GetAttackLaunchEventWeaponIndex",                            "()I");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTarget,                              "X_GetAttackLaunchEventTarget",                                 "()Hwidget;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<CUnit>,                  "X_GetAttackLaunchEventTargetUnit",                             "()Hunit;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<CItem>,                  "X_GetAttackLaunchEventTargetItem",                             "()Hitem;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetTyped<CDestructable>,          "X_GetAttackLaunchEventTargetDestructable",                     "()Hdestructable;");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventSourceX,                             "X_GetAttackLaunchEventSourceX",                                "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventSourceY,                             "X_GetAttackLaunchEventSourceY",                                "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetX,                             "X_GetAttackLaunchEventTargetX",                                "()R");
    jass::japi_add((uint32_t)X_GetAttackLaunchEventTargetY,                             "X_GetAttackLaunchEventTargetY",                                "()R");
    jass::japi_add((uint32_t)X_IsAttackLaunchEventPointTarget,                          "X_IsAttackLaunchEventPointTarget",                             "()B");
    jass::japi_add((uint32_t)X_IsAttackLaunchEventExposeSource,                         "X_IsAttackLaunchEventExposeSource",                            "()B");
    jass::japi_add((uint32_t)X_SetAttackLaunchEventExposeSource,                        "X_SetAttackLaunchEventExposeSource",                           "(B)B");
}