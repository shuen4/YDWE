#pragma once
#include "util.h"

namespace TriggerEvent {
    enum class Type : uint32_t {
        CPlayerUnitEventDataBase = 'pued',
    };
    enum JAPI_eventid : uint32_t {
        EVENT_PLAYER_UNIT_ATTACK_MISS = 0x81000,
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
        uint32_t GetFilterUnit;
        CPlayerUnitEventDataBase_vtable() {
            memcpy(this, (void*)get_vfn_ptr(".?AVCPlayerUnitEventDataBase@@"), sizeof(CPlayerUnitEventDataBase_vtable));
        };
        CPlayerUnitEventDataBase_vtable* copy() {
            CPlayerUnitEventDataBase_vtable* ret = new CPlayerUnitEventDataBase_vtable();
            memcpy(ret, this, sizeof(CPlayerUnitEventDataBase_vtable));
            return ret;
        }
    };
    extern CPlayerUnitEventDataBase_vtable CPlayerUnitEventDataBase_vtable_instance;
    class CPlayerUnitEventDataBase {
    public:
        CPlayerUnitEventDataBase();
        void ctor();
        CPlayerUnitEventDataBase_vtable* vtable;
        // CObserver
        uint32_t CObserver1[0x2];
        // CAgent
        objectid_64 unk1;
        uint32_t CObserver2_vtable;
        uint32_t CObserver2[0x2];
        // CPlayerEventDataBase
        objectid_64 player;
        uint32_t unk2;
        // CPlayerUnitEventDataBase
        objectid_64 triggerUnit;
        uint32_t unk3;
        objectid_64 filterUnit;
        uint32_t unk4;
    };
    #define defineEventData(name, id, filterunit, code)                                                                         \
        TriggerEvent::CPlayerUnitEventDataBase_vtable* name##_vtable;                                                           \
        class name : public TriggerEvent::CPlayerUnitEventDataBase {                                                            \
        public:                                                                                                                 \
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
            static void ctor(uint32_t _this) {                                                                                  \
                ((CPlayerUnitEventDataBase*)_this)->ctor();                                                                     \
                ((name*)_this)->vtable = name##_vtable;                                                                         \
            }                                                                                                                   \
        };
#define setupEventData(name, eventid, ...) do {                                                                                 \
        name##_vtable = TriggerEvent::CPlayerUnitEventDataBase_vtable_instance.copy();                                          \
        WriteMemory((uint32_t)&name##_vtable->GetTypeID, &name::GetTypeID);                                                     \
        WriteMemory((uint32_t)&name##_vtable->GetTypeName, &name::GetTypeName);                                                 \
        WriteMemory((uint32_t)&name##_vtable->GetFilterUnit, &name::GetFilterUnit);                                             \
        TriggerEvent::RegisterTriggerEventData(eventid, TriggerEvent::Type::CPlayerUnitEventDataBase, { __VA_ARGS__}, sizeof(name), 0x10, name::ctor); } while(0)
#define setupEventData_s(name, eventid) setupEventData(name, eventid, eventid)
    void RegisterTriggerEventData(uint32_t typeID, Type parentTypeID, std::vector<uint32_t> eventID, uint32_t size, uint32_t batchAllocCount, void(*ctor)(uint32_t));
    uint32_t TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t boolexpr);
    uint32_t GetTriggerEventId();
    uint32_t GetTriggerEventData(uint32_t typeID = 'wscd');
    bool IsEventRegistered(uint32_t pObserver, uint32_t eventID);
    uint32_t GetUnitOwner(uint32_t pUnit);
    void FirePlayerUnitEvent(CPlayerUnitEventDataBase* pPlayerUnitEvent, uint32_t pUnit, uint32_t pUnit2, uint32_t eventID);

}