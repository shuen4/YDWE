#include <map>

#include <warcraft3/event.h>
#include <warcraft3/jass/hook.h>

#include "util.h"

std::map<CDestructable*, uint32_t> destructable_color;
uint32_t* (__fastcall* real_GetDestructableColorById)(uint32_t* output, uint32_t destructableID);
uint32_t* __fastcall fake_GetDestructableColorById(uint32_t* out, uint32_t pDestructableID /* 原本是可破坏物类型, 换成 lea 了所以现在是指针 */) {
    CDestructable* pDestructable = (CDestructable*)(pDestructableID - 0x30);
    auto i = destructable_color.find(pDestructable);
    if (i == destructable_color.end())
        return real_GetDestructableColorById(out, ReadMemory(pDestructableID));
    *out = i->second;
    return out;
}

uint32_t __cdecl X_GetDestructableColor(uint32_t destructable) {
    CDestructable* pDestructable = ConvertHandle<CDestructable>(destructable);
    if (!pDestructable)
        return 0xFFFFFFFF;
    return *fake_GetDestructableColorById((uint32_t*)&pDestructable/* 复用变量 */, (uint32_t)&pDestructable->ID);
}
uint32_t __cdecl X_SetDestructableColor(uint32_t destructable, uint32_t color) {
    CDestructable* pDestructable = ConvertHandle<CDestructable>(destructable);
    if (!pDestructable)
        return false;
    destructable_color[pDestructable] = color;
    return true;
}
uint32_t __cdecl X_ResetDestructableColor(uint32_t destructable) {
    CDestructable* pDestructable = ConvertHandle<CDestructable>(destructable);
    if (!pDestructable)
        return false;
    auto i = destructable_color.find(pDestructable);
    if (i == destructable_color.end())
        return false;
    destructable_color.erase(i);
    return true;
}

init_L(Destructable) {
    jass::japi_add((uint32_t)X_GetDestructableColor,       "X_GetDestructableColor",           "(Hdestructable;)I");
    jass::japi_add((uint32_t)X_SetDestructableColor,       "X_SetDestructableColor",           "(Hdestructable;I)B");
    jass::japi_add((uint32_t)X_ResetDestructableColor,     "X_ResetDestructableColor",         "(Hdestructable;)B");

    real_GetDestructableColorById = FUNC::GetDestructableColorById;
    WriteMemoryEx<uint8_t>(ASM::CDestructable_UpdateColor_mov_destructableID, 0x8D); // mov -> lea
    PatchCallRelative(ASM::CDestructable_UpdateColor_call_GetDestructableColorById, fake_GetDestructableColorById);

    event_agent_destructor([](uint32_t _this) {
        auto i = destructable_color.find((CDestructable*)_this);
        if (i != destructable_color.end()) {
            destructable_color.erase(i);
        }
    });
}