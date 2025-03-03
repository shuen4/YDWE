#include <map>

#include <base/hook/inline.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>

#include "TriggerEvent.h"
#include "util.h"

#define TRIGGER_EVENT_ID_BASE 0x80200

namespace TriggerEvent {
    class X_InstanceGenerator : public war3::InstanceGenerator {
    public:
        void(*ctor)(uint32_t);
        void(*dtor)(uint32_t);
        uint32_t typeID;
        uint32_t parentTypeID;
        std::vector<uint32_t> eventID;

        X_InstanceGenerator(uint32_t size, uint32_t batchAllocCount, uint32_t typeID, uint32_t parentTypeID, std::vector<uint32_t> eventID, void(*ctor)(uint32_t), void(*dtor)(uint32_t)) : typeID(typeID), parentTypeID(parentTypeID), eventID(eventID), ctor(ctor), dtor(dtor) {
            PreAllocInstanceCount = batchAllocCount;
            PreAllocInstanceSize = size;
        };
        virtual uint32_t CreateObj() {
            uint32_t obj = __super::CreateObj();
            if (obj)
                ctor(obj);
            return obj;
        };
        virtual void FreeObj(uint32_t obj) {
            if (dtor)
                dtor(obj);
            base::this_call_vf<void>(obj, 4, 0);
            return __super::FreeObj(obj);
        };
    };

    CPlayerUnitEventDataBase_vtable CPlayerUnitEventDataBase_vtable_instance;
    std::map<uint32_t, X_InstanceGenerator*> Generator;

    void CPlayerUnitEventDataBase_ctor(CPlayerUnitEventDataBase* _this) {
        static uint32_t pCObserver_vtable = VFN::CPlayerUnitEventDataBase;
        // CObserver
        _this->__vfn = VFN::CPlayerUnitEventDataBase;
        _this->reference_count = 0;
        _this->unk_8 = 0;
        // CAgent
        _this->AgentBaseAbsTag.tagA = 0xFFFFFFFF;
        _this->AgentBaseAbsTag.tagB = 0xFFFFFFFF;
        _this->unk_20.__vfn = VFN::CPlayerUnitEventDataBase;
        _this->unk_20.reference_count = 0;
        _this->unk_20.unk_8 = 0;
        // CPlayerEventDataBase
        _this->player.tagA = 0xFFFFFFFF;
        _this->player.tagB = 0xFFFFFFFF;
        _this->player.unk_8 = 0;
        // CPlayerUnitEventDataBase
        _this->unit.tagA = 0xFFFFFFFF;
        _this->unit.tagB = 0xFFFFFFFF;
        _this->unit.unk_8 = 0;
        _this->agent.tagA = 0xFFFFFFFF;
        _this->agent.tagB = 0xFFFFFFFF;
        _this->agent.unk_8 = 0;
    }

    uint32_t(__thiscall* real_CPlayerEventReg_ProcessEvent)(CPlayerEventReg* pPlayerEventReg, CScriptEvent* pEvent);
    uint32_t __fastcall fake_CPlayerEventReg_ProcessEvent(CPlayerEventReg* _this, uint32_t, CScriptEvent* pScriptEvent) {
        if (Agile::IsChild(pScriptEvent->data, X_CPlayerUnitEventDataBase::typeID)) {
            CUnit* pFilterUnit = ((CPlayerUnitEventDataBase_vtable*)pScriptEvent->data->__vfn)->GetFilterUnit();
            if (pFilterUnit) {
                CGameWar3::GetInstance()->GetGameState()->Push(CGameState::DataType::filterunit, pFilterUnit, 0);
                bool do_action = _this->Evaluate();
                CGameWar3::GetInstance()->GetGameState()->Pop(CGameState::DataType::filterunit, 1);

                if (!do_action)
                    return false;
            }
            
            auto test = (X_CPlayerUnitEventDataBase*)pScriptEvent->data;

            CGameWar3::GetInstance()->GetGameState()->Push(CGameState::DataType::triggerunit, UTIL::GetAgentByTag<CUnit>(((X_CPlayerUnitEventDataBase*)pScriptEvent->data)->unit), 0);
            bool ret = real_CPlayerEventReg_ProcessEvent(_this, pScriptEvent);
            CGameWar3::GetInstance()->GetGameState()->Pop(CGameState::DataType::triggerunit, 1);

            return ret;
        }
        return real_CPlayerEventReg_ProcessEvent(_this, pScriptEvent);
    }

    void(__thiscall* real_CPlayerWar3_Save)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
    void __fastcall fake_CPlayerWar3_Save(CPlayerWar3* pPlayerWar3, uint32_t, CSaveGame* pSaveGame) {
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            if (Agile::IsChild(i->second->typeID, X_CPlayerUnitEventDataBase::typeID))
                for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                    pPlayerWar3->SaveEvent(pSaveGame, *i1, 1024);
        real_CPlayerWar3_Save(pPlayerWar3, pSaveGame);
    }

    void(__thiscall* real_CPlayerWar3_Load)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
    void __fastcall fake_CPlayerWar3_Load(CPlayerWar3* pPlayerWar3, uint32_t, CSaveGame* pSaveGame) {
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            if (Agile::IsChild(i->second->typeID, X_CPlayerUnitEventDataBase::typeID))
                for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                    pPlayerWar3->LoadEvent(pSaveGame, *i1, 1024);
        real_CPlayerWar3_Load(pPlayerWar3, pSaveGame);
    }

    void(__fastcall* real_RegisterType)(uint32_t ID, uint32_t parentID, InstanceGenerator* pInstanceGenerator);
    void __fastcall fake_RegisterType(uint32_t ID, uint32_t parentTypeID, InstanceGenerator* generator) {
         real_RegisterType(ID, parentTypeID, generator);

         for (auto i = Generator.begin(); i != Generator.end(); i++)
             if (i->second->parentTypeID == ID)
                 fake_RegisterType(i->second->typeID, ID, i->second);
    }

    // 注册触发数据
    void RegisterTriggerEventData(uint32_t typeID, uint32_t parentTypeID, std::vector<uint32_t> eventID, uint32_t size, uint32_t batchAllocCount, void(*ctor)(uint32_t), void(*dtor)(uint32_t)) {
        if (Generator.find(typeID) == Generator.end()) {
            if (Generator.size() == 0) { // 初始化
                real_CPlayerEventReg_ProcessEvent = FUNC::CPlayerEventReg_ProcessEvent;
                base::hook::install((uint32_t*)&real_CPlayerEventReg_ProcessEvent, (uint32_t)fake_CPlayerEventReg_ProcessEvent); // 问就是懒
                real_CPlayerWar3_Save = FUNC::CPlayerWar3_Save;
                base::hook::install((uint32_t*)&real_CPlayerWar3_Save, (uint32_t)fake_CPlayerWar3_Save);
                real_CPlayerWar3_Load = FUNC::CPlayerWar3_Load;
                base::hook::install((uint32_t*)&real_CPlayerWar3_Load, (uint32_t)fake_CPlayerWar3_Load);
                real_RegisterType = FUNC::RegisterType;
                base::hook::install((uint32_t*)&real_RegisterType, (uint32_t)fake_RegisterType);
            }
            X_InstanceGenerator* generator = new X_InstanceGenerator(size, batchAllocCount, typeID, parentTypeID, eventID, ctor, dtor);
            Generator[typeID] = generator;
        }
    }
    
    // 注册玩家单位事件
    uint32_t TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t boolexpr) {
        CTriggerWar3* pTriggerWar3 = ConvertHandle<CTriggerWar3>(trigger);
        CPlayerWar3* pPlayerWar3 = ConvertHandle<CPlayerWar3>(player);
        CBooleanExpr* pBooleanExpr = ConvertHandle<CBooleanExpr>(boolexpr);

        if (!pTriggerWar3 || !pPlayerWar3)
            return NULL;

        CPlayerEventReg* pPlayerEventReg = (CPlayerEventReg*)Agile::CreateObject('pevt', false);
        pPlayerEventReg->Initialize(pTriggerWar3, pPlayerWar3, eventID, pBooleanExpr);
        pTriggerWar3->AddEvent(pPlayerEventReg);
        uint32_t hEvent = CGameWar3::GetInstance()->GetGameState()->ToHandle(pPlayerEventReg, 0);
        pPlayerEventReg->release();
        return hEvent;
    }

    // 获取当前触发的事件ID
    uint32_t GetTriggerEventId() {
        return (uint32_t)CGameWar3::GetInstance()->GetGameState()->Peek(CGameState::DataType::eventid) + TRIGGER_EVENT_ID_BASE;
    }

    // 获取当前触发的事件数据
    CScriptEventData* GetTriggerEventData(uint32_t typeID) {
        CGameState* pGameState = CGameWar3::GetInstance()->GetGameState();
        if (!pGameState->Peek(CGameState::DataType::eventreg))
            return NULL;
        
        uint32_t eventdata = (uint32_t)pGameState->Peek(CGameState::DataType::eventdata);
        if (!eventdata)
            return NULL;

        CScriptEventData* pScriptEventData = ConvertHandle<CScriptEventData>(eventdata);
        if (!pScriptEventData || !Agile::IsChild(pScriptEventData, typeID))
            return NULL;

        return pScriptEventData;
    }
    // 运行事件
    void FirePlayerUnitEvent(CPlayerUnitEventDataBase* pPlayerUnitEvent, CUnit* pUnit, CAgent* pAgent, uint32_t eventID) {
        CPlayerWar3* pPlayerWar3 = pUnit->GetOwner();

        pPlayerUnitEvent->player = UTIL::GetAgentTag(pPlayerWar3);
        pPlayerUnitEvent->unit = UTIL::GetAgentTag(pUnit);
        if (pAgent)
            pPlayerUnitEvent->agent = UTIL::GetAgentTag(pAgent);

        pPlayerWar3->DispatchScriptEvent(eventID, pPlayerUnitEvent);
        pPlayerUnitEvent->release();
    }

    uint32_t __cdecl X_TriggerRegisterPlayerUnitEvent(uint32_t trigger, uint32_t player, uint32_t eventID, uint32_t filter) {
        eventID += TRIGGER_EVENT_ID_BASE;
        for (auto i = Generator.begin(); i != Generator.end(); i++)
            for (auto i1 = i->second->eventID.begin(); i1 != i->second->eventID.end(); i1++)
                if (*i1 == eventID && Agile::IsChild(i->second->typeID, X_CPlayerUnitEventDataBase::typeID))
                    return TriggerRegisterPlayerUnitEvent(trigger, player, eventID, filter);
        return 0;
    }
    
    uint32_t __cdecl X_TriggerRegisterPlayerUnitEventSimple(uint32_t trigger, uint32_t player, uint32_t eventID) {
        return X_TriggerRegisterPlayerUnitEvent(trigger, player, eventID, NULL);
    }

    uint32_t __cdecl X_GetTriggerUnit() {
        return (uint32_t)CGameWar3::GetInstance()->GetGameState()->Peek(CGameState::DataType::triggerunit);
    }

    uint32_t __cdecl X_GetTriggerPlayer() {
        return (uint32_t)CGameWar3::GetInstance()->GetGameState()->Peek(CGameState::DataType::triggerplayer);
    }

    CPlayerUnitEventDataBase_vtable* X_CPlayerUnitEventDataBase_vtable;
    uint32_t X_CPlayerUnitEventDataBase::GetTypeID() {
        return X_CPlayerUnitEventDataBase::typeID;
    }
    const char* X_CPlayerUnitEventDataBase::GetTypeName() {
        return "X_CPlayerUnitEventDataBase";
    }
    void X_CPlayerUnitEventDataBase::ctor(X_CPlayerUnitEventDataBase* _this) {
        CPlayerUnitEventDataBase_ctor(_this);
        ((X_CPlayerUnitEventDataBase*)_this)->__vfn = (uint32_t)X_CPlayerUnitEventDataBase_vtable;
    }

    init_M(TriggerEvent) {
        CPlayerUnitEventDataBase_vtable_instance.real_init();
        X_CPlayerUnitEventDataBase_vtable = CPlayerUnitEventDataBase_vtable_instance.copy();
        WriteMemory((uint32_t)&X_CPlayerUnitEventDataBase_vtable->GetTypeID, &X_CPlayerUnitEventDataBase::GetTypeID);
        WriteMemory((uint32_t)&X_CPlayerUnitEventDataBase_vtable->GetTypeName, &X_CPlayerUnitEventDataBase::GetTypeName);
        RegisterTriggerEventData(X_CPlayerUnitEventDataBase::typeID, CPlayerUnitEventDataBase::typeID, {}, sizeof(X_CPlayerUnitEventDataBase), 0x1, (void(*)(uint32_t))X_CPlayerUnitEventDataBase::ctor);
    }

    init_L(TriggerEvent) {
        jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitEvent,          "X_TriggerRegisterPlayerUnitEvent",         "(Htrigger;Hplayer;IHboolexpr;)Hevent;");
        jass::japi_add((uint32_t)X_TriggerRegisterPlayerUnitEventSimple,    "X_TriggerRegisterPlayerUnitEventSimple",   "(Htrigger;Hplayer;I)Hevent;");
        jass::japi_add((uint32_t)X_GetTriggerUnit,                          "X_GetTriggerUnit",                         "()Hunit;");
        jass::japi_add((uint32_t)X_GetTriggerPlayer,                        "X_GetTriggerPlayer",                       "()Hplayer;");
    }
}