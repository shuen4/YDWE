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

    uint32_t searchCreateMissTextAtUnit()
    {
        war3_searcher& s = get_war3_searcher();
        uint32_t ptr = s.search_string("MISS", sizeof("MISS"));
        return s.current_function(ptr);
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
        switch (TriggerEvent::GetTriggerEventId()) {
        case TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS:
            if (JAPI_PlayerUnitAttackMissEventData* pEventData = (JAPI_PlayerUnitAttackMissEventData*)TriggerEvent::GetTriggerEventData(TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS)) {
                pEventData->hide = hide;
                return 1;
            }
            else
                return 0;
        default:
            return 0;
        }
    }

    bool __cdecl EXIsAttackMissEventTextHidden() {
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

    void InitializeUnitEvent() {
        setupEventData_s(JAPI_PlayerUnitAttackMissEventData, TriggerEvent::EVENT_PLAYER_UNIT_ATTACK_MISS);

        // 玩家单位攻击丢失事件
        real_CreateMissTextAtUnit = searchCreateMissTextAtUnit();
        base::hook::install(&real_CreateMissTextAtUnit, (uint32_t)fake_CreateMissTextAtUnit);
        jass::japi_add((uint32_t)EXTriggerRegisterPlayerUnitAttackMissEvent,        "EXTriggerRegisterPlayerUnitAttackMissEvent",       "(Htrigger;Hplayer;)Hevent;");
        jass::japi_add((uint32_t)EXHideAttackMissEventText,                         "EXHideAttackMissEventText",                        "(B)B");
        jass::japi_add((uint32_t)EXIsAttackMissEventTextHidden,                     "EXIsAttackMissEventTextHidden",                    "()B");
    }
}
