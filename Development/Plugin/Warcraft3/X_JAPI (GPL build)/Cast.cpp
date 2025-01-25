#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>
#include "init_util.h"

namespace warcraft3::japi {

    uint32_t __cdecl X_CastI2R(uint32_t value) {
        return value;
    }

    uint32_t __cdecl X_CastR2I(float* value) {
        return ReadMemory((uint32_t)value);
    }

    init(Cast) {
        jass::japi_add((uintptr_t)X_CastI2R, "X_CastI2R", "(I)R");
        jass::japi_add((uintptr_t)X_CastR2I, "X_CastR2I", "(R)I");
    }
}
