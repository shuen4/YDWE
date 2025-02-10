#include <warcraft3/war3_searcher.h>
#include <warcraft3/jass/hook.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include "TriggerEvent.h"
#include "util.h"

uint32_t searchCUnit_UpdateInventory() {
    uint32_t ptr = ReadMemory(get_vfn_ptr(".?AVCAbilityPurchaseItem@@") + 0xC);
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

defineEventData(X_PlayerUnitUpdateInventoryEventData, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY, 0, );

uint32_t real_CUnit_UpdateInventory = 0;
uint32_t __fastcall fake_CUnit_UpdateInventory(uint32_t _this) {
    if (TriggerEvent::IsEventRegistered(TriggerEvent::GetUnitOwner(_this), TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY)) {
        X_PlayerUnitUpdateInventoryEventData* pEventData = (X_PlayerUnitUpdateInventoryEventData*)create_by_typeid(TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY);

        TriggerEvent::FirePlayerUnitEvent(pEventData, _this, NULL, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY);
    }
    return base::this_call<uint32_t>(real_CUnit_UpdateInventory, _this);
}

uint32_t __cdecl X_TriggerRegisterPlayerUnitUpdateInventoryEvent(uint32_t trigger, uint32_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY, NULL);
}

init(UnitEvent_Inventory) {
    // 玩家单位更新物品栏事件 
    // 丢弃物品 获取物品 科技升级 之类的
    setupEventData_s(X_PlayerUnitUpdateInventoryEventData, TriggerEvent::EVENT_PLAYER_UNIT_UPDATE_INVENTORY);
    real_CUnit_UpdateInventory = searchCUnit_UpdateInventory();
    base::hook::install(&real_CUnit_UpdateInventory, (uint32_t)fake_CUnit_UpdateInventory);
    jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitUpdateInventoryEvent,       "X_TriggerRegisterPlayerUnitUpdateInventoryEvent",      "(Htrigger;Hplayer;)Hevent;");
}