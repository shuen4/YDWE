#include <warcraft3/event.h>
#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <base/hook/replace_pointer.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <deque>

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
uintptr_t searchRunUnitDamagedEventTrigger() {
    war3_searcher& s = get_war3_searcher();

    for (uintptr_t ptr = s.search_int_in_text(524852 /* 事件ID */); ptr; ptr = s.search_int_in_text(524852, ptr + 1)) {
        if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80234
            return s.current_function(ptr);
        }
    }

    return 0;
}

uintptr_t getUnitDamageFunc()
{
	static uintptr_t s_result = searchUnitDamageFunc();
	return s_result;
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

	uint32_t damage_type()
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

struct UnitDamagedEvent {
    uint32_t pUnit;
    float amount;
    uint32_t pSrcUnit;
    UnitDamagedEvent(uint32_t pUnit, float amount, uint32_t pSrcUnit) :pUnit(pUnit), amount(amount), pSrcUnit(pSrcUnit) {}
};
std::deque<UnitDamagedEvent> unitDamagedEvent;

std::vector<jass::jhandle_t> unitDamagedTriggers;
bool __cdecl EXTriggerRegisterAnyUnitDamagedEvent(jass::jhandle_t trig) {
    for (auto i = unitDamagedTriggers.begin(); i != unitDamagedTriggers.end(); i++)
        if (*i == trig)
            return false;
    unitDamagedTriggers.push_back(trig);
    return true;
}

uintptr_t real_RunUnitDamagedEventTrigger = 0;
uint32_t __fastcall fake_RunUnitDamagedEventTrigger(uint32_t pUnit, uint32_t edx, float* amount, uint32_t pSrcUnit) {
    unitDamagedEvent.push_back(UnitDamagedEvent(pUnit, *amount, pSrcUnit));
    auto unitDamagedTriggers_copy = unitDamagedTriggers;
    for (auto i = unitDamagedTriggers_copy.begin(); i != unitDamagedTriggers_copy.end(); i++)
        if (jass::call("IsTriggerEnabled", *i))
            if (jass::call("TriggerEvaluate", *i))
                jass::call("TriggerExecute", *i);
    unitDamagedEvent.pop_back();

    return base::fast_call<uint32_t>(real_RunUnitDamagedEventTrigger, pUnit, edx, amount, pSrcUnit);
}

uint32_t __cdecl EXGetEventDamage() {
    // 不在任意单位接受伤害事件
    if (unitDamagedEvent.empty())
        return 0;

    // g_edd 和 unitDamagedEvent 的数量调用时应始终相同
    if (g_edd.size() != unitDamagedEvent.size())
        return jass::to_real(unitDamagedEvent.back().amount);

    event_damage_data& edd = g_edd.back();

    // 伤害已更改
    if (edd.change)
        return edd.new_amount;

    return jass::to_real(unitDamagedEvent.back().amount);
}

uint32_t __cdecl EXGetEventDamageTarget() {
    return unitDamagedEvent.empty() ? 0 : object_to_handle(unitDamagedEvent.back().pUnit);
}

uint32_t __cdecl EXGetEventDamageSource() {
    return unitDamagedEvent.empty() ? 0 : object_to_handle(unitDamagedEvent.back().pSrcUnit);
}

void __cdecl EXTriggerRemoveAnyUnitDamagedEvent(jass::jhandle_t trig) {
    unitDamagedTriggers.erase(std::remove(unitDamagedTriggers.begin(), unitDamagedTriggers.end(), trig), unitDamagedTriggers.end());
}

uintptr_t real_DestroyTrigger = 0;
void __cdecl fake_DestroyTrigger(jass::jhandle_t trig) {
    EXTriggerRemoveAnyUnitDamagedEvent(trig);
    base::c_call<void>(real_DestroyTrigger, trig);
}

void InitializeEventDamageData()
{
	RealUnitDamageFunc = base::hook::replace_pointer(getUnitDamageFunc(), (uintptr_t)FakeUnitDamageFunc);
	jass::japi_hook("GetEventDamage", &RealGetEventDamage, (uintptr_t)FakeGetEventDamage);
	jass::japi_add((uintptr_t)EXGetEventDamageData,                     "EXGetEventDamageData",                 "(I)I");
	jass::japi_add((uintptr_t)EXSetEventDamage,                         "EXSetEventDamage",                     "(R)B");

    // 任意单位接受伤害事件
    real_RunUnitDamagedEventTrigger = searchRunUnitDamagedEventTrigger();
    base::hook::install(&real_RunUnitDamagedEventTrigger, (uintptr_t)fake_RunUnitDamagedEventTrigger);
    jass::japi_hook("DestroyTrigger", &real_DestroyTrigger, (uintptr_t)fake_DestroyTrigger);
    jass::japi_add((uintptr_t)EXTriggerRegisterAnyUnitDamagedEvent,     "EXTriggerRegisterAnyUnitDamagedEvent", "(Htrigger;)B");
    jass::japi_add((uintptr_t)EXTriggerRemoveAnyUnitDamagedEvent,       "EXTriggerRemoveAnyUnitDamagedEvent",   "(Htrigger;)V");
    // 任意单位接受伤害事件 - 获取触发数据
    // 直接 hook 魔兽的 JAPI 可能会出现问题
    jass::japi_add((uintptr_t)EXGetEventDamage,                         "EXGetEventDamage",                     "()R");
    jass::japi_add((uintptr_t)EXGetEventDamageTarget,                   "EXGetEventDamageTarget",               "()Hunit;");
    jass::japi_add((uintptr_t)EXGetEventDamageSource,                   "EXGetEventDamageSource",               "()Hunit;");
    event_game_reset([]() {
        unitDamagedTriggers.clear();
    });
}
}
