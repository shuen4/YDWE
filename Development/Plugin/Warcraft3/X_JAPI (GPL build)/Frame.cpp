#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

struct SpriteFrameSetDefaultLight {
    uint32_t createLight;
    uint32_t setupLight3; // 懒得研究函数动作
    uint32_t spriteFrameSetLight;
};
SpriteFrameSetDefaultLight searchCreateLight() {
    SpriteFrameSetDefaultLight ret;
    uint32_t ptr = get_war3_searcher().search_string("CinematicDialogueText");
    ptr += 4;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.createLight = convert_function(ptr);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // setupLight1
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // setupLight2
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // setupLight1
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // setupLight2
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // setupLight3
    ret.setupLight3 = convert_function(ptr);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ret.spriteFrameSetLight = convert_function(ptr);
    return ret;
}

bool isSpriteFrame(uint32_t pSpriteFrame) {
    static uint32_t CSpriteFrame_vtable = get_vfn_ptr(".?AVCSpriteFrame@@");
    return !IsBadReadPtr((void*)pSpriteFrame, 4) && ReadMemory(pSpriteFrame) != CSpriteFrame_vtable;
}

// 因为懒得研究函数 所以随便起名称
// setupLight1 / setupLight2 通用
// 1 只有一个数值 2 有三个
void setupLight(uint32_t pLight, uint32_t index, float* value) {
    int unk = ReadMemory(ReadMemory(pLight + 16) + 4 * index);
    WriteMemory(unk + 0x10, 0);
    WriteMemory(unk + 0x14, 0);
    WriteMemory(unk + 0x18, 0);
    base::this_call_vf<void>(unk, 0xC, value);
}

uint32_t __cdecl X_SpriteFrameSetDefaultLight(uint32_t pSpriteFrame) {
    if (!isSpriteFrame(pSpriteFrame))
        return false;
    static auto addr = searchCreateLight();
    uint32_t pLight = base::c_call<uint32_t>(addr.createLight);
    float unk[3];
    unk[0] = 0.330000013113022f;
    setupLight(pLight, 2, unk); // setupLight1
    unk[0] = 1.f;
    unk[1] = 1.f;
    unk[2] = 1.f;
    setupLight(pLight, 0, unk); // setupLight2
    unk[0] = 1.f;
    setupLight(pLight, 3, unk); // setupLight1
    unk[0] = 1.f;
    unk[1] = 1.f;
    unk[2] = 1.f;
    setupLight(pLight, 1, unk); // setupLight2
    unk[0] = -1.f;
    unk[1] = 0.800000011920929f;
    unk[2] = -1.f;
    base::fast_call<void>(addr.setupLight3, pLight, 4, unk, 0);
    base::this_call<void>(addr.spriteFrameSetLight, pSpriteFrame, pLight, 0);
    reference_free_ptr((uint32_t**)&pLight, 4);
    return true;
}

init(Frame) {
    jass::japi_add((uint32_t)X_SpriteFrameSetDefaultLight, "X_SpriteFrameSetDefaultLight", "(I)B");
}