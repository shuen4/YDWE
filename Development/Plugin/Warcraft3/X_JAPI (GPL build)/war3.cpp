#include <warcraft3/war3_searcher.h>

#include "util.h"
#include "war3.h"

namespace war3 {
    VERSION version;

    void reference_free_ptr(uint32_t** _this, uint32_t offset) {
        // 已有数值
        if (_this[0]) {
            // 计数 - 1
            --_this[0][1];
            if (!_this[0][1])
                // 计数 0 则调用解构函数
                base::this_call<void>(ReadMemory<uint32_t>(_this[0][offset]), _this[0]);
            // 清空
            _this[0] = 0;
        }
    }
    void TRefCnt::release() {
        TRefCnt* pTRefCnt = this;
        reference_free_ptr((uint32_t**)&pTRefCnt, 0);
    }
    void CLight::release() {
        CLight* pLight = this;
        reference_free_ptr((uint32_t**)&pLight, 0);
    }
    void CUnit::AddBuff(CBuff* pBuff) {
        AddAbility(pBuff);
        pBuff->UpdateUI();
    }
    void CLight::Setup1(uint32_t index, float* value) {
        int unk = ReadMemory(ReadMemory((uint32_t)this + 16) + 4 * index);
        WriteMemory(unk + 0x10, 0);
        WriteMemory(unk + 0x14, 0);
        WriteMemory(unk + 0x18, 0);
        ((void(__thiscall*)(float*))(ReadMemory(ReadMemory(unk) + 0xC)))(value);
    }
    void CLight::Setup2(uint32_t index, float* value) {
        int unk = ReadMemory(ReadMemory((uint32_t)this + 16) + 4 * index);
        WriteMemory(unk + 0x10, 0);
        WriteMemory(unk + 0x14, 0);
        WriteMemory(unk + 0x18, 0);
        ((void(__thiscall*)(float*))(ReadMemory(ReadMemory(unk) + 0xC)))(value);
    }
    void CLight::Setup3(uint32_t index, float* value, uint32_t a3) {
        return FUNC::Light_Setup3(this, index, value, a3);
    }
    void CAbilitySpell::StartCD(float* cooldown) {
        if (agent_flag & agent_flag_in_cooldown)
            cooldown_timer.Stop();
        else
            agent_flag |= agent_flag_in_cooldown;

        cooldown_timer.Start(cooldown, eventID_cooldown, this, 0, 0);
        current_cooldown_duration.value = *cooldown;

        UpdateUI();
    }
    void CAbilitySpell::StopCD() {
        if (agent_flag & agent_flag_in_cooldown) {
            cooldown_timer.Stop();
            agent_flag &= ~agent_flag_in_cooldown;
            UpdateUI();
        }
    }
    bool CPlayerEventReg::Evaluate() {
        if (!filter_func)
            return true;
        else
            return filter_func->Evaluate();
    }
    CAgent* Agile::CreateObject(uint32_t typeID, uint32_t async) {
        AGILE_TYPE_DATA* pAgileTypeData;
        if (version >= version_127a)
            pAgileTypeData = FUNC::Agile_GetTypeDataByID(typeID);
        else
            pAgileTypeData = FUNC::Agile_GetTypeDataByID_func2(ReadMemory(ADDR::AGILE_TYPE_DATA) + 0xC, FUNC::Agile_GetTypeDataByID_func1(&typeID), &typeID);
        CREATE_OBJECT_DATA data{};
        FUNC::Agile_PrepareCreateObjectData(&data, typeID, pAgileTypeData->generator);
        data.net_mode = async ? -2 : -1;
        CAgentBaseAbs* pCAgentAbs = FUNC::Agile_CreateAgentAbs(&data, 1, 1);
        pCAgentAbs->agent->reference_count++;
        return pCAgentAbs->agent;
    }
    bool Agile::IsChild(uint32_t childTypeID, uint32_t parentTypeID) {
        if (childTypeID == parentTypeID)
            return true;
        AGILE_TYPE_DATA* pAgileTypeData;
        while (true) {
            if (version >= version_127a)
                pAgileTypeData = FUNC::Agile_GetTypeDataByID(childTypeID);
            else
                pAgileTypeData = FUNC::Agile_GetTypeDataByID_func2(ReadMemory(ADDR::AGILE_TYPE_DATA) + 0xC, FUNC::Agile_GetTypeDataByID_func1(&childTypeID), &childTypeID);
            if (!pAgileTypeData || pAgileTypeData->parentTypeID == childTypeID) // 无数据 / 父 == 子
                return false;
            childTypeID = pAgileTypeData->parentTypeID; // 父类型ID
            if (childTypeID == parentTypeID)
                return true;
        }
    }
    bool Agile::IsChild(CAgent* pAgent, uint32_t typeID) {
        if (!pAgent)
            return false;
        return IsChild(pAgent->GetTypeID(), typeID);
    }
    CAgent* CGameState::FromHandle(uint32_t handle) {
        CAgent* pAgent = FUNC::CGameState_FromHandle(this, handle);
        if (pAgent && pAgent->isValid())
            return pAgent;
        return NULL;
    }
    CAgent* CGameState::FromHandleEx(uint32_t handle, uint32_t typeID) {
        CAgent* pAgent = FromHandle(handle);
        if (pAgent && Agile::IsChild(pAgent, typeID))
            return pAgent;
        return NULL;
    }
    bool CAgent::isValid() {
        CAgentBaseAbs* pAgentBaseAbs = FUNC::GetAgentBaseAbsByTag(AgentBaseAbsTag.tagA, AgentBaseAbsTag.tagB);
        if (pAgentBaseAbs && pAgentBaseAbs->ID == '+agl' && !pAgentBaseAbs->deleted)
            return true;
        return false;
    }
    CSprite::Type CSprite::GetSpriteType() {
        if (IsBadReadPtr((void*)this, 4))
            return Type::INVALID;

        if (__vfn == VFN::CSpriteMini_ || __vfn == VFN::TAllocatedHandleObjectLeaf_CSpriteMini_256)
            return Type::MINI;

        if (__vfn == VFN::CSpriteUber_ || __vfn == VFN::TAllocatedHandleObjectLeaf_CSpriteUber_128)
            return Type::UBER;

        return Type::INVALID;
    }
    void CSpriteUber_::UpdateColor() {
        color.unk_16 = 0;
        color.unk_20 = 0;
        uint32_t flag = color.unk_12;
        if (!(flag & 0b100000000000))
            color.unk_12 = flag | 0b100000000000;
    }
    void CEffectImage::TimedRemove(float* timeout) {
        uint32_t async = 0;
        CAgentBaseAbs* pAgentBaseAbs = FUNC::GetAgentBaseAbsByTag(AgentBaseAbsTag.tagA, AgentBaseAbsTag.tagB);
        if (pAgentBaseAbs && pAgentBaseAbs->ID == '+agl' && !pAgentBaseAbs->deleted && pAgentBaseAbs->self.tagA < 0)
            async = 1;
        destroy_timer.Start(timeout, CEffectImage::eventID_destroy, this, 0, async);
    }
    void NTempest::C33Matrix::operator=(float r[3][3]) {
        for (uint32_t i = 0; i < 3; i++)
            for (uint32_t j = 0; j < 3; j++)
                matrix[i][j] = r[i][j];
    }
    void NTempest::C33Matrix::operator*=(float r[3][3]) {
        float tmp[3][3];

        for (uint32_t i = 0; i < 3; i++)
            for (uint32_t j = 0; j < 3; j++)
                tmp[i][j] = matrix[i][j];

        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++) {
                float tmp1(0);
                for (size_t k = 0; k < 3; k++)
                    tmp1 += tmp[i][k] * r[k][j];
                matrix[i][j] = tmp1;
            }
    }
    float CAgentTimer::GetRemaining() {
        float out;
        base::this_call_vf<void>(this, 0x18, &out);
        return out;
    }
    bool CAgentTimer::IsRunning() {
        return unk_12 != 0 && (ReadMemory<uint8_t>(unk_12 + 0x12) & 1) == 0;
    }
    void CAgentTimer::SetNextTimeout(float timeout) {
        if (unk_12)
            WriteMemory<float>(unk_12 + 0x8, timeout);
    }
    void CTimerWar3::SetTimeout(float timeout) {
        this->timeout.value = timeout;
        timer.SetNextTimeout(timeout);
    }
    CUnit* CAbility::GetOwner() {
        if (unit)
            return unit;
        CAgentBaseAbs* pAgentBaseAbs = FUNC::GetAgentBaseAbsByTag(AgentBaseAbsTag.tagA, AgentBaseAbsTag.tagB);
        if (pAgentBaseAbs && pAgentBaseAbs->ID == '+agl') {
            pAgentBaseAbs = pAgentBaseAbs->unk_48;
            if (pAgentBaseAbs && !pAgentBaseAbs->deleted)
                return (CUnit*)pAgentBaseAbs->agent;
        }
        return NULL;
    }
    NTempest::C3Vector Position::GetLocation() {
        NTempest::C3Vector ret;
        FUNC::Position_GetLocation(this, &ret);
        return ret;
    }
    void Position::SetLocation(NTempest::C3Vector* xyz) {
        FUNC::Position_SetLocation(this, xyz);
    }
    uint32_t CDataAllocator::CreateObj(uint32_t zero, const char* srcfile, uint32_t srcline) {
        return FUNC::CDataAllocator_CreateObj(this, zero, srcfile, srcline);
    }
    void CDataAllocator::FreeObj(uint32_t obj, const char* srcfile, uint32_t srcline) {
        FUNC::CDataAllocator_FreeObj(this, obj, srcfile, srcline);
    }
    uint32_t CDataAllocator::FreeAllObj(const char* srcfile, uint32_t srcline, uint32_t a4) {
        return FUNC::CDataAllocator_FreeAllObj(this, srcfile, srcline, a4);
    }
    uint32_t CGameHashTableManager::GetStoredHandle(uint32_t key, uint32_t parentKey, uint32_t childKey) {
        return FUNC::CGameHashTableManager_GetStoredHandle(this, key, parentKey, childKey);
    }
    void CGameHashTableManager::StoreHandle(uint32_t key, uint32_t parentKey, uint32_t childKey, uint32_t handle, uint32_t flag) {
        return FUNC::CGameHashTableManager_StoreHandle(this, key, parentKey, childKey, handle, flag);
    }
    void CAgentTimer::Start(float* timeout, uint32_t evenID, CAgent* target, uint32_t periodic, uint32_t async) {
        return FUNC::CAgentTimer_Start(this, timeout, evenID, target, periodic, async);
    }
    void CAgentTimer::Stop() {
        return FUNC::CAgentTimer_Stop(this);
    }
    double CSprite::SetAnimation(uint32_t a1, uint32_t a2, uint32_t flag) {
        return FUNC::SetSpriteAnimation(this, a1, a2, flag);
    }
    double CSprite::SetAnimationByIndex(uint32_t index, uint32_t flag) {
        return FUNC::SetSpriteAnimationByIndex(this, index, flag);
    }
    void CSprite::SetReplacableTexture(const char* path, uint32_t replaceableID, uint32_t flag) {
        uint32_t pTexture = FUNC::LoadTexture(path, flag);
        if (pTexture)
            FUNC::CSprite_SetReplacableTexture(this, pTexture, replaceableID);
    }
    void CSprite::SetTeamColor(uint32_t playerID, uint32_t color) {
        return FUNC::SetSpriteTeamColor(this, playerID, color);
    }
    void CSprite::SetTimeScale(float timescale) {
        base::this_call_vf<void>(this, 0x28, timescale);
    }
    float CSprite::GetTimeScale() {
        return base::this_call_vf<float>(this, 0x48);
    }
    void CSprite::SetAlpha(uint32_t alpha) {
        return base::this_call_vf<void>(this, 0x34, alpha & 0xFF);
    }
    void CSprite::SetSize(float size) {
        return base::this_call_vf<void>(this, 0x24, size);
    }
    void CSprite::SetColor(uint32_t color) {
        color &= 0xFFFFFF;
        return base::this_call_vf<void>(this, 0x30, &color /* NTempest::CImVector* */);
    }
    uint32_t CObserver::IsEventRegistered(uint32_t eventID) {
        return FUNC::CObserver_IsEventRegistered(this, eventID);
    }
    uint32_t CObserver::DispatchEvent(CEvent* pEvent) {
        return FUNC::CObserver_DispatchEvent(this, pEvent);
    }
    CLight* CLight::Create() {
        return FUNC::CreateLight();
    }
    CLight* CLight::CreateEx() {
        CLight* pLight = Create();
        float unk[3];
        unk[0] = 0.330000013113022f;
        pLight->Setup1(2, unk); // setupLight1
        unk[0] = 1.f;
        unk[1] = 1.f;
        unk[2] = 1.f;
        pLight->Setup2(0, unk); // setupLight2
        unk[0] = 1.f;
        pLight->Setup1(3, unk); // setupLight1
        unk[0] = 1.f;
        unk[1] = 1.f;
        unk[2] = 1.f;
        pLight->Setup2(1, unk); // setupLight2
        unk[0] = -1.f;
        unk[1] = 0.800000011920929f;
        unk[2] = -1.f;
        pLight->Setup3(4, unk, 0);
        return pLight;
    }
    void CSimpleMessageFrame::AddText(const char* text, uint32_t color, float duration, uint32_t flag) {
        return base::this_call_vf<void>(this, 0x6C, text, &color, duration, flag);
    }
    void CSimpleMessageFrame::ClearText() {
        return FUNC::CSimpleMessageFrame_ClearText(this);
    }
    CGameUI* CGameUI::GetInstance(uint32_t createIfNotExist, uint32_t free) {
        return FUNC::GetGameUI(createIfNotExist, free);
    }
    void CSpriteFrame::SetLight(CLight* pLight, uint32_t a2) {
        return FUNC::CSpriteFrame_SetLight(this, pLight, a2);
    }
    uint32_t CAgent::GetTypeID() {
        return base::this_call_vf<uint32_t>(this, 0x1C);
    }
    void CAgent::Destroy() {
        return base::this_call_vf<void>(this, 0x5C);
    }
    void CAgent::SaveEvent(CSaveGame* pSaveGame, uint32_t eventID, uint32_t a4) {
        return base::this_call_vf<void>(this, 0x50, pSaveGame, eventID, a4);
    }
    void CAgent::LoadEvent(CSaveGame* pSaveGame, uint32_t eventID, uint32_t a4) {
        return base::this_call_vf<void>(this, 0x54, pSaveGame, eventID, a4);
    }
    void CPlayerWar3::DispatchScriptEvent(uint32_t eventID, CScriptEventData* pScriptEventData) {
        return FUNC::CPlayerWar3_DispatchScriptEvent(this, eventID, pScriptEventData);
    }
    Position* CWidget::GetPosition() {
        return base::this_call_vf<Position*>(this, 0xB8);
    }
    void CUnit::AddAbility(CAbility* pAbility) {
        return FUNC::CUnit_AddAbility(this, pAbility);
    }
    CAbility* CUnit::GetAbilityByID(uint32_t abilID, uint32_t check_code, uint32_t check_alias, uint32_t check_item_ability, uint32_t check_unit_ability) {
        return FUNC::CUnit_GetAbilityByID(this, abilID, check_code, check_alias, check_item_ability, check_unit_ability);
    }
    CPlayerWar3* CUnit::GetOwner() {
        return FUNC::CUnit_GetOwner(this);
    }
    float CAbility::GetDataC() {
        float ret;
        FUNC::CAbility_GetDataC(this, &ret, level);
        return ret;
    }
    void CAbility::UpdateUI() {
        return FUNC::CAbility_UpdateUI(this);
    }
    void CBuff::UpdateUI() {
        FUNC::CBuff_UpdateUI(this);
    }
    void CBuff::Init(BuffInfo* buffInfo) {
        return base::this_call_vf<void>(this, 0x324, buffInfo);
    }
    bool CBooleanExpr::Evaluate() {
        return base::this_call_vf<uint32_t>(this, 0x78);
    }
    void CTriggerWar3::AddEvent(CEventRegWar3* pEventRegWar3) {
        return FUNC::CTriggerWar3_AddEvent(this, pEventRegWar3);
    }
    void CPlayerEventReg::Initialize(CTriggerWar3* pTriggerWar3, CPlayerWar3* pPlayerWar3, uint32_t eventID, CBooleanExpr* pBooleanExpr) {
        return FUNC::CPlayerEventReg_Initialize(this, pTriggerWar3, pPlayerWar3, eventID, pBooleanExpr);
    }
    void CTimerWar3::Start(float timeout, uint32_t is_periodic) {
        timer.Start(&timeout, CTimerWar3::eventID_expire, this, is_periodic, 0);
    }
    CGameState* CGameWar3::GetGameState() {
        return FUNC::CGameWar3_GetGameState(this);
    }
    CGameWar3* CGameWar3::GetInstance() {
        return ReadMemory<CGameWar3*>(ADDR::CGameWar3);
    }
    CAgent* CGameState::Peek(DataType dataType) {
        return FUNC::CGameState_Peek(this, dataType);
    }
    void CGameState::Push(DataType dataType, CAgent* data, uint32_t flag) {
        return FUNC::CGameState_Push(this, dataType, data, flag);
    }
    CAgent* CGameState::Pop(DataType dataType, uint32_t flag) {
        return FUNC::CGameState_Pop(this, dataType, flag);
    }
    uint32_t CGameState::ToHandle(CAgent* agent, uint32_t autoDestroy) {
        return FUNC::CGameState_ToHandle(this, agent, autoDestroy);
    }
    CUnit* CUnitSet::At(uint32_t index) {
        return FUNC::CUnitSet_At(this, index);
    }

    namespace ASM {
        uint32_t JassInstance_check_opcode_counter;
        uint32_t JassInstance_add_opcode_counter;
        uint32_t JassInstance_add_opcode_counter_SIZE;
        uint32_t CDestructable_UpdateColor_mov_destructableID;
        uint32_t CDestructable_UpdateColor_call_GetDestructableColorById;
        uint32_t CItem_UpdateColor_mov_itemID;
        uint32_t CItem_UpdateColor_call_GetItemColorById;
    }
    namespace VFN {
        uint32_t CSpriteFrame;
        uint32_t CAbilityChannel;
        uint32_t CAbilityStoneForm;
        uint32_t CUnit;
        uint32_t CDestructable;
        uint32_t CAbilityPurchaseItem;
        uint32_t CItem;
        uint32_t CAbilityRangerArrow;
        uint32_t CAbilityNeutral;
        uint32_t CBuff;
        uint32_t CSpriteMini_;
        uint32_t TAllocatedHandleObjectLeaf_CSpriteMini_256;
        uint32_t CSpriteUber_;
        uint32_t TAllocatedHandleObjectLeaf_CSpriteUber_128;
        uint32_t InstanceGenerator_CEffectImagePos;
        uint32_t CPlayerEventReg;
        uint32_t CPlayerWar3;
        uint32_t CObserver;
        uint32_t CPlayerUnitEventDataBase;
    }
    namespace ADDR {
        uint32_t item_data_table;
        uint32_t CPlayerEventReg_DispatchEvent_ptr;
        uint32_t AGILE_TYPE_DATA; // < 1.27
        uint32_t CGameWar3;
    }
    namespace FUNC {
        void(__thiscall* CAgentTimer_Start)(CAgentTimer* pAgentTimer, float* duration, uint32_t eventID, CAgent* target, uint32_t periodic, uint32_t async);
        void(__thiscall* CAgentTimer_Stop)(CAgentTimer* pAgentTimer);
        void(__thiscall* CUnit_DisplayMissText)(CUnit* pUnit);
        void(__thiscall* Position_GetLocation)(Position* pPosition, NTempest::C3Vector* vector);
        uint32_t(__thiscall* CAbilityAttack_AttackTarget)(CAbilityAttack* pAbilityAttack, CWidget* pTargetWidget, uint32_t weapon_index, uint32_t, uint32_t, uint32_t, uint32_t exposeSource);
        void(__thiscall* CUnit_AddAbility)(CUnit* pUnit, CAbility* pAbility);
        void(__thiscall* CBuff_UpdateUI)(CBuff* pBuff);
        uint32_t(__thiscall* JassInstance_CallNative)(JassInstance* _this, const char* funcName);
        uint32_t(__thiscall* JassInstance_ConvertCode)(JassInstance* _this, uint32_t code);
        CAbility* (__thiscall* CUnit_GetAbilityByID)(CUnit* pUnit, uint32_t abilID, uint32_t, uint32_t, uint32_t, uint32_t);
        uint32_t(__thiscall* CUnit_OnDamage)(CUnit* pUnit, uint32_t, uint32_t ptr, uint32_t is_physical, CUnit* source_unit);
        void(__thiscall* CUnit_DispatchDamaged)(CUnit* pUnit, float* amount, CUnit* pSrcUnit);
        void(__thiscall* Position_SetLocation)(Position* pPosition, NTempest::C3Vector* xyz);
        void(__fastcall* SetSpriteTeamColor)(CSprite* pSprite, uint32_t playerID, uint32_t color);
        void(__fastcall* GetAnimationDataFromJassString)(uint32_t animName, uint32_t* animData);
        double(__fastcall* SetSpriteAnimation)(CSprite* pSprite, uint32_t, uint32_t, uint32_t flag);
        double(__fastcall* SetSpriteAnimationByIndex)(CSprite* pSprite, uint32_t index, uint32_t flag);
        CLight* (__cdecl* CreateLight)();
        void(__fastcall* Light_Setup3)(CLight* pLight, uint32_t, float*, uint32_t);
        void(__thiscall* CSpriteFrame_SetLight)(CSpriteFrame* pSpriteFrame, CLight* pLight, uint32_t);
        void(__thiscall* CSimpleMessageFrame_ClearText)(CSimpleMessageFrame* pSimpleMessageFrame);
        CGameUI* (__fastcall* GetGameUI)(uint32_t createIfNotExist, uint32_t free);
        CUnit* (__thiscall* CUnitSet_At)(CUnitSet* pUnitSet, uint32_t index);
        uint32_t(__thiscall* CGameHashTableManager_GetStoredHandle)(CGameHashTableManager* pGameHashTableManager, uint32_t, uint32_t parentKey, uint32_t childKey);
        void(__thiscall* CGameHashTableManager_StoreHandle)(CGameHashTableManager* pGameHashTableManager, uint32_t, uint32_t parentKey, uint32_t childKey, uint32_t handle, uint32_t flag);
        uint32_t(__stdcall* LoadTexture)(const char* filepath, uint32_t);
        void(__fastcall* CSprite_SetReplacableTexture)(CSprite* pSprite, uint32_t pTexture, uint32_t replaceableID);
        double(__fastcall* GetTerrainZ)(uint32_t, uint32_t, float x, float y, uint32_t);
        uint32_t(__thiscall* DecodeTargetArgsString)(const char* targetArgs);
        void(__thiscall* CAbility_UpdateUI)(CAbility* pAbility);
        uint32_t(__thiscall* CAbilityChannel_GetFlag)(CAbilityChannel* pAbilityChannel);
        void(__thiscall* CAbility_GetDataC)(CAbility* pAbility, float* output, uint32_t level);
        uint32_t* (__fastcall* GetDestructableColorById)(uint32_t* output, uint32_t destructableID);
        void(__thiscall* CUnit_UpdateInventoryUI) (CUnit* pUnit);
        uint32_t* (__fastcall* GetItemColorById)(uint32_t* output, uint32_t itemID);
        uint32_t(__thiscall* CDataAllocator_CreateObj)(CDataAllocator* pDataAllocator, uint32_t defaultZero, const char* sourcefile, uint32_t sourceline);
        uint32_t(__thiscall* CDataAllocator_FreeObj)(CDataAllocator* pDataAllocator, uint32_t pObj, const char* sourcefile, uint32_t sourceline);
        uint32_t(__thiscall* CDataAllocator_FreeAllObj)(CDataAllocator* pDataAllocator, const char* sourcefile, uint32_t sourceline, uint32_t);
        void(__fastcall* RegisterType)(uint32_t ID, uint32_t parentID, InstanceGenerator* pInstanceGenerator);
        void(__thiscall* CPlayerEventReg_Initialize)(CPlayerEventReg* pPlayerEventReg, CTriggerWar3* pTriggerWar3, CPlayerWar3* pPlayerWar3, uint32_t eventID, CBooleanExpr* pBooleanExpr);
        void(__thiscall* CTriggerWar3_AddEvent)(CTriggerWar3* pTriggerWar3, CEventRegWar3* pEventRegWar3);
        void(__thiscall* CGameState_Push)(CGameState* pGameState, CGameState::DataType dataType, CAgent* data, uint32_t flag);
        CAgent* (__thiscall* CGameState_Peek)(CGameState* pGameState, CGameState::DataType dataType);
        CAgent* (__thiscall* CGameState_Pop)(CGameState* pGameState, CGameState::DataType dataType, uint32_t flag);
        uint32_t(__thiscall* CPlayerEventReg_ProcessEvent)(CPlayerEventReg* pPlayerEventReg, CScriptEvent* pEvent);
        uint32_t(__thiscall* CObserver_DispatchEvent)(CObserver* pPlayerEventReg, CEvent* pEvent);
        void(__thiscall* CPlayerWar3_Save)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
        void(__thiscall* CPlayerWar3_Load)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
        CPlayerWar3* (__thiscall* CUnit_GetOwner)(CUnit* pUnit);
        uint32_t(__thiscall* CObserver_IsEventRegistered)(CObserver* pObserver, uint32_t eventID);
        void(__thiscall* CPlayerWar3_DispatchScriptEvent)(CPlayerWar3* pPlayerWar3, uint32_t eventID, CScriptEventData* pScriptEventData);
        AGILE_TYPE_DATA* (__thiscall* Agile_GetTypeDataByID)(uint32_t typeID);
        uint32_t(__thiscall* Agile_GetTypeDataByID_func1)(uint32_t* typeID); // < 1.27
        AGILE_TYPE_DATA* (__thiscall* Agile_GetTypeDataByID_func2)(uint32_t, uint32_t, uint32_t* typeID); // < 1.27
        void(__fastcall* Agile_PrepareCreateObjectData)(Agile::CREATE_OBJECT_DATA* data, uint32_t typeID, InstanceGenerator* generator);
        CAgentBaseAbs* (__fastcall* Agile_CreateAgentAbs)(Agile::CREATE_OBJECT_DATA* data, uint32_t, uint32_t);
        uint32_t(__thiscall* CGameState_ToHandle)(CGameState* pGameState, CAgent* pAgent, uint32_t autoDestroy);
        CAgent* (__thiscall* CGameState_FromHandle)(CGameState* pGameState, uint32_t handle);
        CGameState* (__thiscall* CGameWar3_GetGameState)(CGameWar3* pGameWar3);
        CAgentBaseAbs* (__fastcall* GetAgentBaseAbsByTag)(uint32_t tagA, uint32_t tagB);
        uint32_t(__fastcall* GetInstance)(InstanceType type);
        uint32_t(__stdcall* SStrHash)(const char* str);
    }
    namespace UTIL {
        object_tag GetAgentTag(CAgent* agent) {
            CAgentBaseAbs* pAgentBaseAbs = FUNC::GetAgentBaseAbsByTag(agent->AgentBaseAbsTag.tagA, agent->AgentBaseAbsTag.tagB);
            if (pAgentBaseAbs && pAgentBaseAbs->ID == '+agl')
                return pAgentBaseAbs->self;
            else
                return { 0xFFFFFFFF, 0xFFFFFFFF };
        }
        CAgent* GetAgentByTag(object_tag tag, uint32_t typeID) {
            if ((tag.tagA & tag.tagB) == 0xFFFFFFFF) // opt: A == 0xFFFFFFFF && B == 0xFFFFFFFF
                return NULL;
            CAgentBaseAbs* pAgentBaseAbs = FUNC::GetAgentBaseAbsByTag(tag.tagA, tag.tagB);
            if (!pAgentBaseAbs || pAgentBaseAbs->deleted || !Agile::IsChild(pAgentBaseAbs->agent, typeID))
                return NULL;
            return pAgentBaseAbs->agent;
        }
    }
    init_H(war3) {
#define Set(a) do { WriteMemory((uint32_t)&a, ptr); ptr = 0; } while(0)
#define Set_(a, b) WriteMemory((uint32_t)&a, b)
#define Set_vfn(a) WriteMemory((uint32_t)&VFN::##a, get_vfn_ptr(".?AV" #a "@@"))
#define Set_vfn_(a, b) WriteMemory((uint32_t)&VFN::##a, get_vfn_ptr(b))
        war3_searcher& s = get_war3_searcher();
        version = (VERSION)s.get_version();
        uint32_t ptr;

        Set_vfn(CSpriteFrame);
        Set_vfn(CAbilityChannel);
        Set_vfn(CAbilityStoneForm);
        Set_vfn(CUnit);
        Set_vfn(CDestructable);
        Set_vfn(CAbilityPurchaseItem);
        Set_vfn(CItem);
        Set_vfn(CAbilityRangerArrow);
        Set_vfn(CAbilityNeutral);
        Set_vfn(CBuff);
        Set_vfn(CSpriteMini_);
        Set_vfn_(TAllocatedHandleObjectLeaf_CSpriteMini_256, ".?AV?$TAllocatedHandleObjectLeaf@VCSpriteMini_@@$0BAA@@@");
        Set_vfn(CSpriteUber_);
        Set_vfn_(TAllocatedHandleObjectLeaf_CSpriteUber_128, ".?AV?$TAllocatedHandleObjectLeaf@VCSpriteUber_@@$0IA@@@");
        Set_vfn_(InstanceGenerator_CEffectImagePos, ".?AV?$InstanceGenerator@VCEffectImagePos@@@@");
        Set_vfn(CPlayerEventReg);
        Set_vfn(CPlayerWar3);
        Set_vfn(CObserver);
        Set_vfn(CPlayerUnitEventDataBase);

        Set_(ADDR::CPlayerEventReg_DispatchEvent_ptr, VFN::CPlayerEventReg + 0xC);

        /* CAgentTimer_Start */ {
            uint32_t str = s.search_string_ptr("EffectDeathTime", sizeof("EffectDeathTime"));
            for (ptr = s.search_int_in_text(str); ptr; ptr = s.search_int_in_text(str, ptr + 1)) {
                uint32_t func = s.current_function(ptr);
                if (ptr - func > 0x10) {
                    ptr += 4;
                    ptr = next_opcode(ptr, 0xE8, 5);
                    ptr += 5;
                    ptr = next_opcode(ptr, 0xE8, 5);
                    ptr = convert_function(ptr);
                    Set(FUNC::CAgentTimer_Start);
                    break;
                }
            }
            if (!FUNC::CAgentTimer_Start)
                __debugbreak();
        }
        /* CAgentTimer_Stop */ {
            ptr = s.search_string("PauseTimer");
            ptr = ReadMemory(ptr + 0x05);
            ptr = next_opcode(ptr, 0xE9, 5);
            ptr = convert_function(ptr);
            if (version < version_127a) {
                ptr = next_opcode(ptr, 0xE9, 5);
                ptr = convert_function(ptr);
            }
            else {
                ptr = next_opcode(ptr, 0xE8, 5);
                ptr += 5;
                ptr = next_opcode(ptr, 0xE8, 5);
                ptr = convert_function(ptr);
            }
            Set(FUNC::CAgentTimer_Stop);
        }
        /* CUnit_DisplayMissText */ {
            ptr = s.search_string("MISS", sizeof("MISS"));
            ptr = s.current_function(ptr);
            Set(FUNC::CUnit_DisplayMissText);
        }
        /* Position_GetLocation / Position_SetLocation */ {
            ptr = s.search_string("SetUnitX");
            ptr = ReadMemory(ptr + 0x05);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::Position_GetLocation, convert_function(ptr));
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::Position_SetLocation);
        }
        /* CAbilityAttack_AttackTarget */ {
            ptr = ReadMemory(VFN::CAbilityRangerArrow + 0x424);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CAbilityAttack_AttackTarget);
        }
        /* CUnit_AddAbility */ {
            ptr = VFN::CAbilityNeutral;
            ptr = ReadMemory(ptr + 0x80); // vftable + 0x80
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            if (version >= version_127a) {
                ptr += 5;
                ptr = next_opcode(ptr, 0xE8, 5);
            }
            ptr = convert_function(ptr);
            Set(FUNC::CUnit_AddAbility);
        }
        /* CBuff_UpdateUI */ {
            ptr = VFN::CBuff;
            ptr = ReadMemory(ptr + 0xA4); // vftable + 0xA4
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CBuff_UpdateUI);
        }
        /* JassInstance_CallNative / JassInstance_ConvertCode / JassInstance_check_opcode_counter / JassInstance_add_opcode_counter*/ {
            ptr = s.search_string("config");
            ptr += 4;

            ptr = next_opcode(ptr, 0xE8, 5);            //搜索下面第一个call
            ptr = convert_function(ptr);                //进入这个函数

            ptr = next_opcode(ptr, 0xE8, 5);            //搜索第一个call
            ptr += 0x5;                                 //+5
            ptr = next_opcode(ptr, 0xE8, 5);            //搜索到第二个call
            ptr = convert_function(ptr);                //进入

            ptr = next_opcode(ptr, 0xE8, 5);            //搜索第一个call
            ptr = convert_function(ptr);                //进入
            uint32_t JassInstance_execute = ptr;

            ptr = next_opcode(ptr, 0xFF, 7);            //搜索第一个call
            ptr += 0x3;
            ptr = ReadMemory(ptr);                      //取到跳转地址表地址
            ptr += 0x4C;                                //跳转到偏移0x13 * 4的地址处
            ptr = ReadMemory(ptr);
            ptr = next_opcode(ptr, 0xE8, 5);            //搜索第一个call
            ptr += 0x5;                                 //+5
            ptr = next_opcode(ptr, 0xE8, 5);            //搜索到第二个call
            ptr = convert_function(ptr);                //进入
            Set_(FUNC::JassInstance_CallNative, ptr);

            ptr = next_opcode(ptr, 0xE8, 5);            //搜索第一个call
            ptr += 0x5;                                 //+5
            ptr = next_opcode(ptr, 0xE8, 5);            //搜索到第二个call
            ptr = convert_function(ptr);                //进入
            Set_(FUNC::JassInstance_ConvertCode, ptr);

            ptr = JassInstance_execute;
            ptr = next_opcode(ptr, 0x0F, 6);
            while (*(unsigned char*)(ptr + 1) != 0x8C) {
                ptr += 6;
                ptr = next_opcode(ptr, 0x0F, 6);
            }
            Set(ASM::JassInstance_check_opcode_counter);

            ptr = JassInstance_execute;
            uint32_t last_3_opcode = next_opcode(ptr);
            uint32_t last_2_opcode = next_opcode(last_3_opcode);
            uint32_t last_opcode = next_opcode(last_2_opcode);
            while (last_opcode != ASM::JassInstance_check_opcode_counter) {
                ptr = last_3_opcode;
                last_3_opcode = last_2_opcode;
                last_2_opcode = last_opcode;
                last_opcode = next_opcode(last_2_opcode);
            }
            ASM::JassInstance_add_opcode_counter_SIZE = last_3_opcode - ptr;
            Set(ASM::JassInstance_add_opcode_counter);
        }
        /* CUnit_GetAbilityByID */ {
            ptr = s.search_string("UnitRemoveAbility");
            ptr = ReadMemory(ptr + 0x05);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CUnit_GetAbilityByID);
        }
        /* CUnit_OnDamage */ {
            uint32_t str = s.search_string_ptr("EtherealDamageBonusAlly", sizeof("EtherealDamageBonusAlly"));

            for (ptr = s.search_int_in_text(str); ptr; ptr = s.search_int_in_text(str, ptr + 1)) {
                uint32_t func = s.current_function(ptr);
                if (ptr - func > 1000) {
                    ptr = func;
                    Set(FUNC::CUnit_OnDamage);
                    break;
                }
            }
            if (!FUNC::CUnit_OnDamage)
                __debugbreak();
        }
        /* CUnit_DispatchDamaged */ {
            for (ptr = s.search_int_in_text(524852 /* 事件ID */); ptr; ptr = s.search_int_in_text(524852, ptr + 1)) {
                if (ReadMemory<uint8_t>(ptr - 1) == 0x68) {// push 0x80234
                    ptr = s.current_function(ptr);
                    Set(FUNC::CUnit_DispatchDamaged);
                    break;
                }
            }
            if (!FUNC::CUnit_DispatchDamaged)
                __debugbreak();
        }
        /* SetSpriteTeamColor */ {
            ptr = s.search_string("SetUnitColor");
            ptr = ReadMemory(ptr + 0x05);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::SetSpriteTeamColor);
        }
        /* GetAnimationDataFromJassString */ {
            ptr = s.search_string("SetUnitAnimation");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::GetAnimationDataFromJassString, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::SetSpriteAnimation);
        }
        /* SetSpriteAnimationByIndex */ {
            ptr = s.search_string("SetUnitAnimationByIndex");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::SetSpriteAnimationByIndex);
        }
        /* CreateLight / Light_Setup3 / CSpriteFrame_SetLight */ {
            ptr = s.search_string("CinematicDialogueText");
            ptr += 4;

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CreateLight, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::Light_Setup3, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CSpriteFrame_SetLight);
        }
        /* CSimpleMessageFrame_ClearText */ {
            ptr = s.search_string("ClearTextMessages");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE9, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE9, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CSimpleMessageFrame_ClearText);
        }
        /* GetGameUI */ {
            ptr = s.search_string("EnableSelect");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::GetGameUI);
        }
        /* CUnitSet_At */ {
            ptr = s.search_string("FirstOfGroup");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 0x05;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CUnitSet_At);
        }
        /* CGameHashTableManager_GetStoredHandle */ {
            ptr = s.search_string("LoadPlayerHandle");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            Set(FUNC::CGameHashTableManager_GetStoredHandle);
        }
        /* CGameHashTableManager_StoreHandle */ {
            ptr = s.search_string("SavePlayerHandle");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            Set(FUNC::CGameHashTableManager_StoreHandle);
        }
        /* item_data_table */ {
            if (version != version_127a) {
                ptr = s.search_string("IsItemIdSellable");
                ptr = ReadMemory(ptr + 0x05);

                ptr = next_opcode(ptr, 0xE9, 5);
                ptr = convert_function(ptr);

                ptr = next_opcode(ptr, 0xB9, 5);
                ptr = ReadMemory(ptr + 1);
            }
            else
                // 被 inline 了
                ptr = s.base() + 0xBEC238;

            Set(ADDR::item_data_table);
        }
        /* LoadTexture */ {
            ptr = s.search_string("ui\\widgets\\escmenu\\human\\observer-icon.blp");
            ptr += 4;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            Set(FUNC::LoadTexture);
        }
        /* CSprite_SetReplacableTexture */ {
            ptr = (uint32_t)FUNC::SetSpriteTeamColor;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            Set(FUNC::CSprite_SetReplacableTexture);
        }
        /* GetTerrainZ */ {
            ptr = s.search_string("GetLocationZ");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            Set(FUNC::GetTerrainZ);
        }
        /* DecodeTargetArgsString */ {
            ptr = s.search_string("Unknown Target Flag: \"%s\"", sizeof("Unknown Target Flag: \"%s\""));
            ptr = s.current_function(ptr);

            Set(FUNC::DecodeTargetArgsString);
        }
        /* CAbility_UpdateUI */ {
            ptr = ReadMemory(VFN::CAbilityStoneForm + 0x414);
            ptr = next_opcode(ptr, 0xE9, 5);
            ptr = convert_function(ptr);

            Set(FUNC::CAbility_UpdateUI);
        }
        /* CAbilityChannel_GetFlag */ {
            ptr = VFN::CAbilityChannel;
            ptr = ReadMemory(ptr + 0x30C);

            Set(FUNC::CAbilityChannel_GetFlag);
        }
        /* CAbility_GetDataC */ {
            ptr = VFN::CAbilityChannel;
            ptr = ReadMemory(ptr + 0x30C); // vftable + 0x30C
            ptr = next_opcode(ptr, 0xE8, 5); // GetDataB
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5); // float2int
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5); // GetDataC
            ptr = convert_function(ptr);

            Set(FUNC::CAbility_GetDataC);
        }
        /* GetDestructableColorById */ {
            ptr = ReadMemory(VFN::CDestructable + 0x104);
            ptr = next_opcode(ptr, 0x8B, 3);
            ASM::CDestructable_UpdateColor_mov_destructableID = ptr;

            ptr = next_opcode(ptr, 0xE8, 5);
            ASM::CDestructable_UpdateColor_call_GetDestructableColorById = ptr;

            ptr = convert_function(ptr);
            Set(FUNC::GetDestructableColorById);
        }
        /* CUnit_UpdateInventoryUI */ {
            uint32_t ptr = ReadMemory(VFN::CAbilityPurchaseItem + 0xC);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CUnit_UpdateInventoryUI);
        }
        /* GetItemColorById */ {
            ptr = ReadMemory(VFN::CItem + 0x104);
            ptr = next_opcode(ptr, 0x8B, 3);
            ASM::CItem_UpdateColor_mov_itemID = ptr;

            ptr = next_opcode(ptr, 0xE8, 5);
            ASM::CItem_UpdateColor_call_GetItemColorById = ptr;

            ptr = convert_function(ptr);
            Set(FUNC::GetItemColorById);
        }
        /* CDataAllocator_Create */ {
            ptr = ReadMemory(VFN::InstanceGenerator_CEffectImagePos + 0x0);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            if (version < VERSION::version_127a) {
                ptr = next_opcode(ptr, 0xE8, 5);
                ptr = convert_function(ptr);
            }
            Set(FUNC::CDataAllocator_CreateObj);
        }
        /* CDataAllocator_Free */ {
            ptr = ReadMemory(VFN::InstanceGenerator_CEffectImagePos + 0x4);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CDataAllocator_FreeObj);
        }
        /* CDataAllocator_FreeAll */ {
            ptr = ReadMemory(VFN::InstanceGenerator_CEffectImagePos + 0x8);
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CDataAllocator_FreeAllObj);
        }
        /* RegisterType */ {
            for (ptr = s.search_int_in_text('+w3a'); ptr; ptr = s.search_int_in_text('+w3a', ptr + 1)) {
                ptr += 4;
                if (ReadMemory<uint8_t>(ptr) == 0xE8) { // call
                    ptr = convert_function(ptr);
                    Set(FUNC::RegisterType);
                    break;
                }
            }
            if (!FUNC::RegisterType)
                __debugbreak();
        }
        /* CPlayerEventReg_Initialize / CTriggerWar3_AddEvent */ {
            ptr = s.search_string("TriggerRegisterPlayerUnitEvent");
            ptr = ReadMemory(ptr + 5);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            if (version < VERSION::version_127a) {
                ptr = next_opcode(ptr, 0xE8, 5);
                ptr += 5;
            }
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CPlayerEventReg_Initialize, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CTriggerWar3_AddEvent);
        }
        /* CGameState_Peek */ {
            ptr = s.search_string("GetTriggerEventId");
            ptr = ReadMemory(ptr + 5);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CGameState_Peek);
        }
        /* CGameState_Push / CGameState_Pop */ {
            ptr = s.search_string("TriggerExecute");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CGameState_Push, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CGameState_Pop);
        }
        /* CPlayerEventReg_ProcessEvent */ {
            ptr = ReadMemory(VFN::CPlayerEventReg + 0xC);
            Set(FUNC::CPlayerEventReg_ProcessEvent);
        }
        /* CPlayerWar3_Save */ {
            ptr = VFN::CPlayerWar3;
            ptr = ReadMemory(ptr + 0x38);
            Set(FUNC::CPlayerWar3_Save);
        }
        /* CPlayerWar3_Load */ {
            ptr = VFN::CPlayerWar3;
            ptr = ReadMemory(ptr + 0x3C);
            Set(FUNC::CPlayerWar3_Load);
        }
        /* CUnit_GetOwner */ {
            ptr = s.search_string("GetOwningPlayer");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::CUnit_GetOwner);
        }
        /* CObserver_IsEventRegistered / CPlayerWar3_DispatchScriptEvent*/ {
            for (ptr = s.search_int_in_text(524818 /* 事件ID */); ptr; ptr = s.search_int_in_text(524818, ptr + 1)) {
                if (ReadMemory<uint8_t>(ptr - 1) == 0x68) {// push 0x80212
                    Set_(FUNC::CObserver_IsEventRegistered, convert_function(next_opcode(ptr, 0xE8, 5)));
                    ptr = s.search_int_in_text(524818, ptr + 1);
                    if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
                        ptr = next_opcode(ptr, 0xE8, 5);
                        ptr = convert_function(ptr);
                        Set(FUNC::CPlayerWar3_DispatchScriptEvent);
                        break;
                    }
                }
            }
            if (!FUNC::CObserver_IsEventRegistered)
                __debugbreak();
            if (!FUNC::CPlayerWar3_DispatchScriptEvent)
                __debugbreak();
        }
        /* Agile_GetTypeDataByID / Agile_PrepareCreateObjectData / Agile_CreateAgentAbs */ {
            for (ptr = s.search_int_in_text(524818); ptr; ptr = s.search_int_in_text(524818, ptr + 1)) {
                if (ReadMemory<uint8_t>(ptr - 1) == 0x68) {
                    ptr = s.current_function(ptr);
                    break;
                }
            }
            if (!ptr)
                __debugbreak();

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            if (version >= version_127a) {
                Set_(FUNC::Agile_GetTypeDataByID, convert_function(ptr));
            }
            else {
                ptr += 5;
                ptr = next_opcode(ptr, 0x8B, 6);
                Set_(ADDR::AGILE_TYPE_DATA, ReadMemory<uint32_t>(ptr + 2));
                ptr = next_opcode(ptr, 0xE8, 5);
                Set_(FUNC::Agile_GetTypeDataByID_func1, convert_function(ptr));
                ptr += 5;
                ptr = next_opcode(ptr, 0xE8, 5);
                Set_(FUNC::Agile_GetTypeDataByID_func2, convert_function(ptr));
            }
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::Agile_PrepareCreateObjectData, convert_function(ptr));
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::Agile_CreateAgentAbs);
        }
        /* CGameWar3 / CGameState_ToHandle / CGameState_FromHandle / CGameWar3_GetGameState / GetAgentBaseAbsByTag */ {
            ptr = s.search_string("FirstOfGroup");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            uint32_t convert_handle_CGroup = convert_function(ptr);

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0x8B, 6);
            Set_(ADDR::CGameWar3, ReadMemory(ptr + 0x2));

            ptr += 6;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CGameWar3_GetGameState, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CGameState_ToHandle, convert_function(ptr));

            ptr = convert_handle_CGroup;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            Set_(FUNC::CGameState_FromHandle, convert_function(ptr));

            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::GetAgentBaseAbsByTag);
        }
        /* GetInstance */ {
            ptr = s.search_string("TriggerSleepAction");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE9, 5);
            ptr = convert_function(ptr);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::GetInstance);
        }
        /* SStrHash */ {
            ptr = s.search_string("StringHash");
            ptr = ReadMemory(ptr + 0x05);

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;

            ptr = next_opcode(ptr, 0xE8, 5);
            ptr = convert_function(ptr);
            Set(FUNC::SStrHash);
        }
    }
}
