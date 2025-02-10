#include <base/hook/fp_call.h>

#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t __cdecl X_BitAnd(uint32_t a, uint32_t b) {
    return a & b;
}

uint32_t __cdecl X_BitOr(uint32_t a, uint32_t b) {
    return a | b;
}

uint32_t __cdecl X_BitXor(uint32_t a, uint32_t b) {
    return a ^ b;
}

uint32_t __cdecl X_IsBitSet(uint32_t a, uint32_t b) {
    return (a & b) == b;
}

uint32_t __cdecl X_CastI2R(uint32_t value) {
    return value;
}

uint32_t __cdecl X_CastR2I(uint32_t* value) {
    return *value;
}

uint32_t __cdecl X_ConvertColor(uint32_t a, uint32_t r, uint32_t g, uint32_t b) {
    return ((b & 0xFF) | ((g & 0xFF) << 8) | ((r & 0xFF) << 16) | (a & 0xFF) << 24);
}

uint32_t __cdecl X_ReplaceColorR(uint32_t color, uint32_t r) {
    return (color & 0xFF00FFFF) | (r & 0xFF) << 16;
}

uint32_t __cdecl X_ReplaceColorG(uint32_t color, uint32_t g) {
    return (color & 0xFFFF00FF) | (g & 0xFF) << 8;
}

uint32_t __cdecl X_ReplaceColorB(uint32_t color, uint32_t b) {
    return (color & 0xFFFFFF00) | (b & 0xFF) << 0;
}

uint32_t __cdecl X_ReplaceColorA(uint32_t color, uint32_t a) {
    return (color & 0x00FFFFFF) | (a & 0xFF) << 24;
}

uint32_t __cdecl X_GetColorR(uint32_t color) {
    return color & 0x00FF0000;
}

uint32_t __cdecl X_GetColorG(uint32_t color) {
    return color & 0x0000FF00;
}

uint32_t __cdecl X_GetColorB(uint32_t color) {
    return color & 0x000000FF;
}

uint32_t __cdecl X_GetColorA(uint32_t color) {
    return color & 0xFF000000;
}

uint32_t searchDecodeTargetArgsString() {
    war3_searcher& s = get_war3_searcher();
    uint32_t ptr = s.search_string("Unknown Target Flag: \"%s\"", sizeof("Unknown Target Flag: \"%s\""));
    return s.current_function(ptr);
}

uint32_t __cdecl X_ConvertTargetArgsStringToFlags(uint32_t targs) {
    static uint32_t pDecodeTargetArgsString = searchDecodeTargetArgsString();
    return base::this_call<uint32_t>(pDecodeTargetArgsString, jass::from_string(targs));
}

init(Util) {
    jass::japi_add((uint32_t)X_BitAnd,                              "X_BitAnd",                             "(II)I");
    jass::japi_add((uint32_t)X_BitOr,                               "X_BitOr",                              "(II)I");
    jass::japi_add((uint32_t)X_BitXor,                              "X_BitXor",                             "(II)I");
    jass::japi_add((uint32_t)X_IsBitSet,                            "X_IsBitSet",                           "(II)B");
    jass::japi_add((uint32_t)X_CastI2R,                             "X_CastI2R",                            "(I)R");
    jass::japi_add((uint32_t)X_CastR2I,                             "X_CastR2I",                            "(R)I");
    jass::japi_add((uint32_t)X_ConvertColor,                        "X_ConvertColor",                       "(IIII)I");
    jass::japi_add((uint32_t)X_ReplaceColorR,                       "X_ReplaceColorR",                      "(II)I");
    jass::japi_add((uint32_t)X_ReplaceColorG,                       "X_ReplaceColorG",                      "(II)I");
    jass::japi_add((uint32_t)X_ReplaceColorB,                       "X_ReplaceColorB",                      "(II)I");
    jass::japi_add((uint32_t)X_ReplaceColorA,                       "X_ReplaceColorA",                      "(II)I");
    jass::japi_add((uint32_t)X_GetColorR,                           "X_GetColorR",                          "(I)I");
    jass::japi_add((uint32_t)X_GetColorG,                           "X_GetColorG",                          "(I)I");
    jass::japi_add((uint32_t)X_GetColorB,                           "X_GetColorB",                          "(I)I");
    jass::japi_add((uint32_t)X_GetColorA,                           "X_GetColorA",                          "(I)I");
    jass::japi_add((uint32_t)X_ConvertTargetArgsStringToFlags,      "X_ConvertTargetArgsStringToFlags",     "(S)I");
}