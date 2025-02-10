#include <map>

#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/event.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

std::map<uint32_t, uint32_t> destructable_color;
uint32_t real_GetDestructableColorById;
uint32_t* __fastcall fake_GetDestructableColorById(uint32_t* out, uint32_t pDestructableID /* 原本是可破坏物类型, 换成 lea 了所以现在是指针 */) {
    uint32_t pDestructable = pDestructableID - 0x30;
    auto i = destructable_color.find(pDestructable);
    if (i == destructable_color.end())
        return base::fast_call<uint32_t*>(real_GetDestructableColorById, out, ReadMemory(pDestructableID));
    *out = i->second;
    return out;
}
void patchCDestructable_UpdateColor() {
    uint32_t ptr = ReadMemory(get_vfn_ptr(".?AVCDestructable@@") + 0x104);
    ptr = next_opcode(ptr, 0x8B, 3);
    WriteMemoryEx<uint8_t>(ptr, 0x8D); // mov -> lea
    ptr = next_opcode(ptr, 0xE8, 5);
    real_GetDestructableColorById = convert_function(ptr);
    PatchCallRelative(ptr, fake_GetDestructableColorById);
}

uint32_t __cdecl X_GetDestructableColor(uint32_t destructable) {
    uint32_t pDestructable = handle_to_object(destructable);
    if (!pDestructable || !type_check_s(pDestructable, '+w3d'))
        return 0xFFFFFFFF;
    return *fake_GetDestructableColorById(&pDestructable/* 复用变量 */, pDestructable + 0x30);
}
uint32_t __cdecl X_SetDestructableColor(uint32_t destructable, uint32_t color) {
    uint32_t pDestructable = handle_to_object(destructable);
    if (!pDestructable || !type_check_s(pDestructable, '+w3d'))
        return false;
    destructable_color[pDestructable] = color;
    return true;
}
uint32_t __cdecl X_ResetDestructableColor(uint32_t destructable) {
    uint32_t pDestructable = handle_to_object(destructable);
    if (!pDestructable || !type_check_s(pDestructable, '+w3d'))
        return false;
    auto i = destructable_color.find(pDestructable);
    if (i == destructable_color.end())
        return false;
    destructable_color.erase(i);
    return true;
}

init(Destructable) {
    jass::japi_add((uint32_t)X_GetDestructableColor,       "X_GetDestructableColor",           "(Hdestructable;)I");
    jass::japi_add((uint32_t)X_SetDestructableColor,       "X_SetDestructableColor",           "(Hdestructable;I)B");
    jass::japi_add((uint32_t)X_ResetDestructableColor,     "X_ResetDestructableColor",         "(Hdestructable;)B");
    patchCDestructable_UpdateColor();
    event_agent_destructor([](uint32_t _this) {
        auto i = destructable_color.find(_this);
        if (i != destructable_color.end()) {
            destructable_color.erase(i);
        }
    });
}