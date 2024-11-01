#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>

namespace warcraft3::japi {

    uint32_t EXCastI2R(uint32_t value) {
        return value;
    }

    uint32_t EXCastR2I(float* value) {
        return ReadMemory((uint32_t)value);
    }

    void InitializeCast()
    {
        jass::japi_add((uintptr_t)EXCastI2R, "EXCastI2R", "(I)R");
        jass::japi_add((uintptr_t)EXCastR2I, "EXCastR2I", "(R)I");
    }
}
