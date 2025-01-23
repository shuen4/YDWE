#pragma once

namespace warcraft3::japi::TriggerEvent {
    enum class Type : uint32_t {
        CPlayerUnitEventDataBase = 'pued',
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
    void RegisterTriggerEventData(uint32_t typeID, Type parentTypeID, std::vector<uint32_t> eventID, uint32_t size, uint32_t batchAllocCount, void(*ctor)(uint32_t));
    uint32_t TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t boolexpr);
    uint32_t GetTriggerEventId();
    uint32_t GetTriggerEventData(uint32_t typeID = 'wscd');
    bool IsEventRegistered(uint32_t pObserver, uint32_t eventID);
    void FirePlayerUnitEvent(CPlayerUnitEventDataBase* pPlayerUnitEvent, uint32_t pPlayerWar3, uint32_t eventID);

}