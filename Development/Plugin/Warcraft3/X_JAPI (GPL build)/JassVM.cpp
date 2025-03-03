#include <warcraft3/event.h>
#include <warcraft3/jass/hook.h>

#include "util.h"

std::vector<uint8_t> jass_executed_opcode_add_orig;

void __cdecl X_EnableOpcodeLimit(uint32_t flag) {
    if (flag)
        WriteMemoryEx<uint16_t>(ASM::JassInstance_check_opcode_counter, 0x8C0F); // jl
    else
        WriteMemoryEx<uint16_t>(ASM::JassInstance_check_opcode_counter, 0xE990); // NOP, jmp
}

void __cdecl X_EnableOpcodeCounter(uint32_t flag) {
    DWORD old;
    VirtualProtect((void*)ASM::JassInstance_add_opcode_counter, jass_executed_opcode_add_orig.size(), PAGE_EXECUTE_READWRITE, &old);
    if (flag)
        memcpy((void*)ASM::JassInstance_add_opcode_counter, &jass_executed_opcode_add_orig[0], jass_executed_opcode_add_orig.size()); // 1.26 add eax, 1 | 1.27 inc esi
    else
        memset((void*)ASM::JassInstance_add_opcode_counter, 0x90, jass_executed_opcode_add_orig.size());
    VirtualProtect((void*)ASM::JassInstance_add_opcode_counter, jass_executed_opcode_add_orig.size(), old, &old);
    FlushInstructionCache(GetCurrentProcess(), (void*)ASM::JassInstance_add_opcode_counter, jass_executed_opcode_add_orig.size());
}

void __cdecl X_SetOpcodeBehavior(uint32_t enableCheck, uint32_t enableCounter) {
    X_EnableOpcodeLimit(enableCheck);
    X_EnableOpcodeCounter(enableCounter);
}

init_L(JassVM) {
    jass_executed_opcode_add_orig.resize(ASM::JassInstance_add_opcode_counter_SIZE);
    memcpy(&jass_executed_opcode_add_orig[0], (void*)ASM::JassInstance_add_opcode_counter, ASM::JassInstance_add_opcode_counter_SIZE);

    jass::japi_add((uint32_t)X_EnableOpcodeLimit,           "X_EnableOpcodeLimit",          "(B)V");
    jass::japi_add((uint32_t)X_EnableOpcodeCounter,         "X_EnableOpcodeCounter",        "(B)V");
    jass::japi_add((uint32_t)X_SetOpcodeBehavior,           "X_SetOpcodeBehavior",          "(BB)V");

    event_game_reset([]() {
        X_SetOpcodeBehavior(true, true);
    });
}
