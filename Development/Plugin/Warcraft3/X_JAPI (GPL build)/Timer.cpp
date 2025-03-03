#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_TimerSetPeriodic(uint32_t timer, uint32_t flag) {
    CTimerWar3* pTimerWar3 = ConvertHandle<CTimerWar3>(timer);
    
    if (!pTimerWar3)
        return false;
    
    if (flag)
        pTimerWar3->timer.flag |= CAgentTimer::timer_flag_periodic;
    else
        pTimerWar3->timer.flag &= ~CAgentTimer::timer_flag_periodic;
    return true;
}

uint32_t __cdecl X_TimerSetTimeout(uint32_t timer, float* timeout) {
    CTimerWar3* pTimerWar3 = ConvertHandle<CTimerWar3>(timer);

    if (!pTimerWar3)
        return false;

    pTimerWar3->SetTimeout(*timeout);
    return true;
}

uint32_t __cdecl X_ResumeTimer(uint32_t timer) {
    CTimerWar3* pTimerWar3 = ConvertHandle<CTimerWar3>(timer);

    if (!pTimerWar3 || pTimerWar3->timer.IsRunning())
        return false;

    pTimerWar3->Start(pTimerWar3->timer.GetRemaining(), (bool)(pTimerWar3->timer.flag & CAgentTimer::timer_flag_periodic));
    pTimerWar3->timer.SetNextTimeout(pTimerWar3->timeout.value);
    return true;
}

uint32_t __cdecl X_PauseTimer(uint32_t timer) {
    CTimerWar3* pTimerWar3 = ConvertHandle<CTimerWar3>(timer);

    if (!pTimerWar3 || !pTimerWar3->timer.IsRunning())
        return false;

    pTimerWar3->remaining.value = pTimerWar3->timer.GetRemaining();
    bool is_periodic = pTimerWar3->timer.flag & CAgentTimer::timer_flag_periodic;
    pTimerWar3->timer.Stop();
    if (is_periodic)
        pTimerWar3->timer.flag |= CAgentTimer::timer_flag_periodic;
    return true;
}

uint32_t __cdecl X_DestroyTimer(uint32_t timer) {
    CTimerWar3* pTimerWar3 = ConvertHandle<CTimerWar3>(timer);

    if (!pTimerWar3)
        return false;

    if (pTimerWar3->timer.IsRunning())
        pTimerWar3->timer.Stop();

    pTimerWar3->Destroy();
    return true;
}

init_L(Timer) {
    jass::japi_add((uint32_t)X_TimerSetPeriodic,        "X_TimerSetPeriodic",       "(Htimer;B)B");
    jass::japi_add((uint32_t)X_TimerSetTimeout,         "X_TimerSetTimeout",        "(Htimer;R)B");
    jass::japi_add((uint32_t)X_ResumeTimer,             "X_ResumeTimer",            "(Htimer;)B");
    jass::japi_add((uint32_t)X_PauseTimer,              "X_PauseTimer",             "(Htimer;)B");
    jass::japi_add((uint32_t)X_DestroyTimer,            "X_DestroyTimer",           "(Htimer;)B");
}