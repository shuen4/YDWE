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
#define EVENT_PLAYER_UNIT_DAMAGING 315

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

struct ExecutePlayerUnitEvent {
    uint32_t pCObserver_IsEventRegistered;
    uint32_t pDispatchPlayerUnitEvent;
};
ExecutePlayerUnitEvent searchExecutePlayerUnitEvent() {
    ExecutePlayerUnitEvent ret{};
    war3_searcher& s = get_war3_searcher();

    for (uintptr_t ptr = s.search_int_in_text(524818 /* 事件ID */); ptr; ptr = s.search_int_in_text(524818, ptr + 1)) {
        if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
            ret.pCObserver_IsEventRegistered = convert_function(next_opcode(ptr, 0xE8, 5));
            ptr = s.search_int_in_text(524818, ptr + 1);
            if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
                ret.pDispatchPlayerUnitEvent = convert_function(next_opcode(ptr, 0xE8, 5));
                return ret;
            }
        }
    }

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

uintptr_t searchCPlayerWar3_Save()
{
    uint32_t ptr = get_vfn_ptr(".?AVCPlayerWar3@@");
    return ReadMemory(ptr + 0x38); // vftable + 0x38
}

uintptr_t searchCPlayerWar3_Load()
{
    uint32_t ptr = get_vfn_ptr(".?AVCPlayerWar3@@");
    return ReadMemory(ptr + 0x3C); // vftable + 0x3C
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
    if ((jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGED || jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGING) && base::c_call<uint32_t>(getEventDataBase.IsValid))
        return ReadMemory<uint32_t>(base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x28);

	return base::fast_call<uint32_t>(RealGetEventDamage);
}

uintptr_t RealUnitDamageFunc = 0;
uint32_t __fastcall FakeUnitDamageFunc(uint32_t _this, uint32_t _edx, uint32_t a2, war3_event_damage_data* ptr, uint32_t is_physical, uint32_t source_unit)
{
	g_edd.push_back(event_damage_data(is_physical, ptr));

    static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
    uint32_t pOwningPlayer = base::this_call<uint32_t>(CUnit_GetOwner, _this);
    static ExecutePlayerUnitEvent executePlayerUnitEvent = searchExecutePlayerUnitEvent();
    if (base::this_call<uint32_t>(executePlayerUnitEvent.pCObserver_IsEventRegistered, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGING)) {
        uint32_t pPlayerUnitEventDataBase = create_by_typeid('pued');
        // sizeof(CAgent/* base */) 0x20 (32字节)
        // sizeof(CPlayerUnitEventDataBase) 0x44 (68字节)
        // 0x20 ~ (0x44 - 1)
        if (ptr->amount)
            // damage amount
            WriteMemory(pPlayerUnitEventDataBase + 0x28, ptr->amount);
        if (_this) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)_this + 0xC), ReadMemory<uint32_t>((uint32_t)_this + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // trigger unit
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x2C, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x30, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        if (ptr->source_unit) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)ptr->source_unit + 0xC), ReadMemory<uint32_t>((uint32_t)ptr->source_unit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // damage source 0x34 0x38
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x34, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x38, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        base::this_call<uint32_t>(executePlayerUnitEvent.pDispatchPlayerUnitEvent, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGING, pPlayerUnitEventDataBase);
        reference_free_ptr((uint32_t**)&pPlayerUnitEventDataBase);
    }

    if (g_edd.back().change)
        ptr->amount = g_edd.back().new_amount; // 不用管 C6011:取消NULL指针引用

    g_edd.back() = event_damage_data(is_physical, ptr);

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

uintptr_t real_CUnit_RunDamagedEvent = 0;
uint32_t __fastcall fake_CUnit_RunDamagedEvent(uint32_t pUnit, uint32_t edx, float* amount, uint32_t pSrcUnit) {
    (void)edx;
    static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
    uint32_t pOwningPlayer = base::this_call<uint32_t>(CUnit_GetOwner, pUnit);
    static ExecutePlayerUnitEvent executePlayerUnitEvent = searchExecutePlayerUnitEvent();
    if (base::this_call<uint32_t>(executePlayerUnitEvent.pCObserver_IsEventRegistered, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGED)) {
        uint32_t pPlayerUnitEventDataBase = create_by_typeid('pued');
        // sizeof(CAgent/* base */) 0x20 (32字节)
        // sizeof(CPlayerUnitEventDataBase) 0x44 (68字节)
        // 0x20 ~ (0x44 - 1)
        if (amount)
            // damage amount
            WriteMemory<float>(pPlayerUnitEventDataBase + 0x28, *amount);
        if (pUnit) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pUnit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // trigger unit
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x2C, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x30, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        if (pSrcUnit) {
            uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                // damage source 0x34 0x38
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x34, ReadMemory<uint32_t>((uint32_t)pAgent + 0x14));
                WriteMemory<uint32_t>(pPlayerUnitEventDataBase + 0x38, ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }
        base::this_call<uint32_t>(executePlayerUnitEvent.pDispatchPlayerUnitEvent, pOwningPlayer, 524800 + EVENT_PLAYER_UNIT_DAMAGED, pPlayerUnitEventDataBase);
        reference_free_ptr((uint32_t**)&pPlayerUnitEventDataBase);
    }
    uint32_t ret = base::this_call<uint32_t>(real_CUnit_RunDamagedEvent, pUnit, amount, pSrcUnit);
    if (!g_edd.empty()) {
        event_damage_data& edd = g_edd.back();
        if (edd.change)
            *amount = *(float*)&edd.new_amount; // 不用管 C6011:取消NULL指针引用
    }
    return ret;
}

uint32_t __cdecl EXTriggerRegisterPlayerUnitDamagedEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
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

uint32_t __cdecl EXTriggerRegisterPlayerUnitDamagingEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
    static auto s = searchTriggerRegisterPlayerUnitEvent_JumpIfInvalidEventId();
    uint16_t p1 = ReadMemory<uint16_t>(s.p1);
    uint16_t p2 = ReadMemory<uint16_t>(s.p2);
    uint16_t p3 = ReadMemory<uint16_t>(s.p3);
    WriteMemoryEx<uint16_t>(s.p1, 0x9090);
    WriteMemoryEx<uint16_t>(s.p2, 0x9090);
    WriteMemoryEx<uint16_t>(s.p3, 0x9090);
    uint32_t hEvent = jass::call("TriggerRegisterPlayerUnitEvent", trigger, player, EVENT_PLAYER_UNIT_DAMAGING, NULL);
    WriteMemoryEx<uint16_t>(s.p1, p1);
    WriteMemoryEx<uint16_t>(s.p2, p2);
    WriteMemoryEx<uint16_t>(s.p3, p3);
    return hEvent;
}

uint32_t real_GetTriggerUnit;
uint32_t __cdecl fake_GetTriggerUnit() {
    if (jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGED || jass::call("GetTriggerEventId") == EVENT_PLAYER_UNIT_DAMAGING) {
        static auto getEventDataBase = searchGetEventDataBase();
        if (base::c_call<uint32_t>(getEventDataBase.IsValid)) {
            return create_handle(
                GetObjectByHash(
                    ReadMemory<uint32_t>(
                        base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x2C), 
                    ReadMemory<uint32_t>(base::c_call<uint32_t>(getEventDataBase.GetDataBase) + 0x30)
                )
            );
        }
    }
    return base::c_call<uint32_t>(real_GetTriggerUnit);
}

bool __cdecl EXSetEventAttackType(uint32_t type) {
    if (type > 6 || g_edd.empty())
        return false;
    g_edd.back().data->attack_type = type;
    return true;
}

bool __cdecl EXSetEventDamageType(uint32_t type) {
    if (type > 26 || g_edd.empty() || type == 1 || type == 2 || type == 3 || type == 6 || type == 7)
        return false;
    g_edd.back().data->damage_type = 1 << type;
    return true;
}

bool __cdecl EXSetEventWeaponType(uint32_t type) {
    if (type > 23 || g_edd.empty())
        return false;
    g_edd.back().data->weapon_type = type;
    return true;
}

uint32_t real_CPlayerWar3_Save;
void __fastcall fake_CPlayerWar3_Save(uint32_t _this, uint32_t edx, uint32_t pSaveGame) {
    edx;
    base::this_call_vf<void>(_this, 0x50, pSaveGame, 524800 + EVENT_PLAYER_UNIT_DAMAGED, 1024);
    base::this_call_vf<void>(_this, 0x50, pSaveGame, 524800 + EVENT_PLAYER_UNIT_DAMAGING, 1024);
    base::this_call<void>(real_CPlayerWar3_Save, _this, pSaveGame);
}

uint32_t real_CPlayerWar3_Load;
void __fastcall fake_CPlayerWar3_Load(uint32_t _this, uint32_t edx, uint32_t pSaveGame) {
    edx;
    base::this_call_vf<void>(_this, 0x54, pSaveGame, 524800 + EVENT_PLAYER_UNIT_DAMAGED, 1024);
    base::this_call_vf<void>(_this, 0x54, pSaveGame, 524800 + EVENT_PLAYER_UNIT_DAMAGING, 1024);
    base::this_call<void>(real_CPlayerWar3_Load, _this, pSaveGame);
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

    // 玩家单位接受伤害事件 (计算护甲前)
    jass::japi_add((uintptr_t)EXTriggerRegisterPlayerUnitDamagingEvent, "EXTriggerRegisterPlayerUnitDamagingEvent", "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uintptr_t)EXSetEventAttackType,                     "EXSetEventAttackType",                     "(Hattacktype;)B");
    jass::japi_add((uintptr_t)EXSetEventDamageType,                     "EXSetEventDamageType",                     "(Hdamagetype;)B");
    jass::japi_add((uintptr_t)EXSetEventWeaponType,                     "EXSetEventWeaponType",                     "(Hweapontype;)B");

    // 添加 JAPI的玩家单位事件 进保存的游戏
    real_CPlayerWar3_Save = searchCPlayerWar3_Save();
    base::hook::install(&real_CPlayerWar3_Save, (uintptr_t)fake_CPlayerWar3_Save);
    real_CPlayerWar3_Load = searchCPlayerWar3_Load();
    base::hook::install(&real_CPlayerWar3_Load, (uintptr_t)fake_CPlayerWar3_Load);
}
}
