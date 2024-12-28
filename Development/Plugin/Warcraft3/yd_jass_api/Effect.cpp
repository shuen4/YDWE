#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <string>
#include <warcraft3/version.h>

#define M_PI       3.14159265358979323846   // pi


template <typename T>
class qmatrix
{
public:
	typedef T value_type[3][3];

public:
	qmatrix(T* data)
		: data_(data)
	{ }

	qmatrix<T>& operator =(const value_type& r)
	{
		value_type& m = *(value_type*)data_;
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				m[i][j] = r[i][j];
			}
		}
		return *this;
	}

	qmatrix<T>& operator *=(const value_type& r)
	{
		value_type& m = *(value_type*)data_;
		value_type l;
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				l[i][j] = m[i][j];
			}
		}
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				T n(0);
				for (size_t k = 0; k < 3; ++k)
				{
					n += l[i][k] * r[k][j];
				}
				m[i][j] = n;
			}
		}
		return *this;
	}

private:
	T* data_;
};

namespace warcraft3::japi {

	uintptr_t searchSmartPosSetLocation() {
		uintptr_t ptr;

		//=========================================
		// (1)
		//
		// push		"()V"
		// mov		edx, "SetUnitX"
		// mov		ecx, [SetUnitX函数的地址]  <----
		// call		BindNative
		//=========================================
		ptr = get_war3_searcher().search_string("SetUnitX");
		ptr = *(uintptr_t*)(ptr + 0x05);

		//=========================================
		// (2)
		//  SetUnitX:
		//    ...
		//    call		ConvertHandle
		//    ...
		//    call		GetSmartPosition (vfn)
		//    ...
		//    call      SmartPosition::GetLocation
		//    ...
		//    call		GetSmartPosition (vfn)
		//    ...
		//    call		SmartPosition::SetLocation <----
		//    
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xFF, 2);
		ptr += 2;
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xFF, 2);
		ptr += 2;
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr = convert_function(ptr);

		return ptr;
	}

	uintptr_t searchSetSpriteTeamColor() {
		uintptr_t ptr;

		//=========================================
		// (1)
		//
		// push		"()V"
		// mov		edx, "SetUnitColor"
		// mov		ecx, [SetUnitColor函数的地址]  <----
		// call		BindNative
		//=========================================
		ptr = get_war3_searcher().search_string("SetUnitColor");
		ptr = *(uintptr_t*)(ptr + 0x05);

		//=========================================
		// (2)
		//  SetUnitColor:
		//    ...
		//    call		ConvertHandle
		//    ...
		//    call		GetCPreselectUI (vfn)
		//    ...
		//    call		SetUnitTeamColor           <----
		//    
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xFF, 2);
		ptr += 2;
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr = convert_function(ptr);

		//=========================================
		// (3)
		//  SetUnitTeamColor:
		//    ...
		//    call		SetSpriteTeamColor         <----
		//    
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr = convert_function(ptr);

		return ptr;
	}

	struct SetSpriteAnimationByNameAddress {
		uintptr_t GetAnimationDataFromJassString;
		uintptr_t SetSpriteAnimation;
	};

	SetSpriteAnimationByNameAddress searchSetSpriteAnimation() {
		SetSpriteAnimationByNameAddress ret{};
		uintptr_t ptr;

		//=========================================
		// (1)
		//
		// push		"()V"
		// mov		edx, "SetUnitAnimation"
		// mov		ecx, [SetUnitAnimation函数的地址] <----
		// call		BindNative
		//=========================================
		ptr = get_war3_searcher().search_string("SetUnitAnimation");
		ptr = *(uintptr_t*)(ptr + 0x05);

		//=========================================
		// (2)
		//  SetUnitAnimation:
		//    call		CUnit::SetAnimation		      <----
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr = convert_function(ptr);

		//=========================================
		// (3)
		//  CUnit::SetAnimation:
		//	  ...
		//    call		ConvertHandle
		//    ...
		//    call      CUnit::GetSprite
		//    ...
		//    call		GetAnimationDataFromJassString <---
		//    ...
		//    call      unk1
		//    ...
		//    call      unk2
		//    ...
		//    call      unk3
		//    ...
		//    call      unk4
		//    ...
		//    call      SetSpriteAnimation            <---
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xE8, 5);
		ret.GetAnimationDataFromJassString = convert_function(ptr);
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
		ret.SetSpriteAnimation = convert_function(ptr);

		return ret;
	}

    uintptr_t searchCAgentTimer_Start() {
        war3_searcher& s = get_war3_searcher();
        uintptr_t str = s.search_string_ptr("EffectDeathTime", sizeof("EffectDeathTime"));

        for (uintptr_t ptr = s.search_int_in_text(str); ptr; ptr = s.search_int_in_text(str, ptr + 1)) {
            uintptr_t func = s.current_function(ptr);
            if (ptr - func > 0x10) {
                ptr += 4;
                ptr = next_opcode(ptr, 0xE8, 5);
                ptr += 5;
                ptr = next_opcode(ptr, 0xE8, 5);
                return convert_function(ptr);
            }
        }

        return 0;
    }

	jass::jnothing_t __cdecl EXSetEffectXY(jass::jhandle_t effect, jass::jreal_t* px, jass::jreal_t* py)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC0) = jass::from_real(*px);
		*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC4) = jass::from_real(*py);
	}

	jass::jnothing_t __cdecl EXSetEffectZ(jass::jhandle_t effect, jass::jreal_t* pz)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC8) = jass::from_real(*pz);

	}

	jass::jreal_t __cdecl EXGetEffectX(jass::jhandle_t effect)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return 0;
		}
		return jass::to_real(*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC0));
	}

	jass::jreal_t __cdecl EXGetEffectY(jass::jhandle_t effect)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return 0;
		}
		return jass::to_real(*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC4));
	}

	jass::jreal_t __cdecl EXGetEffectZ(jass::jhandle_t effect)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return 0;
		}
		return jass::to_real(*(float*)(*(uintptr_t*)(obj + 0x28) + 0xC8));
	}

	jass::jnothing_t __cdecl EXSetEffectSize(jass::jhandle_t effect, jass::jreal_t* psize)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		*(float*)(*(uintptr_t*)(obj + 0x28) + 0xE8) = jass::from_real(*psize);
	}

	jass::jreal_t __cdecl EXGetEffectSize(jass::jhandle_t effect)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return 0;
		}
		return jass::to_real(*(float*)(*(uintptr_t*)(obj + 0x28) + 0xE8));
	}

	jass::jnothing_t __cdecl EXEffectMatRotateX(jass::jhandle_t effect, jass::jreal_t* pangle)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		float angle = jass::from_real(*pangle) * float(M_PI / 180.);
		qmatrix<float> mat((float*)(*(uintptr_t*)(obj + 0x28) + 0x108));
		qmatrix<float>::value_type m = {
			{ 1.f, 0.f, 0.f },
			{ 0.f, cosf(angle), sinf(angle) },
			{ 0.f, -sinf(angle), cosf(angle) },
		};
		mat *= m;
	}

	jass::jnothing_t __cdecl EXEffectMatRotateY(jass::jhandle_t effect, jass::jreal_t* pangle)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		float angle = jass::from_real(*pangle) * float(M_PI / 180.);
		qmatrix<float> mat((float*)(*(uintptr_t*)(obj + 0x28) + 0x108));
		qmatrix<float>::value_type m = {
			{ cosf(angle), 0.f, -sinf(angle) },
			{ 0.f, 1.f, 0.f },
			{ sinf(angle), 0.f, cosf(angle) },
		};
		mat *= m;
	}

	jass::jnothing_t __cdecl EXEffectMatRotateZ(jass::jhandle_t effect, jass::jreal_t* pangle)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		float angle = jass::from_real(*pangle) * float(M_PI / 180.);
		qmatrix<float> mat((float*)(*(uintptr_t*)(obj + 0x28) + 0x108));
		qmatrix<float>::value_type m = {
			{ cosf(angle), sinf(angle), 0.f },
			{ -sinf(angle), cosf(angle), 0.f },
			{ 0.f, 0.f, 1.f },
		};
		mat *= m;
	}

	jass::jnothing_t __cdecl EXEffectMatScale(jass::jhandle_t effect, jass::jreal_t* px, jass::jreal_t* py, jass::jreal_t* pz)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		float x = jass::from_real(*px);
		float y = jass::from_real(*py);
		float z = jass::from_real(*pz);
		qmatrix<float> mat((float*)(*(uintptr_t*)(obj + 0x28) + 0x108));
		qmatrix<float>::value_type m = {
			{ x, 0.f, 0.f },
			{ 0.f, y, 0.f },
			{ 0.f, 0.f, z },
		};
		mat *= m;
	}

	jass::jnothing_t __cdecl EXEffectMatReset(jass::jhandle_t effect)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		qmatrix<float> mat((float*)(*(uintptr_t*)(obj + 0x28) + 0x108));
		qmatrix<float>::value_type m = {
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 1.f },
		};
		mat = m;
	}

	jass::jnothing_t __cdecl EXSetEffectSpeed(jass::jhandle_t effect, jass::jreal_t* pspeed)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		uintptr_t eff = *(uintptr_t*)(obj + 0x28);
		base::this_call<void>(*(uintptr_t*)(*(uintptr_t*)eff + 0x28), eff, jass::from_real(*pspeed));
	}

	void UpdateSpriteColor(uint32_t pSprite) {
		WriteMemory(pSprite + 0x13C, 0);
		WriteMemory(pSprite + 0x140, 0);
		uint32_t flag = ReadMemory(pSprite + 0x138);
		if (!(flag & 0b100000000000))
			WriteMemory(pSprite + 0x138, flag | 0b100000000000);
	}

	jass::jnothing_t __cdecl EXSetEffectColorRed(jass::jhandle_t effect, jass::jinteger_t red) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x14A, red & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl EXSetEffectColorGreen(jass::jhandle_t effect, jass::jinteger_t green) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x149, green & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl EXSetEffectColorBlue(jass::jhandle_t effect, jass::jinteger_t blue) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x148, blue & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl EXSetEffectColor(jass::jhandle_t effect, jass::jinteger_t rgb) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory(pSprite + 0x148, (ReadMemory(pSprite + 0x148) & 0xFF000000) | (rgb & 0xFFFFFF));
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl EXSetEffectAlpha(jass::jhandle_t effect, jass::jinteger_t alpha) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		base::this_call<void>(*(uint32_t*)(*(uint32_t*)pSprite + 0x34), pSprite, alpha & 0xFF);
	}

	jass::jinteger_t __cdecl EXGetEffectColorRed(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x14A);
	}

	jass::jinteger_t __cdecl EXGetEffectColorGreen(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x149);
	}

	jass::jinteger_t __cdecl EXGetEffectColorBlue(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x148);
	}

	jass::jinteger_t __cdecl EXGetEffectColor(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return (ReadMemory<uint32_t>(pSprite + 0x148) & 0xFFFFFF);
	}

	jass::jinteger_t __cdecl EXGetEffectAlpha(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x1B0);
	}

	jass::jnothing_t __cdecl EXSetEffectTeamColor(jass::jhandle_t effect, jass::jinteger_t/* 实际上并不是 handle */ playercolor) {
		static uint32_t SetSpriteTeamColor = searchSetSpriteTeamColor();
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;

		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		uint32_t pPreselectionUI = *(uint32_t*)(obj + 0x58);
		base::fast_call<void>(SetSpriteTeamColor, pSprite, pPreselectionUI, playercolor);
	}

	jass::jnothing_t __cdecl EXUpdateEffectSmartPosition(jass::jhandle_t effect) {
		static uint32_t SmartPos_SetLocation = searchSmartPosSetLocation();

		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		struct {
			float x, y, z;
		} vec3;
		vec3.x = *(float*)(*(uintptr_t*)(obj + 0x28) + 0xC0);
		vec3.y = *(float*)(*(uintptr_t*)(obj + 0x28) + 0xC4);
		vec3.z = *(float*)(*(uintptr_t*)(obj + 0x28) + 0xC8);

		uint32_t pSmartPos = base::this_call<uint32_t>(*(uint32_t*)((*(uint32_t*)obj) + 0xB0), obj);
		if (pSmartPos)
			base::this_call<void>(SmartPos_SetLocation, pSmartPos, &vec3);
	}

	// flag:
	//		none(set)			0
	//		queue				1 << 1
	//		RARITY_FREQUENT		1 << 4
	//		RARITY_RARE			1 << 5
	jass::jboolean_t __cdecl EXSetEffectAnimationEx(jass::jhandle_t effect, jass::jstring_t animName, jass::jinteger_t flag) {
		static SetSpriteAnimationByNameAddress addr = searchSetSpriteAnimation();

		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return jass::jfalse;

		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return jass::jfalse;

		uint32_t AnimData[4] = {0, 0, 0, 0};

		if (get_war3_searcher().get_version() == version_126) {
			_asm {
				mov ecx, animName;
				lea edi, AnimData;
				call addr.GetAnimationDataFromJassString;
			}
		}
		else {
			// 其他版本未测试
			base::fast_call<void>(addr.GetAnimationDataFromJassString, animName, AnimData);
		}
		
		if (!AnimData[1])
			return jass::jfalse;

		base::fast_call<void>(addr.SetSpriteAnimation, pSprite, AnimData[2], AnimData[1], flag);
		return jass::jtrue;
	}

	jass::jboolean_t __cdecl EXSetEffectAnimation(jass::jhandle_t effect, jass::jstring_t animName) {
		return EXSetEffectAnimationEx(effect, animName, 0);
	}

    jass::jnothing_t __cdecl EXHideEffect(jass::jhandle_t effect, jass::jboolean_t hide) {
        uintptr_t obj = handle_to_object(effect);
        if (!obj)
            return;

        uint32_t flag = ReadMemory(obj + 0x20);
        if (hide)
            flag |= 0b1;
        else
            flag &= ~0b1;
        WriteMemory(obj + 0x20, flag);
    }

    jass::jboolean_t __cdecl EXRemoveEffect(jass::jhandle_t effect) {
        uint32_t pEffect = handle_to_object(effect);
        if (!pEffect)
            return jass::jfalse;
        base::this_call_vf<void>(pEffect, 0x5C);
        return jass::jtrue;
    }

    jass::jboolean_t __cdecl EXRemoveEffectTimed(jass::jhandle_t effect, jass::jreal_t duration) {
        uint32_t pEffect = handle_to_object(effect);
        if (!pEffect)
            return jass::jfalse;
        uint32_t dontRemove = 0;
        uint32_t pAgentAbsBase = find_objectid_64(objectid_64(ReadMemory(pEffect + 0xC), ReadMemory(pEffect + 0x10)));
        if (pAgentAbsBase &&                                // agent 存在
            ReadMemory(pAgentAbsBase + 0xC) == '+agl' &&    // 目标类型是 agent
            !ReadMemory(pAgentAbsBase + 0x20) &&            // 已被删除
            ReadMemory<int>(pAgentAbsBase + 0x14) < 0       // 无效 id
        ) // 上面的检查是复制魔兽的逻辑
            dontRemove = 1;

        static uint32_t pAgentTimer_Start = searchCAgentTimer_Start();
        base::this_call<void>(pAgentTimer_Start, pEffect + 0x2C, duration, 0xD01C4, pEffect, 0 /* 是否循环? */, dontRemove /* 回调的第二参数? */);
        return jass::jtrue;
    }

	void InitializeEffect()
	{
		jass::japi_add((uintptr_t)EXSetEffectXY,					"EXSetEffectXY",				"(Heffect;RR)V");
		jass::japi_add((uintptr_t)EXSetEffectZ,						"EXSetEffectZ",					"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXGetEffectX,						"EXGetEffectX",					"(Heffect;)R");
		jass::japi_add((uintptr_t)EXGetEffectY,						"EXGetEffectY",					"(Heffect;)R");
		jass::japi_add((uintptr_t)EXGetEffectZ,						"EXGetEffectZ",					"(Heffect;)R");
		jass::japi_add((uintptr_t)EXSetEffectSize,					"EXSetEffectSize",				"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXGetEffectSize,					"EXGetEffectSize",				"(Heffect;)R");
		jass::japi_add((uintptr_t)EXEffectMatRotateX,				"EXEffectMatRotateX",			"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXEffectMatRotateY,				"EXEffectMatRotateY",			"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXEffectMatRotateZ,				"EXEffectMatRotateZ",			"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXEffectMatScale,					"EXEffectMatScale",				"(Heffect;RRR)V");
		jass::japi_add((uintptr_t)EXEffectMatReset,					"EXEffectMatReset",				"(Heffect;)V");
		jass::japi_add((uintptr_t)EXSetEffectSpeed,					"EXSetEffectSpeed",				"(Heffect;R)V");
		jass::japi_add((uintptr_t)EXSetEffectColorRed,				"EXSetEffectColorRed",			"(Heffect;I)V");
		jass::japi_add((uintptr_t)EXSetEffectColorGreen,			"EXSetEffectColorGreen",		"(Heffect;I)V");
		jass::japi_add((uintptr_t)EXSetEffectColorBlue,				"EXSetEffectColorBlue",			"(Heffect;I)V");
		jass::japi_add((uintptr_t)EXSetEffectColor,					"EXSetEffectColor",				"(Heffect;I)V");
		jass::japi_add((uintptr_t)EXSetEffectAlpha,					"EXSetEffectAlpha",				"(Heffect;I)V");
		jass::japi_add((uintptr_t)EXGetEffectColorRed,				"EXGetEffectColorRed",			"(Heffect;)I");
		jass::japi_add((uintptr_t)EXGetEffectColorGreen,			"EXGetEffectColorGreen",		"(Heffect;)I");
		jass::japi_add((uintptr_t)EXGetEffectColorBlue,				"EXGetEffectColorBlue",			"(Heffect;)I");
		jass::japi_add((uintptr_t)EXGetEffectColor,					"EXGetEffectColor",				"(Heffect;)I");
		jass::japi_add((uintptr_t)EXGetEffectAlpha,					"EXGetEffectAlpha",				"(Heffect;)I");
		jass::japi_add((uintptr_t)EXSetEffectTeamColor,				"EXSetEffectTeamColor",			"(Heffect;Hplayercolor;)V");
		jass::japi_add((uintptr_t)EXUpdateEffectSmartPosition,		"EXUpdateEffectSmartPosition",	"(Heffect;)V");
		jass::japi_add((uintptr_t)EXSetEffectAnimation,				"EXSetEffectAnimation",			"(Heffect;S)B");
		jass::japi_add((uintptr_t)EXSetEffectAnimationEx,			"EXSetEffectAnimationEx",		"(Heffect;SI)B");
        jass::japi_add((uintptr_t)EXHideEffect,                     "EXHideEffect",                 "(Heffect;B)V");
        jass::japi_add((uintptr_t)EXRemoveEffect,                   "EXRemoveEffect",               "(Heffect;)B");
        jass::japi_add((uintptr_t)EXRemoveEffectTimed,              "EXRemoveEffectTimed",          "(Heffect;R)B");
	}
}
