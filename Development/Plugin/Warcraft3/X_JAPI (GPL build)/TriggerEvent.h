#pragma once

#include "util.h"

namespace TriggerEvent {
    enum JAPI_eventid : uint32_t {
        // 这个 事件ID 使用 1.31+ 的魔兽 事件ID
        EVENT_PLAYER_UNIT_DAMAGED = 0x80334,
        EVENT_PLAYER_UNIT_DAMAGING = 0x8033B,
        // 自定义的 事件ID 使用 0x81200 以上
        EVENT_PLAYER_UNIT_ATTACK_MISS = 0x81200,
        EVENT_PLAYER_UNIT_ATTACK_LAUNCH,
        EVENT_PLAYER_UNIT_UPDATE_INVENTORY,
    };
    class CPlayerUnitEventDataBase_vtable {
    public:
        uint32_t unk_func1[7];
        uint32_t GetTypeID;
        uint32_t unk_func2[14];
        uint32_t GetTypeName;
        uint32_t unk_func3[2];
        CUnit*(__thiscall* GetFilterUnit)();
        CPlayerUnitEventDataBase_vtable() {
            memset(this, 0, sizeof(CPlayerUnitEventDataBase_vtable));
        };
        void real_init() {
            memcpy(this, (void*)VFN::CPlayerUnitEventDataBase, sizeof(CPlayerUnitEventDataBase_vtable));
        };
        CPlayerUnitEventDataBase_vtable* copy() {
            CPlayerUnitEventDataBase_vtable* ret = new CPlayerUnitEventDataBase_vtable();
            memcpy(ret, this, sizeof(CPlayerUnitEventDataBase_vtable));
            return ret;
        }
    };
    extern CPlayerUnitEventDataBase_vtable CPlayerUnitEventDataBase_vtable_instance;
    void CPlayerUnitEventDataBase_ctor(CPlayerUnitEventDataBase* _this);
    class X_CPlayerUnitEventDataBase : public war3::CPlayerUnitEventDataBase {
    public:
        constexpr static uint32_t typeID = 'Xpue';

        uint32_t GetTypeID();
        const char* GetTypeName();
        static void ctor(X_CPlayerUnitEventDataBase* _this);
    };
    #define definePlayerUnitEventData(name, id, filterunit, code)                                                                         \
        TriggerEvent::CPlayerUnitEventDataBase_vtable* name##_vtable;                                                           \
        class name : public TriggerEvent::X_CPlayerUnitEventDataBase {                                                          \
        public:                                                                                                                 \
            constexpr static uint32_t typeID = id;                                                                              \
            code                                                                                                                \
            uint32_t GetTypeID() {                                                                                              \
                return id;                                                                                                      \
            }                                                                                                                   \
            const char* GetTypeName() {                                                                                         \
                return #name;                                                                                                   \
            }                                                                                                                   \
            uint32_t GetFilterUnit() {                                                                                          \
                return filterunit;                                                                                              \
            }                                                                                                                   \
            static void ctor(X_CPlayerUnitEventDataBase* _this) {                                                               \
                TriggerEvent::CPlayerUnitEventDataBase_ctor(_this);                                                             \
                ((name*)_this)->__vfn = (uint32_t)name##_vtable;                                                                \
            }                                                                                                                   \
        };
#define setupEventData(name, eventid, ...) do {                                                                                 \
        name##_vtable = TriggerEvent::CPlayerUnitEventDataBase_vtable_instance.copy();                                          \
        WriteMemory((uint32_t)&name##_vtable->GetTypeID, &name::GetTypeID);                                                     \
        WriteMemory((uint32_t)&name##_vtable->GetTypeName, &name::GetTypeName);                                                 \
        WriteMemory((uint32_t)&name##_vtable->GetFilterUnit, &name::GetFilterUnit);                                             \
        TriggerEvent::RegisterTriggerEventData(eventid, TriggerEvent::X_CPlayerUnitEventDataBase::typeID, { __VA_ARGS__}, sizeof(name), 0x10, (void(*)(uint32_t))name::ctor); } while(0)
#define setupPlayerUnitEventData_s(name, eventid) setupEventData(name, eventid, eventid)
    void RegisterTriggerEventData(uint32_t typeID, uint32_t parentTypeID, std::vector<uint32_t> eventID, uint32_t size, uint32_t batchAllocCount, void(*ctor)(uint32_t), void(*dtor)(uint32_t) = NULL);
    uint32_t TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t boolexpr);
    uint32_t GetTriggerEventId();
    CScriptEventData* GetTriggerEventData(uint32_t typeID = 'wscd');
    void FirePlayerUnitEvent(CPlayerUnitEventDataBase* pPlayerUnitEvent, CUnit* pUnit, CAgent* pAgent, uint32_t eventID);
}