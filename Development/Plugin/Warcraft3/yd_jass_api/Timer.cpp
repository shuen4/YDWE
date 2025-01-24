#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>

namespace warcraft3::japi {

    uint32_t __cdecl EXTimerSetPeriodic(jass::jhandle_t timer, jass::jboolean_t flag) {
        uint32_t pTimerWar3 = handle_to_object(timer);
        if (pTimerWar3) {
            uint8_t timer_flag = ReadMemory<uint8_t>(pTimerWar3 + 0x34);
            if (flag)
                timer_flag = timer_flag | 0b10;
            else
                timer_flag = timer_flag & ~0b10;
            WriteMemory<uint8_t>(pTimerWar3 + 0x34, timer_flag);
            return 1;
        }
        return 0;
    }

    float CAgentTimerExtended_GetRemainingTime(uint32_t _this) {
        float out;
        base::this_call_vf<void>(_this, 0x18, &out);
        return out;
    }

    void CTimerWar3_SetTimeout(uint32_t pTimerWar3, float timeout) { // 不是魔兽本身的函数
        // pTimerWar3 + 0x3C 有个一样的不知道什么用
        WriteMemory<float>(pTimerWar3 + 0x48, timeout); // TimerGetTimeout
        uint32_t pAgentTimerExtended = pTimerWar3 + 0x24;
        uint32_t ptr = ReadMemory<uint32_t>(pAgentTimerExtended + 0xC);
        if (ptr)
            WriteMemory<float>(ptr + 0x8, timeout); // 下一次循环的计时
    }

    inline bool CTimerWar3_IsRunning(uint32_t pTimerWar3) {// 不是魔兽本身的函数
        return !!ReadMemory<uint32_t>(pTimerWar3 + 0x30);
    }

    uint32_t __cdecl EXTimerSetTimeout(jass::jhandle_t timer, float* timeout) {
        uint32_t pTimerWar3 = handle_to_object(timer);
        if (pTimerWar3) {
            CTimerWar3_SetTimeout(pTimerWar3, *timeout);
            return 1;
        }
        return 0;
    }

    uintptr_t searchCAgentTimer_Start();
    inline uintptr_t searchCAgentTimerExtended_Start() { // 两个的代码内容完全一致
        return searchCAgentTimer_Start();
    }

    uint32_t __cdecl EXResumeTimer(jass::jhandle_t timer) {
        uint32_t pTimerWar3 = handle_to_object(timer);
        if (pTimerWar3) {
            if (!CTimerWar3_IsRunning(pTimerWar3)) {
                uint32_t is_periodic = ReadMemory<uint8_t>(pTimerWar3 + 0x34) & 0b10 ? 1 : 0;
                float timeout = ReadMemory<float>(pTimerWar3 + 0x48);
                uint32_t pAgentTimerExtended = pTimerWar3 + 0x24;
                float remaining_time = CAgentTimerExtended_GetRemainingTime(pAgentTimerExtended);
                static uint32_t pAgentTimerExtended_Start = searchCAgentTimerExtended_Start();
                base::this_call<void>(pAgentTimerExtended_Start, pAgentTimerExtended, &remaining_time, 0x80204, pTimerWar3, is_periodic, 0);
                CTimerWar3_SetTimeout(pTimerWar3, timeout);
                return 1;
            }
        }
        return 0;
    }

    uint32_t __cdecl EXPauseTimer(jass::jhandle_t timer) {
        uint32_t pTimerWar3 = handle_to_object(timer);
        if (pTimerWar3) {
            if (CTimerWar3_IsRunning(pTimerWar3)) {
                uint32_t is_periodic = ReadMemory<uint8_t>(pTimerWar3 + 0x34) & 0b10 ? 1 : 0;
                jass::call("PauseTimer", timer); // 换成直接调用?
                if (is_periodic)
                    EXTimerSetPeriodic(timer, is_periodic);
                return 1;
            }
        }
        return 0;
    }

    uint32_t __cdecl EXDestroyTimer(jass::jhandle_t timer) {
        uint32_t pTimerWar3 = handle_to_object(timer);
        if (pTimerWar3 && type_check(get_object_type(pTimerWar3), '+tmr')) {
            jass::call("PauseTimer", timer); // 换成直接调用?
            jass::call("DestroyTimer", timer); // 换成直接调用?
            return true;
        }
        return false;
    }

    void InitializeTimer() {
        jass::japi_add((uintptr_t)EXTimerSetPeriodic, "EXTimerSetPeriodic", "(Htimer;B)B");
        jass::japi_add((uintptr_t)EXTimerSetTimeout,  "EXTimerSetTimeout",  "(Htimer;R)B");
        jass::japi_add((uintptr_t)EXResumeTimer,      "EXResumeTimer",      "(Htimer;)B");
        jass::japi_add((uintptr_t)EXPauseTimer,       "EXPauseTimer",       "(Htimer;)B");
        jass::japi_add((uintptr_t)EXDestroyTimer,     "EXDestroyTimer",     "(Htimer;)B");
    }
}
