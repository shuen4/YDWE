#include <warcraft3/jass/hook.h>
#include <base/hook/inline.h>

#include "TriggerEvent.h"
#include "util.h"

definePlayerUnitEventData(X_PlayerUnitUpdateInventoryEventData, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY, 0, );

void(__thiscall* real_CUnit_UpdateInventory)(CUnit* pUnit);
void __fastcall fake_CUnit_UpdateInventory(CUnit* pUnit) {
    if (pUnit->GetOwner()->IsEventRegistered(TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY)) {
        X_PlayerUnitUpdateInventoryEventData* pEventData = (X_PlayerUnitUpdateInventoryEventData*)Agile::CreateObject(X_PlayerUnitUpdateInventoryEventData::typeID, false);

        TriggerEvent::FirePlayerUnitEvent(pEventData, pUnit, NULL, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY);
    }
    real_CUnit_UpdateInventory(pUnit);
}

uint32_t __cdecl X_TriggerRegisterPlayerUnitUpdateInventoryEvent(uint32_t trigger, uint32_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY, NULL);
}

init_L(UnitEvent_Inventory) {
    // 玩家单位更新物品栏事件 
    // 丢弃物品 获取物品 科技升级 之类的
    setupPlayerUnitEventData_s(X_PlayerUnitUpdateInventoryEventData, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY);
    real_CUnit_UpdateInventory = FUNC::CUnit_UpdateInventoryUI;
    base::hook::install((uint32_t*)&real_CUnit_UpdateInventory, (uint32_t)fake_CUnit_UpdateInventory);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitUpdateInventoryEvent,           "X_TriggerRegisterPlayerUnitUpdateInventoryEvent",          "(Htrigger;Hplayer;)Hevent;");
}