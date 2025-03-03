#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>

#include "util.h"

#define M_PI       3.14159265358979323846

CSprite* __cdecl X_SpriteFrameGetSprite(uint32_t pSpriteFrame_raw, uint32_t index) {
    CSpriteFrame* pSpriteFrame = ConvertPtr(CSpriteFrame, pSpriteFrame_raw);

    if (pSpriteFrame->sprite.size < index)
        return NULL;

    return pSpriteFrame->sprite.data[index];
}

CSprite* __cdecl X_War3ImageGetSprite(uint32_t handle) {
    CWar3Image* pWar3Image = ConvertHandle<CWar3Image>(handle);

    if (!pWar3Image)
        return 0;

    return pWar3Image->sprite;
}

uint32_t GetOffsetBySpriteType(CSprite::Type type, uint32_t offsetMini, uint32_t offsetUber) {
    if (type == CSprite::Type::MINI)
        return offsetMini;
    else if (type == CSprite::Type::UBER)
        return offsetUber;
    return 0;
}

uint32_t __cdecl X_IsSpriteValid(CSprite* pSprite) {
    return pSprite->GetSpriteType() != CSprite::Type::INVALID;
}

CSprite::Type __cdecl X_GetSpriteType(CSprite* pSprite) {
    return pSprite->GetSpriteType();
}

#define setupOffset(offsetMini, offsetUber)                                                             \
uint32_t offset = GetOffsetBySpriteType(pSprite->GetSpriteType(), offsetMini, offsetUber);              \
    if (!offset)                                                                                        \
        return false
#define checkValid()                                                                                    \
    if (pSprite->GetSpriteType() == CSprite::Type::INVALID)                                             \
        return false

uint32_t __cdecl X_GetSpriteGeosetCount(CSprite* pSprite) {
    checkValid();
    if (CModel* pModel = pSprite->model)
        return pModel->geoset.size;
    return 0;
}

uint32_t __cdecl X_GetSpriteGeosetColorCount(CSprite* pSprite) {
    if (CModel* pModel = pSprite->model)
        return pModel->geoset_color.size;
    return 0;
}

uint32_t __cdecl X_SetSpriteX(CSprite* pSprite, float* x) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->position.x = *x;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->position.managed_data.x = *x;
        return true;
    default:
        return false;
    }
}

uint32_t __cdecl X_SetSpriteY(CSprite* pSprite, float* y) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->position.y = *y;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->position.managed_data.y = *y;
        return true;
    default:
        return false;
    }
}

uint32_t __cdecl X_SetSpriteZ(CSprite* pSprite, float* z) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->position.z = *z;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->position.managed_data.z = *z;
        return true;
    default:
        return false;
    }
}

uint32_t __cdecl X_GetSpriteX(CSprite* pSprite) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        return ((CSpriteMini_*)pSprite)->position.x;
    case CSprite::Type::UBER:
        return ((CSpriteUber_*)pSprite)->position.managed_data.x;
    default:
        return 0;
    }
}

uint32_t __cdecl X_GetSpriteY(CSprite* pSprite) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        return ((CSpriteMini_*)pSprite)->position.y;
    case CSprite::Type::UBER:
        return ((CSpriteUber_*)pSprite)->position.managed_data.y;
    default:
        return 0;
    }
}

uint32_t __cdecl X_GetSpriteZ(CSprite* pSprite) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        return ((CSpriteMini_*)pSprite)->position.z;
    case CSprite::Type::UBER:
        return ((CSpriteUber_*)pSprite)->position.managed_data.z;
    default:
        return 0;
    }
}

uint32_t __cdecl X_SetSpriteSize(CSprite* pSprite, float* size) {
    checkValid();
    pSprite->SetSize(*size);
    return true;
}

uint32_t __cdecl X_GetSpriteSize(CSprite* pSprite) {
    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        return ((CSpriteMini_*)pSprite)->scale;
    case CSprite::Type::UBER:
        return ((CSpriteUber_*)pSprite)->scale.managed_data;
    default:
        return 0;
    }
}

uint32_t __cdecl X_SpriteMatrixRotateX(CSprite* pSprite, float* angle) {
    checkValid();

    float angle_radians = *angle * float(M_PI / 180.);
    float m[3][3] = {
        { 1.f, 0.f, 0.f },
        { 0.f, cosf(angle_radians), sinf(angle_radians) },
        { 0.f, -sinf(angle_radians), cosf(angle_radians) },
    };

    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->matrix *= m;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->matrix.managed_data *= m;
        return true;
    }
    return true;
}

uint32_t __cdecl X_SpriteMatrixRotateY(CSprite* pSprite, float* angle) {
    checkValid();

    float angle_radians = *angle * float(M_PI / 180.);
    float m[3][3] = {
        { cosf(angle_radians), 0.f, -sinf(angle_radians) },
        { 0.f, 1.f, 0.f },
        { sinf(angle_radians), 0.f, cosf(angle_radians) },
    };

    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->matrix *= m;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->matrix.managed_data *= m;
        return true;
    }
    return true;
}

uint32_t __cdecl X_SpriteMatrixRotateZ(CSprite* pSprite, float* angle) {
    checkValid();

    float angle_radians = *angle * float(M_PI / 180.);
    float m[3][3] = {
        { cosf(angle_radians), sinf(angle_radians), 0.f },
        { -sinf(angle_radians), cosf(angle_radians), 0.f },
        { 0.f, 0.f, 1.f },
    };

    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->matrix *= m;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->matrix.managed_data *= m;
        return true;
    }
    return true;
}

uint32_t __cdecl X_SpriteMatrixScale(CSprite* pSprite, float* x, float* y, float* z) {
    checkValid();

    float m[3][3] = {
        { *x, 0.f, 0.f },
        { 0.f, *y, 0.f },
        { 0.f, 0.f, *z },
    };

    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->matrix *= m;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->matrix.managed_data *= m;
        return true;
    }
    return true;
}

uint32_t __cdecl X_SpriteMatrixReset(CSprite* pSprite) {
    checkValid();

    float m[3][3] = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f },
    };

    switch (pSprite->GetSpriteType()) {
    case CSprite::Type::MINI:
        ((CSpriteMini_*)pSprite)->matrix = m;
        return true;
    case CSprite::Type::UBER:
        ((CSpriteUber_*)pSprite)->matrix.managed_data = m;
        return true;
    }
    return true;
}

uint32_t __cdecl X_SetSpriteTimeScale(CSprite* pSprite, float* timeScale) {
    checkValid();
    pSprite->SetTimeScale(*timeScale);
    return true;
}

uint32_t __cdecl X_GetSpriteTimeScale(CSprite* pSprite) {
    checkValid();
    return __any(pSprite->GetTimeScale());
}

uint32_t __cdecl X_SetSpriteColor(CSprite* pSprite, uint32_t color) {
    checkValid();
    pSprite->SetColor(color & 0xFFFFFF);
    return true;
}

uint32_t __cdecl X_SetSpriteAlpha(CSprite* pSprite, uint32_t alpha) {
    checkValid();
    pSprite->SetAlpha(alpha & 0xFF);
    return true;
}

uint32_t __cdecl X_GetSpriteColor(CSprite* pSprite) {
    checkValid();
    
    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size)
            return pModel->geoset_color.data[0].color.ARGB & 0xFFFFFF;
    return 0xFFFFFF;
}

uint32_t __cdecl X_GetSpriteAlpha(CSprite* pSprite) {
    checkValid();

    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size)
            return (uint8_t)(pModel->geoset_color.data[0].alpha * 255.f);
    return 0xFF;
}

uint32_t __cdecl X_SetSpriteGeosetColor(CSprite* pSprite, uint32_t index, uint32_t value) {
    checkValid();

    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size > index) {
            pModel->geoset_color.data[index].color.ARGB = ((uint32_t)pModel->geoset_color.data[index].color.A << 24) | (value & 0xFFFFFF);
            return true;
        }
    return false;
}

uint32_t __cdecl X_SetSpriteGeosetAlpha(CSprite* pSprite, uint32_t index, uint32_t value) {
    checkValid();

    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size > index)
            pModel->geoset_color.data[index].alpha = (float)(value & 0xFF) / 255.f;
    return false;
}

uint32_t __cdecl X_GetSpriteGeosetColor(CSprite* pSprite, uint32_t index) {
    checkValid();

    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size > index)
            return pModel->geoset_color.data[index].color.ARGB & 0xFFFFFF;
    return 0xFFFFFF;
}

uint32_t __cdecl X_GetSpriteGeosetAlpha(CSprite* pSprite, uint32_t index) {
    checkValid();

    if (CModel* pModel = pSprite->model)
        if (pModel->geoset_color.size > index)
            return (uint8_t)(pModel->geoset_color.data[index].alpha * 255.f);
    return 0xFF;
}

uint32_t __cdecl X_SetSpriteReplaceableTexture(CSprite* pSprite, uint32_t path, uint32_t replaceableID) {
    checkValid();
    pSprite->SetReplacableTexture(jass::from_string(path), replaceableID, 0);
    return true;
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetSpriteAnimationEx(CSprite* pSprite, uint32_t animName, uint32_t flag) {
    checkValid();

    uint32_t animData[4] = { 0, 0, 0, 0 };
    if (version == version_126) {
        _asm {
            mov ecx, animName;
            lea edi, animData;
            call FUNC::GetAnimationDataFromJassString;
        }
    }
    else
        FUNC::GetAnimationDataFromJassString(animName, animData);

    if (!animData[1])
        return false;

    pSprite->SetAnimation(animData[2], animData[1], flag);
    return true;
}

uint32_t __cdecl X_SetSpriteAnimation(CSprite* pSprite, uint32_t animName) {
    return X_SetSpriteAnimationEx(pSprite, animName, 0);
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetSpriteAnimationByIndexEx(CSprite* pSprite, uint32_t index, uint32_t flag) {
    checkValid();

    pSprite->SetAnimationByIndex(index, flag);
    return true;
}

uint32_t __cdecl X_SetSpriteAnimationByIndex(CSprite* pSprite, uint32_t index) {
    return X_SetSpriteAnimationByIndexEx(pSprite, index, 0);
}

#undef checkValid
#undef setupOffset

init_L(Sprite) {
    jass::japi_add((uint32_t)X_SpriteFrameGetSprite,            "X_SpriteFrameGetSprite",           "(II)I");                    // CSpriteUber
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