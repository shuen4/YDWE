#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_GetTerrainZ(float* x, float* y) {
    float z = (float)FUNC::GetTerrainZ(-1, 0, *x, *y, 1);
    return *(uint32_t*)&z;
}

init_L(Terrain) {
    jass::japi_add((uint32_t)X_GetTerrainZ,        "X_GetTerrainZ",        "(RR)R");
}