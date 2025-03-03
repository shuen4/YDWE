#include <base/hook/inline.h>

#include "util.h"

uint32_t(__thiscall* real_CAbilityChannel_GetFlag)(CAbilityChannel* pAbilityChannel);
uint32_t __fastcall fake_CAbilityChannel_GetFlag(CAbilityChannel* pAbilityChannel) {
    uint32_t flag = base::this_call<uint32_t>(real_CAbilityChannel_GetFlag, pAbilityChannel);
    uint32_t dataC = (uint32_t)pAbilityChannel->GetDataC();
    if (dataC & 1 << 5)
        flag |= 1 << 25;
    return flag;
}

init_L(ObjectEditor_Channel_ChannelFlag) {
    real_CAbilityChannel_GetFlag = FUNC::CAbilityChannel_GetFlag;
    base::hook::install((uint32_t*)&real_CAbilityChannel_GetFlag, (uint32_t)fake_CAbilityChannel_GetFlag);
}