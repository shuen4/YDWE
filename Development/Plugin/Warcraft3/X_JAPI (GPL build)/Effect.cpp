#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>

#include "CAgentTimer.h"
#include "util.h"

uint32_t searchSmartPositionSetLocation() {
    uint32_t ptr;

    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "SetUnitX"
    // mov      ecx, [SetUnitX函数的地址]  <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("SetUnitX");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  SetUnitX:
    //    ...
    //    call      ConvertHandle
    //    ...
    //    call      GetSmartPosition (vfn)
    //    ...
    //    call      SmartPosition::GetLocation
    //    ...
    //    call      GetSmartPosition (vfn)
    //    ...
    //    call      SmartPosition::SetLocation <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    return ptr;
}

uint32_t searchSetSpriteTeamColor() {
    uint32_t ptr;

    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "SetUnitColor"
    // mov      ecx, [SetUnitColor函数的地址]  <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("SetUnitColor");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  SetUnitColor:
    //    ...
    //    call      ConvertHandle
    //    ...
    //    call      GetCPreselectUI (vfn)
    //    ...
    //    call      SetUnitTeamColor           <----
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
    //    call      SetSpriteTeamColor         <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    return ptr;
}

struct SetSpriteAnimationByNameAddress {
    uint32_t GetAnimationDataFromJassString;
    uint32_t SetSpriteAnimation;
};

SetSpriteAnimationByNameAddress searchSetSpriteAnimation() {
    SetSpriteAnimationByNameAddress ret{};
    uint32_t ptr;

    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "SetUnitAnimation"
    // mov      ecx, [SetUnitAnimation函数的地址] <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("SetUnitAnimation");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  SetUnitAnimation:
    //    call      CUnit::SetAnimation           <----
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    //=========================================
    // (3)
    //  CUnit::SetAnimation:
    //    ...
    //    call      ConvertHandle
    //    ...
    //    call      CUnit::GetSprite
    //    ...
    //    call      GetAnimationDataFromJassString <---
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
    // push     "()V"
    // mov      edx, "SetUnitAnimationByIndex"
    // mov      ecx, [SetUnitAnimationByIndex函数的地址] <----
    // call     BindNative
    //=========================================
    uint32_t ptr = get_war3_searcher().search_string("SetUnitAnimationByIndex");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  SetUnitAnimationByIndex:
    //    call      ConvertHandle
    //    ...
    //    call      CUnit::GetSprite
    //    ...
    //    call      CSprite_SetAnimation <---
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

uint32_t __cdecl X_SetEffectTimeScale(uint32_t effect, float* pspeed) {
    uint32_t obj = handle_to_object(effect);
    if (!obj) {
        return false;
    }
    uint32_t pSprite = *(uint32_t*)(obj + 0x28);
    base::this_call_vf<void>(pSprite, 0x28, *pspeed);
    return true;
}

void UpdateSpriteColor(uint32_t pSprite) {
    WriteMemory(pSprite + 0x13C, 0);
    WriteMemory(pSprite + 0x140, 0);
    uint32_t flag = ReadMemory(pSprite + 0x138);
    if (!(flag & 0b100000000000))
        WriteMemory(pSprite + 0x138, flag | 0b100000000000);
}

uint32_t __cdecl X_SetEffectColorRed(uint32_t effect, uint32_t red) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    WriteMemory<uint8_t>(pSprite + 0x14A, red & 0xFF);
    UpdateSpriteColor(pSprite);
    return true;
}

uint32_t __cdecl X_SetEffectColorGreen(uint32_t effect, uint32_t green) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    WriteMemory<uint8_t>(pSprite + 0x149, green & 0xFF);
    UpdateSpriteColor(pSprite);
    return true;
}

uint32_t __cdecl X_SetEffectColorBlue(uint32_t effect, uint32_t blue) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    WriteMemory<uint8_t>(pSprite + 0x148, blue & 0xFF);
    UpdateSpriteColor(pSprite);
    return true;
}

uint32_t __cdecl X_SetEffectColor(uint32_t effect, uint32_t rgb) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    WriteMemory(pSprite + 0x148, (ReadMemory(pSprite + 0x148) & 0xFF000000) | (rgb & 0xFFFFFF));
    UpdateSpriteColor(pSprite);
    return true;
}

uint32_t __cdecl X_SetEffectAlpha(uint32_t effect, uint32_t alpha) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    base::this_call_vf<void>(pSprite, 0x34, alpha & 0xFF);
    return true;
}

uint32_t __cdecl X_GetEffectColorRed(uint32_t effect) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return 0xFF;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return 0xFF;

    return ReadMemory<uint8_t>(pSprite + 0x14A);
}

uint32_t __cdecl X_GetEffectColorGreen(uint32_t effect) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return 0xFF;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return 0xFF;

    return ReadMemory<uint8_t>(pSprite + 0x149);
}

uint32_t __cdecl X_GetEffectColorBlue(uint32_t effect) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return 0xFF;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return 0xFF;

    return ReadMemory<uint8_t>(pSprite + 0x148);
}

uint32_t __cdecl X_GetEffectColor(uint32_t effect) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return 0xFF;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return 0xFF;

    return (ReadMemory<uint32_t>(pSprite + 0x148) & 0xFFFFFF);
}

uint32_t __cdecl X_GetEffectAlpha(uint32_t effect) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return 0xFF;
    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return 0xFF;

    return ReadMemory<uint8_t>(pSprite + 0x1B0);
}

uint32_t __cdecl X_SetEffectTeamColor(uint32_t effect, uint32_t/* 实际上并不是 handle */ playercolor) {
    static uint32_t SetSpriteTeamColor = searchSetSpriteTeamColor();
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;

    uint32_t pSprite = ReadMemory(obj + 0x28);
    uint32_t pPreselectionUI = ReadMemory(obj + 0x58);
    base::fast_call<void>(SetSpriteTeamColor, pSprite, pPreselectionUI, playercolor);
    return true;
}

uint32_t __cdecl X_UpdateEffectSmartPosition(uint32_t effect) {
    static uint32_t SmartPosition_SetLocation = searchSmartPositionSetLocation();

    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;
    struct {
        float x, y, z;
    } vec3;
    vec3.x = *(float*)(*(uint32_t*)(obj + 0x28) + 0xC0);
    vec3.y = *(float*)(*(uint32_t*)(obj + 0x28) + 0xC4);
    vec3.z = *(float*)(*(uint32_t*)(obj + 0x28) + 0xC8);

    uint32_t pSmartPos = base::this_call_vf<uint32_t>(obj, 0xB0);
    if (!pSmartPos)
        return false;
    base::this_call<void>(SmartPosition_SetLocation, pSmartPos, &vec3);
    return true;
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetEffectAnimationEx(uint32_t effect, uint32_t animName, uint32_t flag) {
    static SetSpriteAnimationByNameAddress addr = searchSetSpriteAnimation();

    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;

    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

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
        return false;

    base::fast_call<double>(addr.SetSpriteAnimation, pSprite, AnimData[2], AnimData[1], flag);
    return true;
}

uint32_t __cdecl X_SetEffectAnimation(uint32_t effect, uint32_t animName) {
    return X_SetEffectAnimationEx(effect, animName, 0);
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetEffectAnimationByIndexEx(uint32_t effect, uint32_t index, uint32_t flag) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;

    uint32_t pSprite = ReadMemory(obj + 0x28);
    if (!pSprite)
        return false;

    static uint32_t pSetSpriteAnimationByIndex = searchSetSpriteAnimationByIndex();
    base::fast_call<double>(pSetSpriteAnimationByIndex, pSprite, index, flag);
    return true;
}

uint32_t __cdecl X_SetEffectAnimationByIndex(uint32_t effect, uint32_t index) {
    return X_SetEffectAnimationByIndexEx(effect, index, 0);
}

uint32_t __cdecl X_HideEffect(uint32_t effect, uint32_t hide) {
    uint32_t obj = handle_to_object(effect);
    if (!obj)
        return false;

    uint32_t flag = ReadMemory(obj + 0x20);
    if (hide)
        flag |= 0b1;
    else
        flag &= ~0b1;
    WriteMemory(obj + 0x20, flag);
    return true;
}

uint32_t __cdecl X_RemoveEffect(uint32_t effect) {
    uint32_t pEffect = handle_to_object(effect);
    if (!pEffect)
        return false;
    CAgentTimer_Stop(pEffect + 0x2C);
    base::this_call_vf<void>(pEffect, 0x5C);
    return true;
}

uint32_t __cdecl X_RemoveEffectTimed(uint32_t effect, float* duration) {
    uint32_t pEffect = handle_to_object(effect);
    if (!pEffect)
        return false;
    uint32_t dontRemove = 0;
    uint32_t pAgentAbsBase = find_objectid_64(objectid_64(ReadMemory(pEffect + 0xC), ReadMemory(pEffect + 0x10)));
    if (pAgentAbsBase &&                                // agent 存在
        ReadMemory(pAgentAbsBase + 0xC) == '+agl' &&    // 目标类型是 agent
        !ReadMemory(pAgentAbsBase + 0x20) &&            // 已被删除
        ReadMemory<int>(pAgentAbsBase + 0x14) < 0       // 无效 id
    ) // 上面的检查是复制魔兽的逻辑
        dontRemove = 1;

    CAgentTimer_Start(pEffect + 0x2C, duration, 0xD01C4, pEffect, 0, dontRemove);
    return true;
}

init(Effect) {
    jass::japi_add((uint32_t)X_SetEffectTimeScale,             "X_SetEffectTimeScale",         "(Heffect;R)B");
    jass::japi_add((uint32_t)X_SetEffectColorRed,              "X_SetEffectColorRed",          "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColorGreen,            "X_SetEffectColorGreen",        "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColorBlue,             "X_SetEffectColorBlue",         "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColor,                 "X_SetEffectColor",             "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectAlpha,                 "X_SetEffectAlpha",             "(Heffect;I)B");
    jass::japi_add((uint32_t)X_GetEffectColorRed,              "X_GetEffectColorRed",          "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColorGreen,            "X_GetEffectColorGreen",        "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColorBlue,             "X_GetEffectColorBlue",         "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColor,                 "X_GetEffectColor",             "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectAlpha,                 "X_GetEffectAlpha",             "(Heffect;)I");
    jass::japi_add((uint32_t)X_SetEffectTeamColor,             "X_SetEffectTeamColor",         "(Heffect;Hplayercolor;)B");
    jass::japi_add((uint32_t)X_UpdateEffectSmartPosition,      "X_UpdateEffectSmartPosition",  "(Heffect;)B");
    jass::japi_add((uint32_t)X_SetEffectAnimation,             "X_SetEffectAnimation",         "(Heffect;S)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationEx,           "X_SetEffectAnimationEx",       "(Heffect;SI)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationByIndex,      "X_SetEffectAnimationByIndex",  "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationByIndexEx,    "X_SetEffectAnimationByIndexEx","(Heffect;II)B");
    jass::japi_add((uint32_t)X_HideEffect,                     "X_HideEffect",                 "(Heffect;B)B");
    jass::japi_add((uint32_t)X_RemoveEffect,                   "X_RemoveEffect",               "(Heffect;)B");
    jass::japi_add((uint32_t)X_RemoveEffectTimed,              "X_RemoveEffectTimed",          "(Heffect;R)B");
}