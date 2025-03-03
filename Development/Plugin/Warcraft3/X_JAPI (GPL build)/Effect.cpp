#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_SetEffectTimeScale(uint32_t effect, float* speed) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    pEffectImage->sprite->SetTimeScale(*speed);
    return true;
}

uint32_t __cdecl X_SetEffectColorRed(uint32_t effect, uint32_t red) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.R = red & 0xFF;
    ((CSpriteUber_*)pEffectImage->sprite)->UpdateColor();
    return true;
}

uint32_t __cdecl X_SetEffectColorGreen(uint32_t effect, uint32_t green) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.G = green & 0xFF;
    ((CSpriteUber_*)pEffectImage->sprite)->UpdateColor();
    return true;
}

uint32_t __cdecl X_SetEffectColorBlue(uint32_t effect, uint32_t blue) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.B = blue & 0xFF;
    ((CSpriteUber_*)pEffectImage->sprite)->UpdateColor();
    return true;
}

uint32_t __cdecl X_SetEffectColor(uint32_t effect, uint32_t rgb) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.ARGB = rgb & 0xFFFFFF; // 透明无效
    ((CSpriteUber_*)pEffectImage->sprite)->UpdateColor();
    return true;
}

uint32_t __cdecl X_SetEffectAlpha(uint32_t effect, uint32_t alpha) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    pEffectImage->sprite->SetAlpha(alpha & 0xFF);
    return true;
}

uint32_t __cdecl X_GetEffectColorRed(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    return ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.R;
}

uint32_t __cdecl X_GetEffectColorGreen(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    return ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.G;
}

uint32_t __cdecl X_GetEffectColorBlue(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    return ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.B;
}

uint32_t __cdecl X_GetEffectColor(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    return ((CSpriteUber_*)pEffectImage->sprite)->color.managed_data.ARGB & 0xFFFFFF;
}

uint32_t __cdecl X_GetEffectAlpha(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    if (pEffectImage->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    return ((CSpriteUber_*)pEffectImage->sprite)->alpha.managed_data & 0xFF;
}

uint32_t __cdecl X_SetEffectTeamColor(uint32_t effect, uint32_t/* 实际上并不是 handle */ playercolor) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    pEffectImage->sprite->SetTeamColor(playercolor, playercolor);
    return true;
}

uint32_t __cdecl X_UpdateEffectSmartPosition(uint32_t effect) {
    CEffectImagePos* pEffectImagePos = ConvertHandle<CEffectImagePos>(effect);
    if (!pEffectImagePos || !pEffectImagePos->sprite)
        return false;

    if (pEffectImagePos->sprite->GetSpriteType() != CSprite::Type::UBER)
        return false;

    pEffectImagePos->position.SetLocation(&((CSpriteUber_*)pEffectImagePos->sprite)->position.managed_data);
    return true;
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetEffectAnimationEx(uint32_t effect, uint32_t animName, uint32_t flag) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

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

    pEffectImage->sprite->SetAnimation(animData[2], animData[1], flag);
    return true;
}

uint32_t __cdecl X_SetEffectAnimation(uint32_t effect, uint32_t animName) {
    return X_SetEffectAnimationEx(effect, animName, 0);
}

// flag:
//      none(set)           0
//      queue               1 << 1
//      RARITY_FREQUENT     1 << 4
//      RARITY_RARE         1 << 5
uint32_t __cdecl X_SetEffectAnimationByIndexEx(uint32_t effect, uint32_t index, uint32_t flag) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage || !pEffectImage->sprite)
        return false;

    pEffectImage->sprite->SetAnimationByIndex(index, flag);
    return true;
}

uint32_t __cdecl X_SetEffectAnimationByIndex(uint32_t effect, uint32_t index) {
    return X_SetEffectAnimationByIndexEx(effect, index, 0);
}

uint32_t __cdecl X_HideEffect(uint32_t effect, uint32_t hide) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage)
        return false;

    if (hide)
        pEffectImage->agent_flag |= CWar3Image::agent_flag_hidden;
    else
        pEffectImage->agent_flag &= ~CWar3Image::agent_flag_hidden;
    return true;
}

uint32_t __cdecl X_RemoveEffect(uint32_t effect) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage)
        return false;

    pEffectImage->destroy_timer.Stop();
    pEffectImage->Destroy();
    return true;
}

uint32_t __cdecl X_RemoveEffectTimed(uint32_t effect, float* duration) {
    CEffectImage* pEffectImage = ConvertHandle<CEffectImage>(effect);
    if (!pEffectImage)
        return false;

    pEffectImage->TimedRemove(duration);
    return true;
}

init_L(Effect) {
    jass::japi_add((uint32_t)X_SetEffectTimeScale,             "X_SetEffectTimeScale",         "(Heffect;R)B");
    jass::japi_add((uint32_t)X_SetEffectColorRed,              "X_SetEffectColorRed",          "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColorGreen,            "X_SetEffectColorGreen",        "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColorBlue,             "X_SetEffectColorBlue",         "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectColor,                 "X_SetEffectColor",             "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectAlpha,                 "X_SetEffectAlpha",             "(Heffect;I)B");
    jass::japi_add((uint32_t)X_GetEffectColorRed,              "X_GetEffectColorRed",          "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColorGreen,            "X_GetEffectColorGreen",        "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColorBlue,             "X_GetEffectColorBlue",         "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectColor,                 "X_GetEffectColor",             "(Heffect;)I");
    jass::japi_add((uint32_t)X_GetEffectAlpha,                 "X_GetEffectAlpha",             "(Heffect;)I");
    jass::japi_add((uint32_t)X_SetEffectTeamColor,             "X_SetEffectTeamColor",         "(Heffect;Hplayercolor;)B");
    jass::japi_add((uint32_t)X_UpdateEffectSmartPosition,      "X_UpdateEffectSmartPosition",  "(Heffect;)B");
    jass::japi_add((uint32_t)X_SetEffectAnimation,             "X_SetEffectAnimation",         "(Heffect;S)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationEx,           "X_SetEffectAnimationEx",       "(Heffect;SI)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationByIndex,      "X_SetEffectAnimationByIndex",  "(Heffect;I)B");
    jass::japi_add((uint32_t)X_SetEffectAnimationByIndexEx,    "X_SetEffectAnimationByIndexEx","(Heffect;II)B");
    jass::japi_add((uint32_t)X_HideEffect,                     "X_HideEffect",                 "(Heffect;B)B");
    jass::japi_add((uint32_t)X_RemoveEffect,                   "X_RemoveEffect",               "(Heffect;)B");
    jass::japi_add((uint32_t)X_RemoveEffectTimed,              "X_RemoveEffectTimed",          "(Heffect;R)B");
}