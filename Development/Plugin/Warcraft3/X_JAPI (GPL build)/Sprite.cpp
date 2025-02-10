#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>

#include "qmatrix.h"
#include "util.h"

uint32_t searchSetSpriteTeamColor();
struct SetSpriteAnimationByNameAddress {
    uint32_t GetAnimationDataFromJassString;
    uint32_t SetSpriteAnimation;
};
SetSpriteAnimationByNameAddress searchSetSpriteAnimation();
uint32_t searchSetSpriteAnimationByIndex();

uint32_t searchCSprite_SetReplacableTexture() {
    uint32_t ptr = searchSetSpriteTeamColor();
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}
uint32_t searchLoadTexture() {
    uint32_t ptr = get_war3_searcher().search_string("ui\\widgets\\escmenu\\human\\observer-icon.blp");
    ptr += 4;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

bool isSpriteFrame(uint32_t pSpriteFrame);

uint32_t __cdecl X_SpriteFrameGetSprite(uint32_t pSpriteFrame) {
    if (isSpriteFrame(pSpriteFrame) && ReadMemory(pSpriteFrame + 0x174))
        return ReadMemory(ReadMemory(pSpriteFrame + 0x178));
    else
        return 0;
}

uint32_t __cdecl X_War3ImageGetSprite(uint32_t handle) {
    uint32_t pObj = handle_to_object(handle);
    if (!pObj || !type_check_s(pObj, '+w3i'))
        return 0;
    return ReadMemory(pObj + 0x28);
}

enum class SpriteType : int {
    INVALID,
    MINI,
    UBER,
};
SpriteType GetSpriteType(uint32_t pSprite) {
    if (!pSprite || IsBadReadPtr((void*)pSprite, 4))
        return SpriteType::INVALID;

    static uint32_t pCSpriteMini_vtable = get_vfn_ptr(".?AVCSpriteMini_@@");
    // 子类型
    static uint32_t pTAllocatedHandleObjectLeaf_CSpriteMini_vtable = get_vfn_ptr(".?AV?$TAllocatedHandleObjectLeaf@VCSpriteMini_@@$0BAA@@@");
    if (ReadMemory(pSprite) == pCSpriteMini_vtable || ReadMemory(pSprite) == pTAllocatedHandleObjectLeaf_CSpriteMini_vtable)
        return SpriteType::MINI;

    static uint32_t pCSpriteUber_vtable = get_vfn_ptr(".?AVCSpriteUber_@@");
    // 同上
    static uint32_t pTAllocatedHandleObjectLeaf_CSpriteUber_vtable = get_vfn_ptr(".?AV?$TAllocatedHandleObjectLeaf@VCSpriteUber_@@$0IA@@@");
    if (ReadMemory(pSprite) == pCSpriteUber_vtable || ReadMemory(pSprite) == pTAllocatedHandleObjectLeaf_CSpriteUber_vtable)
        return SpriteType::UBER;

    return SpriteType::INVALID;
}
uint32_t GetOffsetBySpriteType(SpriteType type, uint32_t offsetMini, uint32_t offsetUber) {
    if (type == SpriteType::MINI)
        return offsetMini;
    else if (type == SpriteType::UBER)
        return offsetUber;
    return 0;
}

uint32_t __cdecl X_IsSpriteValid(uint32_t pSprite) {
    return GetSpriteType(pSprite) != SpriteType::INVALID;
}

SpriteType __cdecl X_GetSpriteType(uint32_t pSprite) {
    return GetSpriteType(pSprite);
}

#define setupOffset(offsetMini, offsetUber)                                                             \
uint32_t offset = GetOffsetBySpriteType(GetSpriteType(pSprite), offsetMini, offsetUber);            \
    if (!offset)                                                                                        \
        return false
#define checkValid()                                                                                    \
    if (GetSpriteType(pSprite) == SpriteType::INVALID)                                                  \
        return false

uint32_t __cdecl X_GetSpriteGeosetCount(uint32_t pSprite) {
    if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
        return ReadMemory(pModelComplex + 0xC);
    return 0;
}

uint32_t __cdecl X_SetSpriteX(uint32_t pSprite, float* x) {
    setupOffset(0x88, 0xC0);
    WriteMemory(pSprite + offset, *x);
    return true;
}

uint32_t __cdecl X_SetSpriteY(uint32_t pSprite, float* y) {
    setupOffset(0x8C, 0xC4);
    WriteMemory(pSprite + offset, *y);
    return true;
}

uint32_t __cdecl X_SetSpriteZ(uint32_t pSprite, float* z) {
    setupOffset(0x90, 0xC8);
    WriteMemory(pSprite + offset, *z);
    return true;
}

uint32_t __cdecl X_GetSpriteX(uint32_t pSprite) {
    setupOffset(0x88, 0xC0);
    return ReadMemory(pSprite + offset);
}

uint32_t __cdecl X_GetSpriteY(uint32_t pSprite) {
    setupOffset(0x8C, 0xC4);
    return ReadMemory(pSprite + offset);
}

uint32_t __cdecl X_GetSpriteZ(uint32_t pSprite) {
    setupOffset(0x90, 0xC8);
    return ReadMemory(pSprite + offset);
}

uint32_t __cdecl X_SetSpriteSize(uint32_t pSprite, float* size) {
    checkValid();
    base::this_call_vf<void>(pSprite, 0x24, *size);
    return true;
    /* 还是调用 vf 比较好吧
    setupOffset(0x94, 0xE8);
    WriteMemory(pSprite + offset, *size);
    return true;
    */
}

uint32_t __cdecl X_GetSpriteSize(uint32_t pSprite) {
    setupOffset(0x94, 0xE8);
    return ReadMemory(pSprite + offset);
}

uint32_t __cdecl X_SpriteMatrixRotateX(uint32_t pSprite, float* angle) {
    setupOffset(0x64, 0x108);
    float angle_radians = *angle * float(M_PI / 180.);
    qmatrix<float> mat((float*)(pSprite + offset));
    qmatrix<float>::value_type m = {
        { 1.f, 0.f, 0.f },
        { 0.f, cosf(angle_radians), sinf(angle_radians) },
        { 0.f, -sinf(angle_radians), cosf(angle_radians) },
    };
    mat *= m;
    return true;
}

uint32_t __cdecl X_SpriteMatrixRotateY(uint32_t pSprite, float* angle) {
    setupOffset(0x64, 0x108);
    float angle_radians = *angle * float(M_PI / 180.);
    qmatrix<float> mat((float*)(pSprite + offset));
    qmatrix<float>::value_type m = {
        { cosf(angle_radians), 0.f, -sinf(angle_radians) },
        { 0.f, 1.f, 0.f },
        { sinf(angle_radians), 0.f, cosf(angle_radians) },
    };
    mat *= m;
    return true;
}

uint32_t __cdecl X_SpriteMatrixRotateZ(uint32_t pSprite, float* angle) {
    setupOffset(0x64, 0x108);
    float angle_radians = *angle * float(M_PI / 180.);
    qmatrix<float> mat((float*)(pSprite + offset));
    qmatrix<float>::value_type m = {
        { cosf(angle_radians), sinf(angle_radians), 0.f },
        { -sinf(angle_radians), cosf(angle_radians), 0.f },
        { 0.f, 0.f, 1.f },
    };
    mat *= m;
    return true;
}

uint32_t __cdecl X_SpriteMatrixScale(uint32_t pSprite, float* x, float* y, float* z) {
    setupOffset(0x64, 0x108);
    qmatrix<float> mat((float*)(pSprite + offset));
    qmatrix<float>::value_type m = {
        { *x, 0.f, 0.f },
        { 0.f, *y, 0.f },
        { 0.f, 0.f, *z },
    };
    mat *= m;
    return true;
}

uint32_t __cdecl X_SpriteMatrixReset(uint32_t pSprite) {
    setupOffset(0x64, 0x108);
    qmatrix<float> mat((float*)(pSprite + offset));
    qmatrix<float>::value_type m = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f },
    };
    mat = m;
    return true;
}

uint32_t __cdecl X_SetSpriteTimeScale(uint32_t pSprite, float* timeScale) {
    checkValid();
    base::this_call_vf<void>(pSprite, 0x28, *timeScale);
    return true;
    /* 还是调用 vf 比较好吧
    setupOffset(0x48, 0x190);
    WriteMemory(pSprite + offset, *timeScale);
    return true;
    */
}

uint32_t __cdecl X_GetSpriteTimeScale(uint32_t pSprite) {
    setupOffset(0x48, 0x190);
    return ReadMemory(pSprite + offset);
}

uint32_t __cdecl X_SetSpriteColor(uint32_t pSprite, uint32_t color) {
    checkValid();
    base::this_call_vf<void>(pSprite, 0x30, color & 0xFFFFFF);
    return true;
}

uint32_t __cdecl X_SetSpriteAlpha(uint32_t pSprite, uint32_t alpha) {
    checkValid();
    base::this_call_vf<void>(pSprite, 0x34, alpha & 0xFF);
    return true;
}

uint32_t __cdecl X_GetSpriteColor(uint32_t pSprite) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC)) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                    return ReadMemory(pUnk + 0x4) & 0xFFFFFF;
    return 0xFF;
}

uint32_t __cdecl X_GetSpriteAlpha(uint32_t pSprite) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC)) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                    return (uint8_t)(ReadMemory<float>(pUnk + 0xC) * 255.f);
    return 0xFF;
}

uint32_t __cdecl X_SetSpriteGeosetColor(uint32_t pSprite, uint32_t index, uint32_t value) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20)) {
                    WriteMemory(pUnk + 0x4 + index * 0x10, ReadMemory<uint8_t>(pUnk + 0x7 + index * 0x10) | (value & 0xFFFFFF));
                    return true;
                }
    return false;
}

uint32_t __cdecl X_SetSpriteGeosetAlpha(uint32_t pSprite, uint32_t index, uint32_t value) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20)) {
                    WriteMemory(pUnk + 0xC + index * 0x10, (float)(value & 0xFF) / 255.f);
                    return true;
                }
    return false;
}

uint32_t __cdecl X_GetSpriteGeosetColor(uint32_t pSprite, uint32_t index) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                    return ReadMemory(pUnk + 0x4 + index * 0x10) & 0xFFFFFF;
    return 0xFF;
}

uint32_t __cdecl X_GetSpriteGeosetAlpha(uint32_t pSprite, uint32_t index) {
    if (GetSpriteType(pSprite) != SpriteType::INVALID)
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                    return (uint8_t)(ReadMemory<float>(pUnk + 0xC + index * 0x10) * 255.f);
    return 0xFF;
}

uint32_t __cdecl X_SetSpriteReplaceableTexture(uint32_t pSprite, uint32_t path, uint32_t replaceableID) {
    checkValid();
    static uint32_t pSprite_SetReplacableTexture = searchCSprite_SetReplacableTexture();
    static uint32_t pLoadTexture = searchLoadTexture();
    uint32_t pTexture = base::std_call<uint32_t>(pLoadTexture, jass::from_string(path), 0);
    if (!pTexture)
        return false;
    base::fast_call<void>(pSprite_SetReplacableTexture, pSprite, pTexture, replaceableID);
    return true;
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetSpriteAnimationEx(uint32_t pSprite, uint32_t animName, uint32_t flag) {
    checkValid();
    static SetSpriteAnimationByNameAddress addr = searchSetSpriteAnimation();
    uint32_t AnimData[4] = { 0, 0, 0, 0 };

    if (get_war3_searcher().get_version() == version_126) {
        _asm {
            mov ecx, animName;
            lea edi, AnimData;
            call addr.GetAnimationDataFromJassString;
        }
    }
    else {
        base::fast_call<void>(addr.GetAnimationDataFromJassString, animName, AnimData);
    }

    if (!AnimData[1])
        return false;

    base::fast_call<double>(addr.SetSpriteAnimation, pSprite, AnimData[2], AnimData[1], flag);
    return true;
}

uint32_t __cdecl X_SetSpriteAnimation(uint32_t pSprite, uint32_t animName) {
    return X_SetSpriteAnimationEx(pSprite, animName, 0);
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetSpriteAnimationByIndexEx(uint32_t pSprite, uint32_t index, uint32_t flag) {
    checkValid();

    static uint32_t pSetSpriteAnimationByIndex = searchSetSpriteAnimationByIndex();
    base::fast_call<double>(pSetSpriteAnimationByIndex, pSprite, index, flag);
    return true;
}

uint32_t __cdecl X_SetSpriteAnimationByIndex(uint32_t pSprite, uint32_t index) {
    return X_SetSpriteAnimationByIndexEx(pSprite, index, 0);
}

#undef checkValid
#undef setupOffset

init(Sprite) {
    jass::japi_add((uint32_t)X_SpriteFrameGetSprite,            "X_SpriteFrameGetSprite",           "(I)I");                     // CSpriteUber
    jass::japi_add((uint32_t)X_War3ImageGetSprite,              "X_UnitGetSprite",                  "(Hunit;)I");                // CSpriteUber
    jass::japi_add((uint32_t)X_War3ImageGetSprite,              "X_EffectGetSprite",                "(Heffect;)I");              // CSpriteUber
    jass::japi_add((uint32_t)X_War3ImageGetSprite,              "X_TrackableGetSprite",             "(Htrackable;)I");           // CSpriteUber
    jass::japi_add((uint32_t)X_War3ImageGetSprite,              "X_ItemGetSprite",                  "(Hitem;)I");                // CSpriteMini, 无法用Sprite接口设置颜色(实际上是会设置回去)
    jass::japi_add((uint32_t)X_War3ImageGetSprite,              "X_DestructableGetSprite",          "(Hdestructable;)I");        // CSpriteMini, 无法用Sprite接口设置颜色(实际上是会设置回去)

    jass::japi_add((uint32_t)X_IsSpriteValid,                   "X_IsSpriteValid",                  "(I)B");
    jass::japi_add((uint32_t)X_GetSpriteType,                   "X_GetSpriteType",                  "(I)I");
    jass::japi_add((uint32_t)X_GetSpriteGeosetCount,            "X_GetSpriteGeosetCount",           "(I)I");
    
    jass::japi_add((uint32_t)X_SetSpriteX,                      "X_SetSpriteX",                     "(IR)B");
    jass::japi_add((uint32_t)X_GetSpriteX,                      "X_GetSpriteX",                     "(I)R");

    jass::japi_add((uint32_t)X_SetSpriteY,                      "X_SetSpriteY",                     "(IR)B");
    jass::japi_add((uint32_t)X_GetSpriteY,                      "X_GetSpriteY",                     "(I)R");

    jass::japi_add((uint32_t)X_SetSpriteZ,                      "X_SetSpriteZ",                     "(IR)B");
    jass::japi_add((uint32_t)X_GetSpriteZ,                      "X_GetSpriteZ",                     "(I)R");
    
    jass::japi_add((uint32_t)X_SetSpriteSize,                   "X_SetSpriteSize",                  "(IR)B");
    jass::japi_add((uint32_t)X_GetSpriteSize,                   "X_GetSpriteSize",                  "(I)R");
    
    jass::japi_add((uint32_t)X_SpriteMatrixRotateX,             "X_SpriteMatrixRotateX",            "(IR)B");
    jass::japi_add((uint32_t)X_SpriteMatrixRotateY,             "X_SpriteMatrixRotateY",            "(IR)B");
    jass::japi_add((uint32_t)X_SpriteMatrixRotateZ,             "X_SpriteMatrixRotateZ",            "(IR)B");
    jass::japi_add((uint32_t)X_SpriteMatrixScale,               "X_SpriteMatrixScale",              "(IRRR)B");
    jass::japi_add((uint32_t)X_SpriteMatrixReset,               "X_SpriteMatrixReset",              "(I)B");
    
    jass::japi_add((uint32_t)X_SetSpriteTimeScale,              "X_SetSpriteTimeScale",             "(IR)B");
    jass::japi_add((uint32_t)X_GetSpriteTimeScale,              "X_GetSpriteTimeScale",             "(I)R");
    
    jass::japi_add((uint32_t)X_SetSpriteColor,                  "X_SetSpriteColor",                 "(II)B");
    jass::japi_add((uint32_t)X_SetSpriteAlpha,                  "X_SetSpriteAlpha",                 "(II)B");
    
    jass::japi_add((uint32_t)X_GetSpriteColor,                  "X_GetSpriteColor",                 "(I)I");        // 等效于X_GetSpriteGeosetColor, index 0
    jass::japi_add((uint32_t)X_GetSpriteAlpha,                  "X_GetSpriteAlpha",                 "(I)I");        // 等效于X_GetSpriteGeosetAlpha, index 0
    
    jass::japi_add((uint32_t)X_SetSpriteGeosetColor,            "X_SetSpriteGeosetColor",           "(III)B");      // index 始于 0
    jass::japi_add((uint32_t)X_SetSpriteGeosetAlpha,            "X_SetSpriteGeosetAlpha",           "(III)B");      // index 始于 0
    jass::japi_add((uint32_t)X_GetSpriteGeosetColor,            "X_GetSpriteGeosetColor",           "(II)I");       // index 始于 0
    jass::japi_add((uint32_t)X_GetSpriteGeosetAlpha,            "X_GetSpriteGeosetAlpha",           "(II)I");       // index 始于 0

    jass::japi_add((uint32_t)X_SetSpriteReplaceableTexture,     "X_SetSpriteReplaceableTexture",    "(ISI)B");
    
    jass::japi_add((uint32_t)X_SetSpriteAnimation,              "X_SetSpriteAnimation",             "(IS)B");
    jass::japi_add((uint32_t)X_SetSpriteAnimationEx,            "X_SetSpriteAnimationEx",           "(ISI)B");
    jass::japi_add((uint32_t)X_SetSpriteAnimationByIndex,       "X_SetSpriteAnimationByIndex",      "(II)B");
    jass::japi_add((uint32_t)X_SetSpriteAnimationByIndexEx,     "X_SetSpriteAnimationByIndexEx",    "(III)B");
}