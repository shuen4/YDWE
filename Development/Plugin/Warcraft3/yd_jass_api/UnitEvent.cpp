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

namespace warcraft3::japi {

    uint32_t searchCreateMissTextAtUnit() {
        war3_searcher& s = get_war3_searcher();
        uint32_t ptr = s.search_string("MISS", sizeof("MISS"));
        return s.current_function(ptr);
    }
    uint32_t searchCAbilityAttack_AttackTarget() {
        war3_searcher& s = get_war3_searcher();
        uint32_t ptr = s.search_int_in_text(0x73707269);
        return s.current_function(ptr);
    }
    uintptr_t searchSmartPosition_GetLocation() {
        uintptr_t ptr;

        //=========================================
        // (1)
        //
        // push		"()V"
        // mov		edx, "SetUnitX"
        // mov		ecx, [SetUnitX函数的地址]  <----
        // call		BindNative
        //=========================================
        ptr = get_war3_searcher().search_string("SetUnitX");
        ptr = ReadMemory(ptr + 0x05);

        //=========================================
        // (2)
        //  SetUnitX:
        //    ...
        //    call		ConvertHandle
        //    ...
        //    call		GetSmartPosition (vfn)
        //    ...
        //    call      SmartPosition::GetLocation <----
        //    
        //=========================================
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xFF, 2);
        ptr += 2;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);

        return ptr;
    }

    defineEventData(JAPI_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS, 0, bool hide;);

    uint32_t real_CreateMissTextAtUnit = 0;
    void __fastcall fake_CreateMissTextAtUnit(uint32_t _this) {
        if (TriggerEvent::IsEventRegistered(TriggerEvent::GetUnitOwner(_this), TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
            JAPI_PlayerUnitAttackMissEventData* pPlayerUnitAttackMissEventData = (JAPI_PlayerUnitAttackMissEventData*)create_by_typeid(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
            pPlayerUnitAttackMissEventData->hide = false;
            TriggerEvent::FirePlayerUnitEvent(pPlayerUnitAttackMissEventData, _this, NULL, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
            if (pPlayerUnitAttackMissEventData->hide)
                return;
        }
        base::this_call<void>(real_CreateMissTextAtUnit, _this);
    }

    uint32_t __cdecl EXTriggerRegisterPlayerUnitAttackMissEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
        return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS, NULL);
    }

    uint32_t __cdecl EXHideAttackMissEventText(bool hide) {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS) {
            if (JAPI_PlayerUnitAttackMissEventData* pEventData = (JAPI_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
                pEventData->hide = hide;
                return 1;
            }
        }
        return 0;
    }

    uint32_t __cdecl EXIsAttackMissEventTextHidden() {
        switch (TriggerEvent::GetTriggerEventId()) {
        case TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS:
            if (JAPI_PlayerUnitAttackMissEventData* pEventData = (JAPI_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS))
                return pEventData->hide;
            else
                return 0;
        default:
            return 0;
        }
    }

    defineEventData(JAPI_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, 0, uint32_t weapon_index; uint32_t target_x; uint32_t target_y; uint32_t src_x; uint32_t src_y; bool isPointTarget; bool vision;);

    uint32_t real_CAbilityAttack_AttackTarget = 0;
    uint32_t __fastcall fake_CAbilityAttack_AttackTarget(uint32_t _this, uint32_t, uint32_t target /* widget */, uint32_t weapon_index, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t vision) {
        uint32_t pUnit = ReadMemory(_this + 0x30);
        if (!pUnit)
            pUnit = GetObjectByHash_agl(ReadMemory(_this + 0xC), ReadMemory(_this + 0x10));
        if (TriggerEvent::IsEventRegistered(TriggerEvent::GetUnitOwner(pUnit), TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)) {
            JAPI_PlayerUnitAttackLaunchEventData* pPlayerUnitAttackLaunchEventData = (JAPI_PlayerUnitAttackLaunchEventData*)create_by_typeid(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
            pPlayerUnitAttackLaunchEventData->weapon_index = weapon_index;
            pPlayerUnitAttackLaunchEventData->vision = vision;

            uint32_t loc[3];
            static uint32_t pSmartPosition_GetLocation = searchSmartPosition_GetLocation();
            base::this_call<void>(pSmartPosition_GetLocation, base::this_call_vf<uint32_t>(pUnit, 0xB8), loc);
            pPlayerUnitAttackLaunchEventData->src_x = loc[0];
            pPlayerUnitAttackLaunchEventData->src_y = loc[1];

            if (ReadMemory(_this + 0x20) & 0x200) { // 攻击地面
                uint32_t pCTaskPoint = GetObjectByHash(ReadMemory(pUnit + 0x174), ReadMemory(pUnit + 0x178));
                pPlayerUnitAttackLaunchEventData->isPointTarget = true;
                pPlayerUnitAttackLaunchEventData->target_x = ReadMemory(pCTaskPoint + 0x38);
                pPlayerUnitAttackLaunchEventData->target_y = ReadMemory(pCTaskPoint + 0x40);
            }
            else {
                base::this_call<void>(pSmartPosition_GetLocation, base::this_call_vf<uint32_t>(target, 0xB8), loc);
                pPlayerUnitAttackLaunchEventData->isPointTarget = false;
                pPlayerUnitAttackLaunchEventData->target_x = loc[0];
                pPlayerUnitAttackLaunchEventData->target_y = loc[1];
            }
            TriggerEvent::FirePlayerUnitEvent(pPlayerUnitAttackLaunchEventData, pUnit, target, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
            
            return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, _this, target, weapon_index, a4, a5, a6, (uint32_t)pPlayerUnitAttackLaunchEventData->vision);
        }
        return base::this_call<uint32_t>(real_CAbilityAttack_AttackTarget, _this, target, weapon_index, a4, a5, a6, vision);
    }

    uint32_t __cdecl EXTriggerRegisterPlayerUnitAttackLaunchEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
        return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH, NULL);
    }

    uint32_t __cdecl EXGetAttackLaunchEventWeaponIndex() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->weapon_index;
        return 0xFFFFFFFF;
    }

    uint32_t __cdecl EXGetAttackLaunchEventTarget() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                if (uint32_t pWidget = GetObjectByHash(pEventData->filterUnit.a, pEventData->filterUnit.b))
                    return create_handle(pWidget);
        return 0;
    }

    template <uint32_t typeID>
    uint32_t __cdecl EXGetAttackLaunchEventTargetTyped() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                if (uint32_t pWidget = GetObjectByHash(pEventData->filterUnit.a, pEventData->filterUnit.b))
                    if (type_check(get_object_type(pWidget), typeID))
                        return create_handle(pWidget);
        return 0;
    }

    uint32_t __cdecl EXGetAttackLaunchEventSourceX() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->src_x;
        return 0;
    }

    uint32_t __cdecl EXGetAttackLaunchEventSourceY() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->src_y;
        return 0;
    }

    uint32_t __cdecl EXGetAttackLaunchEventTargetX() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->target_x;
        return 0;
    }

    uint32_t __cdecl EXGetAttackLaunchEventTargetY() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->target_y;
        return 0;
    }

    uint32_t __cdecl EXIsAttackLaunchEventPointTarget() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->isPointTarget;
        return false;
    }

    uint32_t __cdecl EXIsAttackLaunchEventGiveVision() {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH))
                return pEventData->vision;
        return false;
    }

    uint32_t __cdecl EXSetAttackLaunchEventGiveVision(bool vision) {
        if (TriggerEvent::GetTriggerEventId() == TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)
            if (JAPI_PlayerUnitAttackLaunchEventData* pEventData = (JAPI_PlayerUnitAttackLaunchEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH)) {
                pEventData->vision = vision;
                return true;
            }
        return false;
    }

    void InitializeUnitEvent() {
        // 玩家单位攻击丢失事件
        setupEventData_s(JAPI_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);
        real_CreateMissTextAtUnit = searchCreateMissTextAtUnit();
        base::hook::install(&real_CreateMissTextAtUnit, (uint32_t)fake_CreateMissTextAtUnit);
        jass::japi_add((uint32_t)EXTriggerRegisterPlayerUnitAttackMissEvent,        "EXTriggerRegisterPlayerUnitAttackMissEvent",       "(Htrigger;Hplayer;)Hevent;");
        jass::japi_add((uint32_t)EXHideAttackMissEventText,                         "EXHideAttackMissEventText",                        "(B)B");
        jass::japi_add((uint32_t)EXIsAttackMissEventTextHidden,                     "EXIsAttackMissEventTextHidden",                    "()B");
        
        // 玩家单位发起攻击事件
        setupEventData_s(JAPI_PlayerUnitAttackLaunchEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_LAUNCH);
        real_CAbilityAttack_AttackTarget = searchCAbilityAttack_AttackTarget();
        base::hook::install(&real_CAbilityAttack_AttackTarget, (uint32_t)fake_CAbilityAttack_AttackTarget);
        jass::japi_add((uint32_t)EXTriggerRegisterPlayerUnitAttackLaunchEvent,      "EXTriggerRegisterPlayerUnitAttackLaunchEvent",     "(Htrigger;Hplayer;)Hevent;");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventWeaponIndex,                 "EXGetAttackLaunchEventWeaponIndex",                "()I");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTarget,                      "EXGetAttackLaunchEventTarget",                     "()Hwidget;");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTargetTyped<'+w3u'>,         "EXGetAttackLaunchEventTargetUnit",                 "()Hunit;");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTargetTyped<'item'>,         "EXGetAttackLaunchEventTargetItem",                 "()Hitem;");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTargetTyped<'+w3d'>,         "EXGetAttackLaunchEventTargetDestructable",         "()Hdestructable;");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventSourceX,                     "EXGetAttackLaunchEventSourceX",                    "()R");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventSourceY,                     "EXGetAttackLaunchEventSourceY",                    "()R");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTargetX,                     "EXGetAttackLaunchEventTargetX",                    "()R");
        jass::japi_add((uint32_t)EXGetAttackLaunchEventTargetY,                     "EXGetAttackLaunchEventTargetY",                    "()R");
        jass::japi_add((uint32_t)EXIsAttackLaunchEventPointTarget,                  "EXIsAttackLaunchEventPointTarget",                 "()B");
        jass::japi_add((uint32_t)EXIsAttackLaunchEventGiveVision,                   "EXIsAttackLaunchEventGiveVision",                  "()B");
        jass::japi_add((uint32_t)EXSetAttackLaunchEventGiveVision,                  "EXSetAttackLaunchEventGiveVision",                 "(B)B");

    }
}
