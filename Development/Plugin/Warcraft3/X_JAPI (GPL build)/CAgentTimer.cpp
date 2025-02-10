#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include "CAgentTimer.h"
#include "util.h"

uint32_t searchCAgentTimer_Start() {
    war3_searcher& s = get_war3_searcher();
    uint32_t str = s.search_string_ptr("EffectDeathTime", sizeof("EffectDeathTime"));

    for (uint32_t ptr = s.search_int_in_text(str); ptr; ptr = s.search_int_in_text(str, ptr + 1)) {
        uint32_t func = s.current_function(ptr);
        if (ptr - func > 0x10) {
            ptr += 4;
            ptr = next_opcode(ptr, 0xE8, 5);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            return convert_function(ptr);
        }
    }

    return 0;
}
uint32_t searchCAgentTimer_Stop() {
    uint32_t ptr = get_war3_searcher().search_string("PauseTimer");
    ptr = ReadMemory(ptr + 0x05);
    ptr = next_opcode(ptr, 0xE9, 5);
    ptr = convert_function(ptr);
    ptr = next_opcode(ptr, 0xE9, 5);
    return convert_function(ptr);
}

void CAgentTimer_Start(uint32_t pAgentTimer, float* duration, uint32_t eventID, uint32_t target, uint32_t periodic, uint32_t async) {
    static uint32_t pCAgentTimer_Start = searchCAgentTimer_Start();
    base::this_call<void>(pCAgentTimer_Start, pAgentTimer, duration, eventID, target, periodic, async);
}
void CAgentTimer_Stop(uint32_t pAgentTimer) {
    static uint32_t pCAgentTimer_Stop = searchCAgentTimer_Stop();
    base::this_call<void>(pCAgentTimer_Stop, pAgentTimer);
}