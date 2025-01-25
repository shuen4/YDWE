#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <warcraft3/version.h>
#include "init_util.h"

namespace warcraft3::japi {

	uintptr_t searchSmartPositionSetLocation() {
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

    uint32_t searchSetSpriteAnimationByIndex() {
        //=========================================
        // (1)
        //
        // push		"()V"
        // mov		edx, "SetUnitAnimationByIndex"
        // mov		ecx, [SetUnitAnimationByIndex函数的地址] <----
        // call		BindNative
        //=========================================
        uint32_t ptr = get_war3_searcher().search_string("SetUnitAnimationByIndex");
        ptr = *(uintptr_t*)(ptr + 0x05);

        //=========================================
        // (2)
        //  SetUnitAnimationByIndex:
		//    call		ConvertHandle
		//    ...
		//    call      CUnit::GetSprite
		//    ...
		//    call		CSprite_SetAnimation <---
        //=========================================
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        return convert_function(ptr);
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

	jass::jnothing_t __cdecl X_SetEffectSpeed(jass::jhandle_t effect, jass::jreal_t* pspeed)
	{
		uintptr_t obj = handle_to_object(effect);
		if (!obj) {
			return;
		}
		uintptr_t pSprite = *(uintptr_t*)(obj + 0x28);
        base::this_call_vf<void>(pSprite, 0x28, *pspeed);
	}

	void UpdateSpriteColor(uint32_t pSprite) {
		WriteMemory(pSprite + 0x13C, 0);
		WriteMemory(pSprite + 0x140, 0);
		uint32_t flag = ReadMemory(pSprite + 0x138);
		if (!(flag & 0b100000000000))
			WriteMemory(pSprite + 0x138, flag | 0b100000000000);
	}

	jass::jnothing_t __cdecl X_SetEffectColorRed(jass::jhandle_t effect, jass::jinteger_t red) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x14A, red & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl X_SetEffectColorGreen(jass::jhandle_t effect, jass::jinteger_t green) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x149, green & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl X_SetEffectColorBlue(jass::jhandle_t effect, jass::jinteger_t blue) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory<uint8_t>(pSprite + 0x148, blue & 0xFF);
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl X_SetEffectColor(jass::jhandle_t effect, jass::jinteger_t rgb) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		WriteMemory(pSprite + 0x148, (ReadMemory(pSprite + 0x148) & 0xFF000000) | (rgb & 0xFFFFFF));
		UpdateSpriteColor(pSprite);
	}

	jass::jnothing_t __cdecl X_SetEffectAlpha(jass::jhandle_t effect, jass::jinteger_t alpha) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return;

		base::this_call<void>(*(uint32_t*)(*(uint32_t*)pSprite + 0x34), pSprite, alpha & 0xFF);
	}

	jass::jinteger_t __cdecl X_GetEffectColorRed(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x14A);
	}

	jass::jinteger_t __cdecl X_GetEffectColorGreen(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x149);
	}

	jass::jinteger_t __cdecl X_GetEffectColorBlue(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x148);
	}

	jass::jinteger_t __cdecl X_GetEffectColor(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return (ReadMemory<uint32_t>(pSprite + 0x148) & 0xFFFFFF);
	}

	jass::jinteger_t __cdecl X_GetEffectAlpha(jass::jhandle_t effect) {
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return 0;
		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		if (!pSprite)
			return 0;

		return ReadMemory<uint8_t>(pSprite + 0x1B0);
	}

	jass::jnothing_t __cdecl X_SetEffectTeamColor(jass::jhandle_t effect, jass::jinteger_t/* 实际上并不是 handle */ playercolor) {
		static uint32_t SetSpriteTeamColor = searchSetSpriteTeamColor();
		uintptr_t obj = handle_to_object(effect);
		if (!obj)
			return;

		uint32_t pSprite = *(uint32_t*)(obj + 0x28);
		uint32_t pPreselectionUI = *(uint32_t*)(obj + 0x58);
		base::fast_call<void>(SetSpriteTeamColor, pSprite, pPreselectionUI, playercolor);
	}

	jass::jnothing_t __cdecl X_UpdateEffectSmartPosition(jass::jhandle_t effect) {
		static uint32_t SmartPosition_SetLocation = searchSmartPositionSetLocation();

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
			base::this_call<void>(SmartPosition_SetLocation, pSmartPos, &vec3);
	}

	// flag:
	//		none(set)			0
	//		queue				1 << 1
	//		RARITY_FREQUENT		1 << 4
	//		RARITY_RARE			1 << 5
	jass::jboolean_t __cdecl X_SetEffectAnimationEx(jass::jhandle_t effect, jass::jstring_t animName, jass::jinteger_t flag) {
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

		base::fast_call<double>(addr.SetSpriteAnimation, pSprite, AnimData[2], AnimData[1], flag);
		return jass::jtrue;
	}

	jass::jboolean_t __cdecl X_SetEffectAnimation(jass::jhandle_t effect, jass::jstring_t animName) {
		return X_SetEffectAnimationEx(effect, animName, 0);
	}

    // flag:
    //		none(set)			0
    //		queue				1 << 1
    //		RARITY_FREQUENT		1 << 4
    //		RARITY_RARE			1 << 5
    jass::jboolean_t __cdecl X_SetEffectAnimationByIndexEx(jass::jhandle_t effect, jass::jinteger_t index, jass::jinteger_t flag) {
        uintptr_t obj = handle_to_object(effect);
        if (!obj)
            return jass::jfalse;

        uint32_t pSprite = ReadMemory(obj + 0x28);
        if (!pSprite)
            return jass::jfalse;

        static uint32_t pSetSpriteAnimationByIndex = searchSetSpriteAnimationByIndex();
        base::fast_call<double>(pSetSpriteAnimationByIndex, pSprite, index, flag);
        return jass::jtrue;
    }

    jass::jboolean_t __cdecl X_SetEffectAnimationByIndex(jass::jhandle_t effect, jass::jinteger_t index) {
        return X_SetEffectAnimationByIndexEx(effect, index, 0);
    }

    jass::jnothing_t __cdecl X_HideEffect(jass::jhandle_t effect, jass::jboolean_t hide) {
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

    jass::jboolean_t __cdecl X_RemoveEffect(jass::jhandle_t effect) {
        uint32_t pEffect = handle_to_object(effect);
        if (!pEffect)
            return jass::jfalse;
        base::this_call_vf<void>(pEffect, 0x5C);
        return jass::jtrue;
    }

    jass::jboolean_t __cdecl X_RemoveEffectTimed(jass::jhandle_t effect, jass::jreal_t duration) {
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

    init(Effect) {
		jass::japi_add((uintptr_t)X_SetEffectSpeed,					"X_SetEffectTimeScale",			"(Heffect;R)V");
		jass::japi_add((uintptr_t)X_SetEffectColorRed,				"X_SetEffectColorRed",			"(Heffect;I)V");
		jass::japi_add((uintptr_t)X_SetEffectColorGreen,			"X_SetEffectColorGreen",		"(Heffect;I)V");
		jass::japi_add((uintptr_t)X_SetEffectColorBlue,				"X_SetEffectColorBlue",			"(Heffect;I)V");
		jass::japi_add((uintptr_t)X_SetEffectColor,					"X_SetEffectColor",				"(Heffect;I)V");
		jass::japi_add((uintptr_t)X_SetEffectAlpha,					"X_SetEffectAlpha",				"(Heffect;I)V");
		jass::japi_add((uintptr_t)X_GetEffectColorRed,				"X_GetEffectColorRed",			"(Heffect;)I");
		jass::japi_add((uintptr_t)X_GetEffectColorGreen,			"X_GetEffectColorGreen",		"(Heffect;)I");
		jass::japi_add((uintptr_t)X_GetEffectColorBlue,				"X_GetEffectColorBlue",			"(Heffect;)I");
		jass::japi_add((uintptr_t)X_GetEffectColor,					"X_GetEffectColor",				"(Heffect;)I");
		jass::japi_add((uintptr_t)X_GetEffectAlpha,					"X_GetEffectAlpha",				"(Heffect;)I");
		jass::japi_add((uintptr_t)X_SetEffectTeamColor,				"X_SetEffectTeamColor",			"(Heffect;Hplayercolor;)V");
		jass::japi_add((uintptr_t)X_UpdateEffectSmartPosition,		"X_UpdateEffectSmartPosition",	"(Heffect;)V");
		jass::japi_add((uintptr_t)X_SetEffectAnimation,				"X_SetEffectAnimation",			"(Heffect;S)B");
		jass::japi_add((uintptr_t)X_SetEffectAnimationEx,			"X_SetEffectAnimationEx",		"(Heffect;SI)B");
		jass::japi_add((uintptr_t)X_SetEffectAnimationByIndex,      "X_SetEffectAnimationByIndex",	"(Heffect; S)B");
		jass::japi_add((uintptr_t)X_SetEffectAnimationByIndexEx,    "X_SetEffectAnimationByIndexEx","(Heffect;SI)B");
        jass::japi_add((uintptr_t)X_HideEffect,                     "X_HideEffect",                 "(Heffect;B)V");
        jass::japi_add((uintptr_t)X_RemoveEffect,                   "X_RemoveEffect",               "(Heffect;)B");
        jass::japi_add((uintptr_t)X_RemoveEffectTimed,              "X_RemoveEffectTimed",          "(Heffect;R)B");
	}
}
