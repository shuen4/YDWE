#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <warcraft3/version.h>

#include "qmatrix.h"

namespace warcraft3::japi {

    uint32_t searchSetSpriteTeamColor();
    struct SetSpriteAnimationByNameAddress {
        uintptr_t GetAnimationDataFromJassString;
        uintptr_t SetSpriteAnimation;
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

    uint32_t __cdecl EXSpriteFrameGetSprite(uint32_t pSpriteFrame) {
        if (isSpriteFrame(pSpriteFrame) && ReadMemory(pSpriteFrame + 0x174))
            return ReadMemory(ReadMemory(pSpriteFrame + 0x178));
        else
            return 0;
    }

    uint32_t __cdecl EXWar3ImageGetSprite(uint32_t handle) {
        uint32_t pObj = handle_to_object(handle);
        if (!pObj || !type_check(get_object_type(pObj), '+w3i'))
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

    bool __cdecl EXIsSpriteValid(uint32_t pSprite) {
        return GetSpriteType(pSprite) != SpriteType::INVALID;
    }

    SpriteType __cdecl EXGetSpriteType(uint32_t pSprite) {
        return GetSpriteType(pSprite);
    }

#define setupOffset(offsetMini, offsetUber)                                                             \
    uint32_t offset = GetOffsetBySpriteType(GetSpriteType(pSprite), offsetMini, offsetUber);            \
    if (!offset)                                                                                        \
        return false
#define checkValid()                                                                                    \
    if (GetSpriteType(pSprite) == SpriteType::INVALID)                                                  \
        return false

    uint32_t __cdecl EXGetSpriteGeosetCount(uint32_t pSprite) {
        if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
            return ReadMemory(pModelComplex + 0xC);
        return 0;
    }

    bool __cdecl EXSetSpriteX(uint32_t pSprite, float* x) {
        setupOffset(0x88, 0xC0);
        WriteMemory(pSprite + offset, *x);
        return true;
    }

    bool __cdecl EXSetSpriteY(uint32_t pSprite, float* y) {
        setupOffset(0x8C, 0xC4);
        WriteMemory(pSprite + offset, *y);
        return true;
    }

    bool __cdecl EXSetSpriteZ(uint32_t pSprite, float* z) {
        setupOffset(0x90, 0xC8);
        WriteMemory(pSprite + offset, *z);
        return true;
    }

    uint32_t __cdecl EXGetSpriteX(uint32_t pSprite) {
        setupOffset(0x88, 0xC0);
        return ReadMemory(pSprite + offset);
    }

    uint32_t __cdecl EXGetSpriteY(uint32_t pSprite) {
        setupOffset(0x8C, 0xC4);
        return ReadMemory(pSprite + offset);
    }

    uint32_t __cdecl EXGetSpriteZ(uint32_t pSprite) {
        setupOffset(0x90, 0xC8);
        return ReadMemory(pSprite + offset);
    }

    bool __cdecl EXSetSpriteSize(uint32_t pSprite, float* size) {
        checkValid();
        base::this_call_vf<void>(pSprite, 0x24, *size);
        return true;
        /* 还是调用 vf 比较好吧
        setupOffset(0x94, 0xE8);
        WriteMemory(pSprite + offset, *size);
        return true;
        */
    }

    uint32_t __cdecl EXGetSpriteSize(uint32_t pSprite) {
        setupOffset(0x94, 0xE8);
        return ReadMemory(pSprite + offset);
    }

    bool __cdecl EXSpriteMatrixRotateX(uint32_t pSprite, float* angle) {
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

    bool __cdecl EXSpriteMatrixRotateY(uint32_t pSprite, float* angle) {
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

    bool __cdecl EXSpriteMatrixRotateZ(uint32_t pSprite, float* angle) {
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

    bool __cdecl EXSpriteMatrixScale(uint32_t pSprite, float* x, float* y, float* z) {
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

    bool __cdecl EXSpriteMatrixReset(uint32_t pSprite) {
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

    uint32_t __cdecl EXSetSpriteTimeScale(uint32_t pSprite, float* timeScale) {
        checkValid();
        base::this_call_vf<void>(pSprite, 0x28, *timeScale);
        return true;
        /* 还是调用 vf 比较好吧
        setupOffset(0x48, 0x190);
        WriteMemory(pSprite + offset, *timeScale);
        return true;
        */
    }

    uint32_t __cdecl EXGetSpriteTimeScale(uint32_t pSprite) {
        setupOffset(0x48, 0x190);
        return ReadMemory(pSprite + offset);
    }

    bool __cdecl EXSetSpriteColor(uint32_t pSprite, uint32_t color) {
        checkValid();
        base::this_call_vf<void>(pSprite, 0x30, color & 0xFFFFFF);
        return true;
    }

    bool __cdecl EXSetSpriteAlpha(uint32_t pSprite, uint32_t alpha) {
        checkValid();
        base::this_call_vf<void>(pSprite, 0x34, alpha & 0xFF);
        return true;
    }

    uint32_t __cdecl EXGetSpriteColor(uint32_t pSprite) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC)) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                        return ReadMemory(pUnk + 0x4) & 0xFFFFFF;
        return 0xFF;
    }

    uint32_t __cdecl EXGetSpriteAlpha(uint32_t pSprite) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC)) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                        return (uint8_t)(ReadMemory<float>(pUnk + 0xC) * 255.f);
        return 0xFF;
    }

    bool __cdecl EXSetSpriteGeosetColor(uint32_t pSprite, uint32_t index, uint32_t value) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20)) {
                        WriteMemory(pUnk + 0x4 + index * 0x10, ReadMemory<uint8_t>(pUnk + 0x7 + index * 0x10) | (value & 0xFFFFFF));
                        return true;
                    }
        return false;
    }

    bool __cdecl EXSetSpriteGeosetAlpha(uint32_t pSprite, uint32_t index, uint32_t value) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20)) {
                        WriteMemory(pUnk + 0xC + index * 0x10, (float)(value & 0xFF) / 255.f);
                        return true;
                    }
        return false;
    }

    uint32_t __cdecl EXGetSpriteGeosetColor(uint32_t pSprite, uint32_t index) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                        return ReadMemory(pUnk + 0x4 + index * 0x10) & 0xFFFFFF;
        return 0xFF;
    }

    uint32_t __cdecl EXGetSpriteGeosetAlpha(uint32_t pSprite, uint32_t index) {
        if (GetSpriteType(pSprite) != SpriteType::INVALID)
            if (uint32_t pModelComplex = ReadMemory(pSprite + 0x20))
                if (ReadMemory(pModelComplex + 0xC) > index) // Geoset 数量
                    if (uint32_t pUnk = ReadMemory(pModelComplex + 0x20))
                        return (uint8_t)(ReadMemory<float>(pUnk + 0xC + index * 0x10) * 255.f);
        return 0xFF;
    }

    bool __cdecl EXSetSpriteReplaceableTexture(uint32_t pSprite, uint32_t path, uint32_t replaceableID) {
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
    //		none(set)			0
    //		queue				1 << 1
    //		RARITY_FREQUENT		1 << 4
    //		RARITY_RARE			1 << 5
    bool __cdecl EXSetSpriteAnimationEx(uint32_t pSprite, uint32_t animName, uint32_t flag) {
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

    bool __cdecl EXSetSpriteAnimation(uint32_t pSprite, uint32_t animName) {
        return EXSetSpriteAnimationEx(pSprite, animName, 0);
    }

    // flag:
    //		none(set)			0
    //		queue				1 << 1
    //		RARITY_FREQUENT		1 << 4
    //		RARITY_RARE			1 << 5
    bool __cdecl EXSetSpriteAnimationByIndexEx(uint32_t pSprite, uint32_t index, uint32_t flag) {
        checkValid();

        static uint32_t pSetSpriteAnimationByIndex = searchSetSpriteAnimationByIndex();
        base::fast_call<double>(pSetSpriteAnimationByIndex, pSprite, index, flag);
        return true;
    }

    bool __cdecl EXSetSpriteAnimationByIndex(uint32_t pSprite, uint32_t index) {
        return EXSetSpriteAnimationByIndexEx(pSprite, index, 0);
    }

#undef checkValid
#undef setupOffset

    void InitializeSprite() {
        jass::japi_add((uint32_t)EXSpriteFrameGetSprite,            "EXSpriteFrameGetSprite",           "(I)I");                     // CSpriteUber
        jass::japi_add((uint32_t)EXWar3ImageGetSprite,              "EXUnitGetSprite",                  "(Hunit;)I");                // CSpriteUber
        jass::japi_add((uint32_t)EXWar3ImageGetSprite,              "EXEffectGetSprite",                "(Heffect;)I");              // CSpriteUber
        jass::japi_add((uint32_t)EXWar3ImageGetSprite,              "EXTrackableGetSprite",             "(Htrackable;)I");           // CSpriteUber
        jass::japi_add((uint32_t)EXWar3ImageGetSprite,              "EXItemGetSprite",                  "(Hitem;)I");                // CSpriteMini, 无法用Sprite接口设置颜色(实际上是会设置回去)
        jass::japi_add((uint32_t)EXWar3ImageGetSprite,              "EXDestructableGetSprite",          "(Hdestructable;)I");        // CSpriteMini, 无法用Sprite接口设置颜色(实际上是会设置回去)

        jass::japi_add((uint32_t)EXIsSpriteValid,                   "EXIsSpriteValid",                  "(I)B");
        jass::japi_add((uint32_t)EXGetSpriteType,                   "EXGetSpriteType",                  "(I)I");
        jass::japi_add((uint32_t)EXGetSpriteGeosetCount,            "EXGetSpriteGeosetCount",           "(I)I");
        
        jass::japi_add((uint32_t)EXSetSpriteX,                      "EXSetSpriteX",                     "(IR)B");
        jass::japi_add((uint32_t)EXGetSpriteX,                      "EXGetSpriteX",                     "(I)R");

        jass::japi_add((uint32_t)EXSetSpriteY,                      "EXSetSpriteY",                     "(IR)B");
        jass::japi_add((uint32_t)EXGetSpriteY,                      "EXGetSpriteY",                     "(I)R");

		jass::japi_add((uint32_t)EXSetSpriteZ,                      "EXSetSpriteZ",                     "(IR)B");
		jass::japi_add((uint32_t)EXGetSpriteZ,                      "EXGetSpriteZ",                     "(I)R");
        
		jass::japi_add((uint32_t)EXSetSpriteSize,                   "EXSetSpriteSize",                  "(IR)B");
		jass::japi_add((uint32_t)EXGetSpriteSize,                   "EXGetSpriteSize",                  "(I)R");
        
		jass::japi_add((uint32_t)EXSpriteMatrixRotateX,             "EXSpriteMatrixRotateX",			"(IR)B");
		jass::japi_add((uint32_t)EXSpriteMatrixRotateY,             "EXSpriteMatrixRotateY",			"(IR)B");
		jass::japi_add((uint32_t)EXSpriteMatrixRotateZ,             "EXSpriteMatrixRotateZ",			"(IR)B");
        jass::japi_add((uint32_t)EXSpriteMatrixScale,               "EXSpriteMatrixScale",              "(IRRR)B");
        jass::japi_add((uint32_t)EXSpriteMatrixReset,               "EXSpriteMatrixReset",              "(I)B");
        
		jass::japi_add((uint32_t)EXSetSpriteTimeScale,              "EXSetSpriteTimeScale",				"(IR)B");
		jass::japi_add((uint32_t)EXGetSpriteTimeScale,              "EXGetSpriteTimeScale",				"(I)R");
        
		jass::japi_add((uint32_t)EXSetSpriteColor,                  "EXSetSpriteColor",				    "(II)B");
		jass::japi_add((uint32_t)EXSetSpriteAlpha,                  "EXSetSpriteAlpha",				    "(II)B");
		
		jass::japi_add((uint32_t)EXGetSpriteColor,                  "EXGetSpriteColor",                 "(I)I");        // 等效于EXGetSpriteGeosetColor, index 0
		jass::japi_add((uint32_t)EXGetSpriteAlpha,                  "EXGetSpriteAlpha",                 "(I)I");        // 等效于EXGetSpriteGeosetAlpha, index 0
        
		jass::japi_add((uint32_t)EXSetSpriteGeosetColor,            "EXSetSpriteGeosetColor",			"(III)B");      // index 始于 0
		jass::japi_add((uint32_t)EXSetSpriteGeosetAlpha,            "EXSetSpriteGeosetAlpha",			"(III)B");      // index 始于 0
		jass::japi_add((uint32_t)EXGetSpriteGeosetColor,            "EXGetSpriteGeosetColor",           "(II)I");       // index 始于 0
		jass::japi_add((uint32_t)EXGetSpriteGeosetAlpha,            "EXGetSpriteGeosetAlpha",           "(II)I");       // index 始于 0

		jass::japi_add((uint32_t)EXSetSpriteReplaceableTexture,     "EXSetSpriteReplaceableTexture",    "(ISI)B");
        
		jass::japi_add((uint32_t)EXSetSpriteAnimation,              "EXSetSpriteAnimation",             "(IS)B");
		jass::japi_add((uint32_t)EXSetSpriteAnimationEx,            "EXSetSpriteAnimationEx",           "(ISI)B");
		jass::japi_add((uint32_t)EXSetSpriteAnimationByIndex,       "EXSetSpriteAnimationByIndex",      "(II)B");
		jass::japi_add((uint32_t)EXSetSpriteAnimationByIndexEx,     "EXSetSpriteAnimationByIndexEx",    "(III)B");
    }
}
