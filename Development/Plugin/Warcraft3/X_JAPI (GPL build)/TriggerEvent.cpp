#include <map>
#include <string>
#include <stack>

#include <base/hook/fp_call.h>
#include <base/hook/iat.h>
#include <base/hook/inline.h>
#include <base/hook/replace_pointer.h>
#include <base/util/memory.h>

#include <warcraft3/hashtable.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>

#include "TriggerEvent.h"
#include "util.h"

#define TRIGGER_EVENT_ID_BASE 0x80200
#define EVENT_PLAYER_UNIT_ATTACKED (TRIGGER_EVENT_ID_BASE + 0x18)

enum class DataType : uint32_t {
    triggerunit,
    triggerwidget,
    triggeringtrigger,
    triggerplayer,
    eventreg,
    eventdata,
    eventid,
    filterunit,
    filterplayer,
    filterdestructable,
    enumunit,
    enumplayer,
    enumdestructable,
    enumitem,
    filteritem,
};

namespace TriggerEvent {

    uint32_t searchCDataAllocator_Create() {
        uint32_t ptr = ReadMemory(get_vfn_ptr(".?AV?$InstanceGenerator@VCEffectImagePos@@@@") + 0x0);
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);
        if (get_war3_searcher().get_version() >= VERSION::version_127a)
            return ptr;
        ptr = next_opcode(ptr, 0xE8, 5);
        return convert_function(ptr);
    }
    uint32_t searchCDataAllocator_Free() {
        uint32_t ptr = ReadMemory(get_vfn_ptr(".?AV?$InstanceGenerator@VCEffectImagePos@@@@") + 0x4);
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        return convert_function(ptr);
    }
    uint32_t searchCDataAllocator_FreeAll() {
        uint32_t ptr = ReadMemory(get_vfn_ptr(".?AV?$InstanceGenerator@VCEffectImagePos@@@@") + 0x8);
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        return convert_function(ptr);
    }
    uint32_t searchRegisterType() {
        war3_searcher& s = get_war3_searcher();

        for (uint32_t ptr = s.search_int_in_text('+w3a'); ptr; ptr = s.search_int_in_text('+w3a', ptr + 1)) {
            ptr += 4;
            if (ReadMemory<uint8_t>(ptr) == 0xE8) // call
                return convert_function(ptr);
        }
        return 0;
    }
    uint32_t searchCPlayerEventReg_SetupEvent() {
        war3_searcher& s = get_war3_searcher();
        uint32_t ptr = s.search_string("TriggerRegisterPlayerUnitEvent");
        ptr = ReadMemory(ptr + 5);

        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        if (s.get_version() < VERSION::version_127a) {
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
        }
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);
        return ptr;
    }
    uint32_t searchCTriggerWar3_AddEvent() {
        war3_searcher& s = get_war3_searcher();
        uint32_t ptr = s.search_string("TriggerRegisterPlayerUnitEvent");
        ptr = ReadMemory(ptr + 5);

        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        if (s.get_version() < VERSION::version_127a) {
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
        }
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);
        return ptr;
    }
    uint32_t searchCGameState_PeekDataByType() {
        uint32_t ptr = get_war3_searcher().search_string("GetTriggerEventId");
        ptr = ReadMemory(ptr + 5);

        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);
        return ptr;
    }

    uint32_t searchCPlayerEventReg_ProcessEvent() {
        return ReadMemory(get_vfn_ptr(".?AVCPlayerEventReg@@") + 0xC);
    }
    uint32_t searchCPlayerEventReg_FireEvent_ptr() {
        return get_vfn_ptr(".?AVCPlayerEventReg@@") + 0x10;
    }
    uint32_t searchCPlayerWar3_Save() {
        uint32_t ptr = get_vfn_ptr(".?AVCPlayerWar3@@");
        return ReadMemory(ptr + 0x38); // vftable + 0x38
    }
    uint32_t searchCPlayerWar3_Load() {
        uint32_t ptr = get_vfn_ptr(".?AVCPlayerWar3@@");
        return ReadMemory(ptr + 0x3C); // vftable + 0x3C
    }
    uint32_t searchCUnit_GetOwner() {
        uint32_t ptr;

        //=========================================
        // (1)
        //
        // push     "()V"
        // mov      edx, "GetOwningPlayer"
        // mov      ecx, [GetOwningPlayer函数的地址] <----
        // call     BindNative
        //=========================================
        ptr = get_war3_searcher().search_string("GetOwningPlayer");
        ptr = *(uint32_t*)(ptr + 0x05);

        //=========================================
        // (2)
        //  GetOwningPlayer:
        //      ...
        //      call ConvertHandle
        //      ...
        //      call CUnit_GetOwner
        //=========================================
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr = convert_function(ptr);
        return ptr;
    }
    struct CDataAllocator {
        uint32_t PreAllocInstanceSize;
        uint32_t PreAllocInstanceCount;
        uint32_t unk1;
        uint32_t unk2;
        uint32_t unk3;
        CDataAllocator(uint32_t size, uint32_t batchAllocCount) : unk1(0), PreAllocInstanceCount(batchAllocCount), PreAllocInstanceSize(size), unk2(0), unk3(0) {};
    };

    uint32_t CDataAllocator_Create(CDataAllocator* _this, bool fillzero) {
        static auto pCDataAllocator_Create = searchCDataAllocator_Create();
        return base::this_call<uint32_t>(pCDataAllocator_Create, _this, fillzero, __FILE__, __LINE__);
    }
    void CDataAllocator_Free(CDataAllocator* _this, uint32_t pObj) {
        static auto pCDataAllocator_Free = searchCDataAllocator_Free();
        base::this_call<void>(pCDataAllocator_Free, _this, pObj, __FILE__, __LINE__);
    }
    void CDataAllocator_FreeAll(CDataAllocator* _this) {
        static auto pCDataAllocator_FreeAll = searchCDataAllocator_FreeAll();
        base::this_call<void>(pCDataAllocator_FreeAll, _this, __FILE__, __LINE__, 1);
    }
    uint32_t searchCObserver_IsEventRegistered() {
        war3_searcher& s = get_war3_searcher();
        for (uint32_t ptr = s.search_int_in_text(524818 /* 事件ID */); ptr; ptr = s.search_int_in_text(524818, ptr + 1))
            if (ReadMemory<uint8_t>(ptr - 1) == 0x68) // push 0x80212
                return convert_function(next_opcode(ptr, 0xE8, 5));
        return 0;
    }
    uint32_t searchDispatchPlayerUnitEvent() {
        war3_searcher& s = get_war3_searcher();
        for (uint32_t ptr = s.search_int_in_text(524818 /* 事件ID */); ptr; ptr = s.search_int_in_text(524818, ptr + 1)) {
            if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
                ptr = s.search_int_in_text(524818, ptr + 1);
                if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
                    return convert_function(next_opcode(ptr, 0xE8, 5));
                }
            }
        }
        return 0;
    }

    class InstanceGenerator {
    public:
        virtual uint32_t alloc() {
            uint32_t pObj = CDataAllocator_Create(&allocator, true);
            if (pObj)
                ctor(pObj);
            return pObj;
        }
        virtual void free(uint32_t pObj) {
            base::this_call_vf<void>(pObj, 4, 0);
            return CDataAllocator_Free(&allocator, pObj);
        }
        virtual void clear() {
            return CDataAllocator_FreeAll(&allocator);
        }
        InstanceGenerator(uint32_t size, uint32_t batchAllocCount, uint32_t typeID, Type parentTypeID, std::vector<uint32_t> eventID, void(*ctor)(uint32_t)) : allocator(size, batchAllocCount), typeID(typeID), parentTypeID(parentTypeID), eventID(eventID), ctor(ctor) {};
        CDataAllocator allocator;
        // extended
        void(*ctor)(uint32_t);
        uint32_t typeID;
        Type parentTypeID;
        std::vector<uint32_t> eventID;
    };
    CPlayerUnitEventDataBase_vtable CPlayerUnitEventDataBase_vtable_instance;
    std::map<uint32_t, InstanceGenerator*> Generator;
    std::map<uint32_t, bool> PlayerUnitEventIds;

    // 如果是 JAPI 的玩家单位事件则临时修改事件ID
    std::stack<uint32_t> real_event_id;
    uint32_t real_CPlayerEventReg_ProcessEvent;
    bool __fastcall fake_CPlayerEventReg_ProcessEvent(uint32_t _this, uint32_t, uint32_t pEvent) {
        uint32_t eventid = ReadMemory(pEvent + 0x8);
        real_event_id.push(eventid);
        if (PlayerUnitEventIds.find(eventid) != PlayerUnitEventIds.end()) {
            WriteMemory(_this + 0x34, EVENT_PLAYER_UNIT_ATTACKED);
            WriteMemory(pEvent + 0x8, EVENT_PLAYER_UNIT_ATTACKED);
        }
        bool ret = base::this_call<bool>(real_CPlayerEventReg_ProcessEvent, _this, pEvent);
        WriteMemory(_this + 0x34, eventid);
        WriteMemory(pEvent + 0x8, eventid);
        real_event_id.pop();
        return ret;
    }

    // 把事件ID修改回去
    uint32_t real_CPlayerEventReg_FireEvent;
    void __fastcall fake_CPlayerEventReg_FireEvent(uint32_t _this, uint32_t, uint32_t pEvent) {
        if (!real_event_id.empty()) {
            uint32_t& eventid = real_event_id.top();
            WriteMemory(_this + 0x34, eventid);
            WriteMemory(pEvent + 0x8, eventid);
        }
        base::this_call<void>(real_CPlayerEventReg_FireEvent, _this, pEvent);
    }

    uint32_t real_CPlayerWar3_Save;
    void __fastcall fake_CPlayerWar3_Save(uint32_t _this, uint32_t, uint32_t pSaveGame) {
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            if (i->second->parentTypeID == Type::CPlayerUnitEventDataBase)
                for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                    base::this_call_vf<void>(_this, 0x50, pSaveGame, TRIGGER_EVENT_ID_BASE + *i1, 1024);
        base::this_call<void>(real_CPlayerWar3_Save, _this, pSaveGame);
    }

    uint32_t real_CPlayerWar3_Load;
    void __fastcall fake_CPlayerWar3_Load(uint32_t _this, uint32_t, uint32_t pSaveGame) {
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            if (i->second->parentTypeID == Type::CPlayerUnitEventDataBase)
                for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                    base::this_call_vf<void>(_this, 0x54, pSaveGame, TRIGGER_EVENT_ID_BASE + *i1, 1024);
        base::this_call<void>(real_CPlayerWar3_Load, _this, pSaveGame);
    }

    uint32_t real_RegisterType;
    void __fastcall fake_RegisterType(Type typeID, Type parentTypeID, uint32_t generator) {
         base::fast_call<void>(real_RegisterType, typeID, parentTypeID, generator);
         if (typeID == Type::CPlayerUnitEventDataBase) {
             for (auto i = Generator.begin(); i != Generator.end(); i++)
                 if (i->second->parentTypeID == typeID)
                     base::fast_call<void>(real_RegisterType, i->second->typeID, typeID, i->second);
         }
    }

    CPlayerUnitEventDataBase::CPlayerUnitEventDataBase() : unk1(0xFFFFFFFF, 0xFFFFFFFF), player(0xFFFFFFFF, 0xFFFFFFFF), triggerUnit(0xFFFFFFFF, 0xFFFFFFFF), filterUnit(0xFFFFFFFF, 0xFFFFFFFF) {
        ctor();
    }
    void CPlayerUnitEventDataBase::ctor() {
        static uint32_t pCObserver_vtable = get_vfn_ptr(".?AVCObserver@@");
        // CObserver
        vtable = (CPlayerUnitEventDataBase_vtable*)pCObserver_vtable;
        CObserver1[0] = 0; // 引用计数
        CObserver1[1] = 0;
        // CAgent
        unk1 = objectid_64(0xFFFFFFFF, 0xFFFFFFFF);
        CObserver2_vtable = pCObserver_vtable;
        CObserver2[0] = 1; // 引用计数
        CObserver2[1] = 0;
        // CPlayerEventDataBase
        player = objectid_64(0xFFFFFFFF, 0xFFFFFFFF);
        unk2 = 0;
        // CPlayerUnitEventDataBase
        triggerUnit = objectid_64(0xFFFFFFFF, 0xFFFFFFFF);
        unk3 = 0;
        filterUnit = objectid_64(0xFFFFFFFF, 0xFFFFFFFF);
        unk4 = 0;
    }
    // 注册触发数据
    // 0x81000 - 0x81FFF 似乎安全
    void RegisterTriggerEventData(uint32_t typeID, Type parentTypeID, std::vector<uint32_t> eventID, uint32_t size, uint32_t batchAllocCount, void(*ctor)(uint32_t)) {
        if (Generator.find(typeID) == Generator.end()) {
            if (Generator.size() == 0) {
                real_CPlayerEventReg_ProcessEvent = searchCPlayerEventReg_ProcessEvent();
                base::hook::install(&real_CPlayerEventReg_ProcessEvent, (uint32_t)fake_CPlayerEventReg_ProcessEvent); // 问就是懒
                real_CPlayerEventReg_FireEvent = ReadMemory(searchCPlayerEventReg_FireEvent_ptr());
                base::hook::replace_pointer(searchCPlayerEventReg_FireEvent_ptr(), (uint32_t)fake_CPlayerEventReg_FireEvent);
                real_CPlayerWar3_Save = searchCPlayerWar3_Save();
                base::hook::install(&real_CPlayerWar3_Save, (uint32_t)fake_CPlayerWar3_Save);
                real_CPlayerWar3_Load = searchCPlayerWar3_Load();
                base::hook::install(&real_CPlayerWar3_Load, (uint32_t)fake_CPlayerWar3_Load);
                real_RegisterType = searchRegisterType();
                base::hook::install(&real_RegisterType, (uint32_t)fake_RegisterType);
            }
            InstanceGenerator* generator = new InstanceGenerator(size, batchAllocCount, typeID, parentTypeID, eventID, ctor);
            Generator[typeID] = generator;
        }
    }
    
    // 注册玩家单位事件
    uint32_t TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t boolexpr) {
        uint32_t pTriggerWar3 = handle_to_object(trigger);
        uint32_t pPlayerWar3 = handle_to_object(player);
        uint32_t pBooleanExpr = handle_to_object(boolexpr);
        if (!pTriggerWar3 || !pPlayerWar3 || !type_check_s(pTriggerWar3, '+trg') || !type_check_s(pPlayerWar3, '+ply'))
            return 0;
        if (pBooleanExpr && !type_check_s(pBooleanExpr, 'bExp'))
            pBooleanExpr = NULL;
        PlayerUnitEventIds[eventID + TRIGGER_EVENT_ID_BASE] = true;
        uint32_t pPlayerEventReg = create_by_typeid('pevt');
        static uint32_t pPlayerEventReg_SetupEvent = searchCPlayerEventReg_SetupEvent();
        base::this_call<void>(pPlayerEventReg_SetupEvent, pPlayerEventReg, pTriggerWar3, pPlayerWar3, TRIGGER_EVENT_ID_BASE + eventID, pBooleanExpr);
        static uint32_t pTriggerWar3_AddEvent = searchCTriggerWar3_AddEvent();
        base::this_call<void>(pTriggerWar3_AddEvent, pTriggerWar3, pPlayerEventReg);
        uint32_t event = create_handle(pPlayerEventReg);
        reference_free_ptr((uint32_t**)&pPlayerEventReg);
        return event;
    }

    // 获取当前触发的事件ID
    uint32_t GetTriggerEventId() {
        static auto& s = get_war3_searcher();
        uint32_t pGameState = base::this_call<uint32_t>(s.create_handle.GetDataNode, s.get_gamewar3());
        if (!pGameState)
            return 0;
        static uint32_t pGameState_PeekDataByType = searchCGameState_PeekDataByType();
        uint32_t eventid = base::this_call<uint32_t>(pGameState_PeekDataByType, pGameState, DataType::eventid);
        if (!eventid)
            return 0;
        return eventid;
    }

    // 获取当前触发的事件数据
    uint32_t GetTriggerEventData(uint32_t typeID) {
        static auto& s = get_war3_searcher();
        uint32_t pGameState = base::this_call<uint32_t>(s.create_handle.GetDataNode, s.get_gamewar3());
        if (!pGameState)
            return 0;
        static uint32_t pGameState_PeekDataByType = searchCGameState_PeekDataByType();
        uint32_t eventreg = base::this_call<uint32_t>(pGameState_PeekDataByType, pGameState, DataType::eventreg);
        if (!eventreg)
            return 0;
        uint32_t eventdata = base::this_call<uint32_t>(pGameState_PeekDataByType, pGameState, DataType::eventdata);
        if (!eventdata)
            return 0;
        uint32_t pEventData = handle_to_object(eventdata);
        if (!pEventData || !type_check_s(pEventData, typeID))
            return 0;
        return pEventData;
    }

    // 检查事件ID是否注册
    bool IsEventRegistered(uint32_t pObserver, uint32_t eventID) {
        static uint32_t pCObserver_IsEventRegistered = searchCObserver_IsEventRegistered();
        return base::this_call<bool>(pCObserver_IsEventRegistered, pObserver, TRIGGER_EVENT_ID_BASE + eventID);
    }

    uint32_t GetUnitOwner(uint32_t pUnit) {
        static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
        return base::this_call<uint32_t>(CUnit_GetOwner, pUnit);
    }
    // 运行事件
    void FirePlayerUnitEvent(CPlayerUnitEventDataBase* pPlayerUnitEvent, uint32_t pUnit, uint32_t pUnit2, uint32_t eventID) {
        uint32_t pOwningPlayer = GetUnitOwner(pUnit);

        uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0xC), ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitEvent->player = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pUnit + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitEvent->triggerUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        if (pUnit2) {
            pAgent = find_objectid_64({ ReadMemory<uint32_t>(pUnit2 + 0xC), ReadMemory<uint32_t>(pUnit2 + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                pPlayerUnitEvent->filterUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        static uint32_t pDispatchPlayerUnitEvent = searchDispatchPlayerUnitEvent();
        base::this_call<uint32_t>(pDispatchPlayerUnitEvent, pOwningPlayer, TRIGGER_EVENT_ID_BASE + eventID, pPlayerUnitEvent);
        reference_free_ptr((uint32_t**)&pPlayerUnitEvent);
    }

    uint32_t __cdecl X_TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t filter) {
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            if (i->second->parentTypeID == Type::CPlayerUnitEventDataBase)
                for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                    return TriggerRegisterPlayerUnitEvent(trigger, player, eventID, filter);
        return 0;
    }
    
    uint32_t __cdecl X_TriggerRegisterPlayerUnitEventSimple(uint32_t trigger, uint32_t player, uint32_t eventID) {
        return X_TriggerRegisterPlayerUnitEvent(trigger, player, eventID, NULL);
    }

    uint32_t __cdecl X_GetTriggerUnit() {
        static auto& s = get_war3_searcher();
        uint32_t pGameState = base::this_call<uint32_t>(s.create_handle.GetDataNode, s.get_gamewar3());
        if (!pGameState)
            return 0;
        static uint32_t pGameState_PeekDataByType = searchCGameState_PeekDataByType();
        uint32_t triggerunit = base::this_call<uint32_t>(pGameState_PeekDataByType, pGameState, DataType::triggerunit);
        if (!triggerunit)
            return 0;
        return triggerunit;
    }

    uint32_t __cdecl X_GetTriggerPlayer() {
        static auto& s = get_war3_searcher();
        uint32_t pGameState = base::this_call<uint32_t>(s.create_handle.GetDataNode, s.get_gamewar3());
        if (!pGameState)
            return 0;
        static uint32_t pGameState_PeekDataByType = searchCGameState_PeekDataByType();
        uint32_t triggerunit = base::this_call<uint32_t>(pGameState_PeekDataByType, pGameState, DataType::triggerplayer);
        if (!triggerunit)
            return 0;
        return triggerunit;
    }

    init(TriggerEvent) {
        jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitEvent,          "X_TriggerRegisterPlayerUnitEvent",         "(Htrigger;Hplayer;IHboolexpr;)Hevent;");
        jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitEventSimple,    "X_TriggerRegisterPlayerUnitEventSimple",   "(Htrigger;Hplayer;I)Hevent;");
        jass::japi_add((uint32_t)X_GetTriggerUnit,                          "X_GetTriggerUnit",                         "()Hunit;");
        jass::japi_add((uint32_t)X_GetTriggerPlayer,                        "X_GetTriggerPlayer",                       "()Hplayer;");
    }
}