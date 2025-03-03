#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_SpriteFrameSetDefaultLight(uint32_t pSpriteFrame_raw) {
    CSpriteFrame* pSpriteFrame = ConvertPtr(CSpriteFrame, pSpriteFrame_raw);
    if (!pSpriteFrame)
        return false;

    CLight* pLight = CLight::CreateEx();
    pSpriteFrame->SetLight(pLight, 0);
    pLight->release();
    return true;
}

init_L(Frame) {
    jass::japi_add((uint32_t)X_SpriteFrameSetDefaultLight, "X_SpriteFrameSetDefaultLight", "(I)B");
}