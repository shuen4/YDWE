#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <base/hook/replace_pointer.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <deque>

#define EVENT_PLAYER_UNIT_DAMAGED 308

namespace warcraft3::japi {

uintptr_t searchUnitDamageFunc()
{
	war3_searcher& s = get_war3_searcher();
	uintptr_t str = s.search_string_ptr("EtherealDamageBonusAlly", sizeof("EtherealDamageBonusAlly"));

	for (uintptr_t ptr = s.search_int_in_text(str); ptr; ptr = s.search_int_in_text(str, ptr+1))
	{
		uintptr_t func = s.current_function(ptr);
		if (ptr - func > 1000)
		{
			return s.search_int_in_rdata(func);
		}
	}

	return 0;
}
uintptr_t getUnitDamageFunc()
{
    static uintptr_t s_result = searchUnitDamageFunc();
    return s_result;
}

uintptr_t searchCUnit_RunDamagedEvent() {
    war3_searcher& s = get_war3_searcher();

    for (uintptr_t ptr = s.search_int_in_text(524852 /* 事件ID */); ptr; ptr = s.search_int_in_text(524852, ptr + 1)) {
        if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80234
            return s.current_function(ptr);
        }
    }

    return 0;
}

uintptr_t searchCUnit_GetOwner() {
    uintptr_t ptr;

    //=========================================
    // (1)
    //
    // push		"()V"
    // mov		edx, "GetOwningPlayer"
    // mov		ecx, [GetOwningPlayer函数的地址] <----
    // call		BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("GetOwningPlayer");
    ptr = *(uintptr_t*)(ptr + 0x05);

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

uintptr_t searchCPlayer_RunUnitAttackedEvent() {
    war3_searcher& s = get_war3_searcher();

    for (uintptr_t ptr = s.search_int_in_text(524818 /* 事件ID */); ptr; ptr = s.search_int_in_text(524818, ptr + 1)) {
        if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
            return s.current_function(ptr);
        }
    }

    return 0;
}

struct ExecutePlayerUnitEvent {
    uint32_t pCObserver_IsEventRegistered;
    union {
        uint32_t pCreateAgileTypeDataByTypeId;
        struct {
            uint32_t pAgileTypeData;
            uint32_t pCreateAgileTypeDataByTypeIdFunc1;
            uint32_t pCreateAgileTypeDataByTypeIdFunc2;
        } inlined;
    };
    uint32_t pInitAgent;
    uint32_t pCreateAgentAbs;
    uint32_t pDispatchPlayerUnitEvent;
};
ExecutePlayerUnitEvent searchExecutePlayerUnitEvent() {
    ExecutePlayerUnitEvent ret = {};
    uintptr_t ptr;

    //=========================================
    //  CPlayer_RunUnitAttackedEvent:
    //      ...
    //      call CObserver_IsEventRegistered
    //      ...
    //      call CPlayerUnitEventDataBase_GetTypeId
    //      ...
    //  if inlined
    //      call CreateAgileTypeDataByTypeId
    //  else
    //      call reference_copy_ptr(&local_var, 0) // 实际动作只是把local_var设置0
    //      ...
    //      mov ebx, pAgileTypeData
    //      ...
    //      call CreateAgileTypeDataByTypeId_subfunc1
    //      ...
    //      call CreateAgileTypeDataByTypeId_subfunc2
    //  endif
    //      ...
    //      call pInitAgent
    //      ...
    //      call CreateAgentAbs
    //      ...
    //      call DispatchPlayerUnitEvent
    //=========================================

    ptr = searchCPlayer_RunUnitAttackedEvent();
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.pCObserver_IsEventRegistered = convert_function(ptr);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    if (get_war3_searcher().get_version() >= version_127a) {
        ret.pCreateAgileTypeDataByTypeId = convert_function(ptr);
    }
    else {
        ptr += 5;
        ptr = next_opcode(ptr, 0x8B, 6);
        ret.inlined.pAgileTypeData = ReadMemory<uint32_t>(ptr + 2);
        ptr = next_opcode(ptr, 0xE8, 5);
        ret.inlined.pCreateAgileTypeDataByTypeIdFunc1 = convert_function(ptr);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ret.inlined.pCreateAgileTypeDataByTypeIdFunc2 = convert_function(ptr);
    }
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.pInitAgent = convert_function(ptr);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.pCreateAgentAbs = convert_function(ptr);
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
    ret.pDispatchPlayerUnitEvent = convert_function(ptr);
    return ret;
}
struct GetEventDataBase {
    uint32_t IsValid;
    uint32_t GetDataBase;
};
GetEventDataBase searchGetEventDataBase() {
    GetEventDataBase ret = {};
    uintptr_t ptr;

    //=========================================
    // (1)
    //
    // push		"()V"
    // mov		edx, "GetEventDamage"
    // mov		ecx, [GetEventDamage函数的地址] <----
    // call		BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("GetEventDamage");
    ptr = *(uintptr_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  GetEventDamage:
    //      ...
    //      call IsValid
    //      ...
    //      call GetDataBase
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.IsValid = convert_function(ptr);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.GetDataBase = convert_function(ptr);
    return ret;
}
struct CreateHandle {
    uint32_t GetDataNode;
    uint32_t CreateOrGetHandleId;
};
CreateHandle searchCreateHandle() {
    CreateHandle ret = {};
    uintptr_t ptr;

    //=========================================
    // (1)
    //
    // push		"()V"
    // mov		edx, "GetEventDamageSource"
    // mov		ecx, [GetEventDamageSource函数的地址] <----
    // call		BindNative
    //=========================================
    ptr = warcraft3::get_war3_searcher().search_string("Player");
    ptr = *(uintptr_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  Player:
    //      ...
    //      call GetCPlayerById
    //      ...
    //      call GetDataNode
    //      ...
    //      call CreateOrGetHandleId
    //=========================================

    ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
    ret.GetDataNode = convert_function(ptr);
    ptr += 5;
    ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
    ret.CreateOrGetHandleId = convert_function(ptr);
    return ret;
}

struct TriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId {
    uint32_t p1;
    uint32_t p2;
    uint32_t p3;
};
TriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId searchTriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId() {
    TriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId ret = {};
    uintptr_t ptr;

    //=========================================
    // (1)
    //
    // push		"()V"
    // mov		edx, "TriggerRegisterPlayerUnitEvent"
    // mov		ecx, [TriggerRegisterPlayerUnitEvent函数的地址] <----
    // call		BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("TriggerRegisterPlayerUnitEvent");
    ptr = *(uintptr_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  TriggerRegisterPlayerUnitEvent:
    //      ...
    //      jae
    //      ...
    //      jmp
    //      ...
    //      ja
    //=========================================
    ptr = next_opcode(ptr, 0x73, 2);
    ret.p1 = ptr;
    ptr = next_opcode(ptr, 0xEB, 2);
    ret.p2 = ptr;
    ptr = next_opcode(ptr, 0x77, 2);
    ret.p3 = ptr;
    return ret;
}

struct war3_event_damage_data
{
	uint32_t source_unit;
	uint32_t weapon_type;
	uint32_t unk2;
	uint32_t flag;
	uint32_t amount;
	uint32_t damage_type;
	uint32_t unk6;
	uint32_t unk7;
	uint32_t attack_type;
};

struct event_damage_data
{
	bool                    physical;
	bool                    change;
	war3_event_damage_data* data;
	uint32_t                old_amount;
	uint32_t                new_amount;

	event_damage_data(uint32_t is_physical, war3_event_damage_data* ptr)
		: physical(!!is_physical)
		, change(false)
		, data(ptr)
		, old_amount(0)
		, new_amount(0)
	{ }

	bool is_same(uint32_t* damage1, uint32_t* damage2) const
	{
		if (!old_amount) 
		{
			return true;
		}

		if ((*damage1 ^ *damage2) != 0x80000000)
		{
			return false;
		}

		return old_amount == *damage1;
	}

    uint32_t damage_type() const
	{
		uint32_t v = data->damage_type;
		uint32_t n = 0;
		for (; v; v >>= 1)
		{
			++n;
		}

		return n-1;
	}
};


std::deque<event_damage_data> g_edd;

uintptr_t RealGetEventDamage = 0;
uint32_t __cdecl FakeGetEventDamage()
{
	if (!g_edd.empty())
	{
		event_damage_data& edd = g_edd.back();
		if (edd.change)
		{
			return g_edd.back().new_amount;
		}
	}

    static GetEventDataBase getEventDataBase = searchGetEventDataBase();
    if (jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGED && base::c_call<uint32_t>(getEventDataBase.IsValid))
        return ReadMemory<uint32_t>(base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x28);

	return base::fast_call<uint32_t>(RealGetEventDamage);
}

uintptr_t RealUnitDamageDoneFunc = 0;
uint32_t __fastcall FakeUnitDamageDoneFunc(uint32_t _this, uint32_t _edx, uint32_t* damage1, uint32_t* damage2)
{
	if (!g_edd.empty())
	{
		event_damage_data& edd = g_edd.back();
		if (edd.change)
		{
			edd.change = false;
			float d = jass::from_real(edd.new_amount);
			uint32_t new_damage1 = jass::to_real(d);
			uint32_t new_damage2 = jass::to_real(-d);
			return base::fast_call<uint32_t>(RealUnitDamageDoneFunc, _this, _edx, &new_damage1, &new_damage2);
		}
	}

	return base::fast_call<uint32_t>(RealUnitDamageDoneFunc, _this, _edx, damage1, damage2);
}

uintptr_t RealUnitDamageFunc = 0;
uint32_t __fastcall FakeUnitDamageFunc(uint32_t _this, uint32_t _edx, uint32_t a2, war3_event_damage_data* ptr, uint32_t is_physical, uint32_t source_unit)
{
	if ((uintptr_t)FakeUnitDamageDoneFunc != *(uintptr_t*)(*(uint32_t*)_this + 296))
	{
		RealUnitDamageDoneFunc = base::hook::replace_pointer(*(uint32_t*)_this + 296, (uintptr_t)FakeUnitDamageDoneFunc);
	}

	g_edd.push_back(event_damage_data(is_physical, ptr));
	uint32_t retval = base::fast_call<uint32_t>(RealUnitDamageFunc, _this, _edx, a2, ptr, is_physical, source_unit);
	g_edd.pop_back();
	return retval;
}


enum EVENT_DAMAGE_DATA_TYPE
{
	EVENT_DAMAGE_DATA_VAILD  = 0,
	EVENT_DAMAGE_DATA_PHYSICAL,
	EVENT_DAMAGE_DATA_IS_ATTACK,
	EVENT_DAMAGE_DATA_IS_RANGED,
	EVENT_DAMAGE_DATA_DAMAGE_TYPE,
	EVENT_DAMAGE_DATA_WEAPON_TYPE,
	EVENT_DAMAGE_DATA_ATTACK_TYPE,
};

uint32_t __cdecl EXGetEventDamageData(uint32_t type)
{
	if (g_edd.empty())
	{
		return 0;
	}

	war3_event_damage_data* ptr = g_edd.back().data;

	switch (type)
	{
	case EVENT_DAMAGE_DATA_VAILD:
		return 1;
	case EVENT_DAMAGE_DATA_PHYSICAL:
		return g_edd.back().physical;
	case EVENT_DAMAGE_DATA_IS_ATTACK:
		return !!(ptr->flag & 0x100);
	case EVENT_DAMAGE_DATA_IS_RANGED:
		return !!(ptr->flag & 0x001);
	case EVENT_DAMAGE_DATA_DAMAGE_TYPE:
		//return ptr->damage_type;
		return g_edd.back().damage_type();
	case EVENT_DAMAGE_DATA_WEAPON_TYPE:
		return ptr->weapon_type;
	case EVENT_DAMAGE_DATA_ATTACK_TYPE:
		return ptr->attack_type;
	default:
		break;
	}

	return 0;
}

bool __cdecl EXSetEventDamage(uint32_t value)
{
	if (g_edd.empty())
	{
		return false;
	}

	event_damage_data& edd = g_edd.back();
	edd.change = true;
	edd.old_amount = edd.data->amount;
	edd.new_amount = *(uint32_t*)value;

	return true;
}

uint32_t** reference_copy_ptr(uint32_t** _this, uint32_t* a2) {
    if (_this[0] != a2) {
        if (_this[0])
            if (_this[0][1]-- == 1)
                base::this_call<void>(ReadMemory<uint32_t>(_this[0][0]), _this[0]);
        if (a2)
            a2[1]++;
        _this[0] = a2;
    }
    return _this;
}
void reference_free_ptr(uint32_t** _this) {
    if (_this[0]) {
        --_this[0][1];
        if (!_this[0][1])
            base::this_call<void>(ReadMemory<uint32_t>(ReadMemory<uint32_t>((uint32_t)_this[0])), _this[0]);
        _this[0] = 0;
    }
}
uintptr_t real_CUnit_RunDamagedEvent = 0;
uint32_t __fastcall fake_CUnit_RunDamagedEvent(uint32_t pUnit, uint32_t edx, float* amount, uint32_t pSrcUnit) {
    (void)edx;
    static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
    uint32_t pOwningPlayer = base::this_call<uint32_t>(CUnit_GetOwner, pUnit);
    static ExecutePlayerUnitEvent executePlayerUnitEvent = searchExecutePlayerUnitEvent();
    if (base::this_call<uint32_t>(executePlayerUnitEvent.pCObserver_IsEventRegistered, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGED)) {
        uint32_t typeId = 'pued'; // CPlayerUnitEventDataBase 没必要特地去创建自定义事件数据库 因为可以用GetTriggerEventId获取触发事件id 然后做判断
        uint32_t pAgileTypeData = 0;
        if (get_war3_searcher().get_version() >= version_127a) 
            pAgileTypeData = base::this_call<uint32_t>(executePlayerUnitEvent.pCreateAgileTypeDataByTypeId, typeId);
        else 
            pAgileTypeData = base::this_call<uint32_t>(executePlayerUnitEvent.inlined.pCreateAgileTypeDataByTypeIdFunc2, ReadMemory(executePlayerUnitEvent.inlined.pAgileTypeData) + 0xC, base::this_call<uint32_t>(executePlayerUnitEvent.inlined.pCreateAgileTypeDataByTypeIdFunc1, &typeId), &typeId);
        uint32_t agent[11];
        base::fast_call<uint32_t>(executePlayerUnitEvent.pInitAgent, agent, typeId, ReadMemory(pAgileTypeData + 0x70));
        agent[9] = (uint32_t) - 1;
        uint32_t pCAgentAbs = base::fast_call<uint32_t>(executePlayerUnitEvent.pCreateAgentAbs, agent, 1, 1);
        uint32_t* pPlayerUnitEventDataBase = 0;
        reference_copy_ptr(&pPlayerUnitEventDataBase, ReadMemory<uint32_t*>(pCAgentAbs + 0x54));
        // CPlayerUnitEventDataBase 只能修改 0x20 - 0x43 (36字节)
        if (amount)
            // damage amount
            WriteMemory<float>((uint32_t)pPlayerUnitEventDataBase + 0x28, *amount);
        if (pUnit) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pUnit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // trigger unit
                WriteMemory<uint32_t>((uint32_t)pPlayerUnitEventDataBase + 0x2C, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>((uint32_t)pPlayerUnitEventDataBase + 0x30, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        if (pSrcUnit) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // damage source 0x34 0x38
                WriteMemory<uint32_t>((uint32_t)pPlayerUnitEventDataBase + 0x34, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>((uint32_t)pPlayerUnitEventDataBase + 0x38, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        base::this_call<uint32_t>(executePlayerUnitEvent.pDispatchPlayerUnitEvent, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGED, pPlayerUnitEventDataBase);
        reference_free_ptr(&pPlayerUnitEventDataBase);
    }
    return base::this_call<uint32_t>(real_CUnit_RunDamagedEvent, pUnit, amount, pSrcUnit);
}

uint32_t EXTriggerRegisterPlayerUnitDamagedEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
    static auto s = searchTriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId();
    uint16_t p1 = ReadMemory<uint16_t>(s.p1);
    uint16_t p2 = ReadMemory<uint16_t>(s.p2);
    uint16_t p3 = ReadMemory<uint16_t>(s.p3);
    WriteMemoryEx<uint16_t>(s.p1, 0x9090);
    WriteMemoryEx<uint16_t>(s.p2, 0x9090);
    WriteMemoryEx<uint16_t>(s.p3, 0x9090);
    uint32_t hEvent = jass::call("TriggerRegisterPlayerUnitEvent", trigger, player, EVENT_PLAYER_UNIT_DAMAGED, NULL);
    WriteMemoryEx<uint16_t>(s.p1, p1);
    WriteMemoryEx<uint16_t>(s.p2, p2);
    WriteMemoryEx<uint16_t>(s.p3, p3);
    return hEvent;
}

uint32_t GetObjectByHash(uint32_t a, uint32_t b) {
    uint32_t obj = find_objectid_64(objectid_64(a, b));
    if (obj && !ReadMemory<uint32_t>(obj + 0x20))
        return ReadMemory<uint32_t>(obj + 0x54);
    else
        return 0;
}

uint32_t real_GetTriggerUnit;
uint32_t fake_GetTriggerUnit() {
    if (jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGED) {
        static auto getEventDataBase = searchGetEventDataBase();
        if (base::c_call<uint32_t>(getEventDataBase.IsValid)) {
            static auto createHandle = searchCreateHandle();
            return base::this_call<uint32_t>(
                createHandle.CreateOrGetHandleId,
                base::this_call<uint32_t>(
                    createHandle.GetDataNode, 
                    get_war3_searcher().get_gamestate()), 
                GetObjectByHash(
                    ReadMemory<uint32_t>(base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x2C),
                    ReadMemory<uint32_t>(base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x30)),
                0);
        }
    }
    return base::c_call<uint32_t>(real_GetTriggerUnit);
}

void InitializeEventDamageData() {
	RealUnitDamageFunc = base::hook::replace_pointer(getUnitDamageFunc(), (uintptr_t)FakeUnitDamageFunc);
	jass::japi_hook("GetEventDamage", &RealGetEventDamage, (uintptr_t)FakeGetEventDamage);
	jass::japi_add((uintptr_t)EXGetEventDamageData,                     "EXGetEventDamageData",                     "(I)I");
	jass::japi_add((uintptr_t)EXSetEventDamage,                         "EXSetEventDamage",                         "(R)B");

    // 玩家单位接受伤害事件
    real_CUnit_RunDamagedEvent = searchCUnit_RunDamagedEvent();
    base::hook::install(&real_CUnit_RunDamagedEvent, (uintptr_t)fake_CUnit_RunDamagedEvent);
    jass::japi_add((uintptr_t)EXTriggerRegisterPlayerUnitDamagedEvent,  "EXTriggerRegisterPlayerUnitDamagedEvent",  "(Htrigger;Hplayer;)Hevent;");
    jass::japi_hook("GetTriggerUnit", &real_GetTriggerUnit, (uintptr_t)fake_GetTriggerUnit);
}
}
