#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>

#include "util.h"

// 透明度有效 但时间一到渐变会重置透明度
// 位置:
// 0 错误消息 (魔法值不够, ...)
// 1 游戏消息 (DisplayTextToPlayer, ...)
// 2 聊天消息 (X_DisplayChat, ...)
// 3 维修等级 (低维修等级, ...)
uint32_t __cdecl X_DisplayMessage(uint32_t message, float* duration, uint32_t where, uint32_t color) {
    if (where > 3)
        return false;

    CGameUI* pGameUI = CGameUI::GetInstance();
    if (!pGameUI)
        return false;

    pGameUI->simple_message_frames[where]->AddText(jass::from_trigstring(jass::from_string(message)), color, *duration, 0);
    return true;
}

// 位置同上
uint32_t __cdecl X_ClearMessage(uint32_t where) {
    if (where > 3)
        return false;

    CGameUI* pGameUI = CGameUI::GetInstance();
    if (!pGameUI)
        return false;

    pGameUI->simple_message_frames[where]->ClearText();
    return true;
}

// 懒人版 (可能性能比在JASS用两个nf好)
// 位置同上
uint32_t __cdecl X_SetMessage(uint32_t message, float* duration, uint32_t where, uint32_t color) {
    if (where > 3)
        return false;

    CGameUI* pGameUI = CGameUI::GetInstance();
    if (!pGameUI)
        return false;

    pGameUI->simple_message_frames[where]->ClearText();
    pGameUI->simple_message_frames[where]->AddText(jass::from_trigstring(jass::from_string(message)), color, *duration, 0);
    return true;
}

init_L(GameUI_Message) {
    jass::japi_add((uint32_t)X_DisplayMessage,     "X_DisplayMessage",     "(SRII)B");
    jass::japi_add((uint32_t)X_ClearMessage,       "X_ClearMessage",       "(I)B");
    jass::japi_add((uint32_t)X_SetMessage,         "X_SetMessage",         "(SRII)B");
}