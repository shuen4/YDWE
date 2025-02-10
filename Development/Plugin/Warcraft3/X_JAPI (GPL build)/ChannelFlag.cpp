#include <warcraft3/war3_searcher.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <vector>
#include "util.h"

uint32_t searchCAbilityChannel_GetFlag() {
    uint32_t ptr = get_vfn_ptr(".?AVCAbilityChannel@@");
    return ReadMemory(ptr + 0x30C); // vftable + 0x30C
}

uint32_t searchCAbility_GetDataC() {
    uint32_t ptr = get_vfn_ptr(".?AVCAbilityChannel@@");
    ptr = ReadMemory(ptr + 0x30C); // vftable + 0x30C
    ptr = next_opcode(ptr, 0xE8, 5); // GetDataB
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // float2int
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5); // GetDataC
    return convert_function(ptr);
}

float CAbility_GetDataC(uint32_t _this) {
    static uint32_t pAbility_GetDataC = searchCAbility_GetDataC();
    float outVar;
    base::this_call<void>(pAbility_GetDataC, _this, &outVar, /* ability level */ReadMemory(_this + 0x50));
    return outVar;
}

uint32_t real_CAbilityChannel_GetFlag = 0;
uint32_t __fastcall fake_CAbilityChannel_GetFlag(uint32_t _this) {
    uint32_t flag = base::this_call<uint32_t>(real_CAbilityChannel_GetFlag, _this);
    uint32_t dataC = (uint32_t)CAbility_GetDataC(_this);
    if (dataC & 1 << 5)
        flag |= 1 << 25;
    return flag;
}

init(ObjectEditor_Channel_ChannelFlag) {
    real_CAbilityChannel_GetFlag = searchCAbilityChannel_GetFlag();
    base::hook::install(&real_CAbilityChannel_GetFlag, (uint32_t)fake_CAbilityChannel_GetFlag);
}