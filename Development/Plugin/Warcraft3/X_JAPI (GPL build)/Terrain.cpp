#include <base/hook/fp_call.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t searchGetTerrainZ() {
    uint32_t ptr;

    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "GetLocationZ"
    // mov      ecx, [GetLocationZ函数的地址]  <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("GetLocationZ");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  GetLocationZ:
    //    ...
    //    call      ConvertLocationHandle
    //    ...
    //    call      GetLocationZ               <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    return ptr;
}

uint32_t __cdecl X_GetTerrainZ(float* x, float* y) {
    static uint32_t GetTerrainZ = searchGetTerrainZ();

    float z = ((float)base::fast_call<double>(GetTerrainZ, -1, 0, *x, *y, 1));
    return *(uint32_t*)&z;
}

init(Terrain) {
    jass::japi_add((uint32_t)X_GetTerrainZ,        "X_GetTerrainZ",        "(RR)R");
}