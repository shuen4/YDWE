#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <base/hook/replace_pointer.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <deque>
#include "TriggerEvent.h"

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

TriggerEvent::CPlayerUnitEventDataBase_vtable* JAPI_PlayerUnitDamageEventData_vtable;
class JAPI_PlayerUnitDamageEventData : public TriggerEvent::CPlayerUnitEventDataBase {
public:
    uint32_t amount;
    uint32_t GetTypeID() {
        return 'pumd';
    }
    const char* GetTypeName() {
        return "JAPI_PlayerUnitDamagedEventData";
    }
    uint32_t GetFilterUnit() {
        return 0; // 禁用
    }
    static void ctor(uint32_t _this) {
        ((CPlayerUnitEventDataBase*)_this)->ctor();
        ((JAPI_PlayerUnitDamageEventData*)_this)->vtable = JAPI_PlayerUnitDamageEventData_vtable;
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

    switch (TriggerEvent::GetTriggerEventId()) {
    case EVENT_PLAYER_UNIT_DAMAGED:
    case EVENT_PLAYER_UNIT_DAMAGING:
        if (JAPI_PlayerUnitDamageEventData* pEventData = (JAPI_PlayerUnitDamageEventData*)TriggerEvent::GetTriggerEventData('pumd'))
            return pEventData->amount;
        else
            return 0;
    default:
        return base::c_call<uint32_t>(RealGetEventDamage);
    }
}

uintptr_t real_GetEventDamageSource = 0;
uint32_t __cdecl fake_GetEventDamageSource() {
    switch (TriggerEvent::GetTriggerEventId()) {
    case EVENT_PLAYER_UNIT_DAMAGED:
    case EVENT_PLAYER_UNIT_DAMAGING:
        if (JAPI_PlayerUnitDamageEventData* pEventData = (JAPI_PlayerUnitDamageEventData*)TriggerEvent::GetTriggerEventData('pumd'))
            return create_handle(GetObjectByHash(pEventData->filterUnit.a, pEventData->filterUnit.b));
        else
            return 0;
    default:
        return base::c_call<uint32_t>(real_GetEventDamageSource);
    }
}

uintptr_t RealUnitDamageFunc = 0;
uint32_t __fastcall FakeUnitDamageFunc(uint32_t _this, uint32_t _edx, uint32_t a2, war3_event_damage_data* ptr, uint32_t is_physical, uint32_t source_unit)
{
	g_edd.push_back(event_damage_data(is_physical, ptr));

    static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
    uint32_t pOwningPlayer = base::this_call<uint32_t>(CUnit_GetOwner, _this);
    if (TriggerEvent::IsEventRegistered(pOwningPlayer, EVENT_PLAYER_UNIT_DAMAGING)) {
        JAPI_PlayerUnitDamageEventData* pPlayerUnitDamageEventData = (JAPI_PlayerUnitDamageEventData*)create_by_typeid('pumd');

        uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0xC), ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitDamageEventData->player = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)_this + 0xC), ReadMemory<uint32_t>((uint32_t)_this + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitDamageEventData->triggerUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        if (ptr->source_unit) {
            pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)ptr->source_unit + 0xC), ReadMemory<uint32_t>((uint32_t)ptr->source_unit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                pPlayerUnitDamageEventData->filterUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }

        pPlayerUnitDamageEventData->amount = ptr->amount;

        TriggerEvent::FirePlayerUnitEvent(pPlayerUnitDamageEventData, pOwningPlayer, EVENT_PLAYER_UNIT_DAMAGING);
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
uint32_t __fastcall fake_CUnit_RunDamagedEvent(uint32_t pUnit, uint32_t, float* amount, uint32_t pSrcUnit) {
    static uint32_t CUnit_GetOwner = searchCUnit_GetOwner();
    uint32_t pOwningPlayer = base::this_call<uint32_t>(CUnit_GetOwner, pUnit);
    if (TriggerEvent::IsEventRegistered(pOwningPlayer, EVENT_PLAYER_UNIT_DAMAGED)) {
        JAPI_PlayerUnitDamageEventData* pPlayerUnitDamageEventData = (JAPI_PlayerUnitDamageEventData*)create_by_typeid('pumd');

        uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0xC), ReadMemory<uint32_t>((uint32_t)pOwningPlayer + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitDamageEventData->player = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pUnit + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
            pPlayerUnitDamageEventData->triggerUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
        }

        if (pSrcUnit) {
            pAgent = find_objectid_64({ ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0xC), ReadMemory<uint32_t>((uint32_t)pSrcUnit + 0x10) });
            if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl') {
                pPlayerUnitDamageEventData->filterUnit = objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18));
            }
        }

        pPlayerUnitDamageEventData->amount = *(uint32_t*)amount;

        TriggerEvent::FirePlayerUnitEvent(pPlayerUnitDamageEventData, pOwningPlayer, EVENT_PLAYER_UNIT_DAMAGED);
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
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, EVENT_PLAYER_UNIT_DAMAGED, NULL);
}

uint32_t __cdecl EXTriggerRegisterPlayerUnitDamagingEvent(jass::jhandle_t trigger, jass::jhandle_t player) {
    return TriggerEvent::TriggerRegisterPlayerUnitEvent(trigger, player, EVENT_PLAYER_UNIT_DAMAGING, NULL);
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
    if (TriggerEvent::GetTriggerEventId() != EVENT_PLAYER_UNIT_DAMAGING || type > 23 || g_edd.empty())
        return false;
    g_edd.back().data->weapon_type = type;
    return true;
}

void InitializeEventDamageData() {
    JAPI_PlayerUnitDamageEventData_vtable = TriggerEvent::CPlayerUnitEventDataBase_vtable_instance.copy();
    WriteMemory((uint32_t)&JAPI_PlayerUnitDamageEventData_vtable->GetTypeID, &JAPI_PlayerUnitDamageEventData::GetTypeID);
    WriteMemory((uint32_t)&JAPI_PlayerUnitDamageEventData_vtable->GetTypeName, &JAPI_PlayerUnitDamageEventData::GetTypeName);
    WriteMemory((uint32_t)&JAPI_PlayerUnitDamageEventData_vtable->GetFilterUnit, &JAPI_PlayerUnitDamageEventData::GetFilterUnit);
    TriggerEvent::RegisterTriggerEventData('pumd', TriggerEvent::Type::CPlayerUnitEventDataBase, { EVENT_PLAYER_UNIT_DAMAGING, EVENT_PLAYER_UNIT_DAMAGED }, sizeof(JAPI_PlayerUnitDamageEventData), 0x10, JAPI_PlayerUnitDamageEventData::ctor);

	RealUnitDamageFunc = base::hook::replace_pointer(getUnitDamageFunc(), (uintptr_t)FakeUnitDamageFunc);
	jass::japi_hook("GetEventDamage", &RealGetEventDamage, (uintptr_t)FakeGetEventDamage);
	jass::japi_add((uintptr_t)EXGetEventDamageData,                     "EXGetEventDamageData",                     "(I)I");
	jass::japi_add((uintptr_t)EXSetEventDamage,                         "EXSetEventDamage",                         "(R)B");

    // 玩家单位接受伤害事件
    real_CUnit_RunDamagedEvent = searchCUnit_RunDamagedEvent();
    base::hook::install(&real_CUnit_RunDamagedEvent, (uintptr_t)fake_CUnit_RunDamagedEvent);
    jass::japi_hook("GetEventDamageSource", &real_GetEventDamageSource, (uintptr_t)fake_GetEventDamageSource);
    jass::japi_add((uintptr_t)EXTriggerRegisterPlayerUnitDamagedEvent,  "EXTriggerRegisterPlayerUnitDamagedEvent",  "(Htrigger;Hplayer;)Hevent;");

    // 玩家单位接受伤害事件 (计算护甲前)
    jass::japi_add((uintptr_t)EXTriggerRegisterPlayerUnitDamagingEvent, "EXTriggerRegisterPlayerUnitDamagingEvent", "(Htrigger;Hplayer;)Hevent;");
    jass::japi_add((uintptr_t)EXSetEventAttackType,                     "EXSetEventAttackType",                     "(Hattacktype;)B");
    jass::japi_add((uintptr_t)EXSetEventDamageType,                     "EXSetEventDamageType",                     "(Hdamagetype;)B");
    jass::japi_add((uintptr_t)EXSetEventWeaponType,                     "EXSetEventWeaponType",                     "(Hweapontype;)B");
}
}
