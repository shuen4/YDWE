#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/event.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t search_jass_vmmain() {
    war3_searcher& s = get_war3_searcher();
    uint32_t ptr = 0;

    //=========================================
    //  (1)
    //
    //    push    493E0h
    //    push    1
    //    push    1
    //    push    0
    //    mov     edx, offset s_Config ; "config"
    //    mov     ecx, esi
    //    call    UnknowFunc  <----
    //=========================================
    ptr = s.search_string("config");
    ptr += sizeof uint32_t;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);
    //=========================================
    //  (2)
    //
    //  UnknowFunc:
    //    push    esi
    //    mov     esi, edx
    //    call    GetVMInstance
    //    cmp     [esp+4+arg_8], 0
    //    mov     ecx, eax
    //    jz      short loc_6F44C170
    //    cmp     dword ptr [ecx+20h], 0
    //    jz      short loc_6F44C170
    //    call    UnknowFunc2         <----
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);
    //=========================================
    //  (3)
    //
    //  UnknowFunc2:
    //    mov     eax, [ecx+20h]
    //    push    0
    //    push    493E0h
    //    push    0
    //    push    eax
    //    call    JassVMMain    <----
    //    retn
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);
    return ptr;
}

uint32_t search_jass_executed_opcode_check() {
    uint32_t ptr = search_jass_vmmain();
    ptr = next_opcode(ptr, 0x0F, 6);
    while (*(unsigned char*)(ptr + 1) != 0x8C) {
        ptr += 6;
        ptr = next_opcode(ptr, 0x0F, 6);
    }
    return ptr;
}

uint32_t get_jass_executed_opcode_check() {
    static uint32_t ret = search_jass_executed_opcode_check();
    return ret;
}
uint32_t real_jass_executed_opcode_check = 0;
uint32_t real_jass_executed_opcode_add = 0;
std::vector<uint8_t> jass_executed_opcode_add_orig;

void init_search_jass_executed_opcode_add() {
    uint32_t stop = get_jass_executed_opcode_check();
    uint32_t last_4_opcode = search_jass_vmmain();
    uint32_t last_3_opcode = next_opcode(last_4_opcode);
    uint32_t last_2_opcode = next_opcode(last_3_opcode);
    uint32_t last_opcode = next_opcode(last_2_opcode);
    while (last_opcode != stop) {
        last_4_opcode = last_3_opcode;
        last_3_opcode = last_2_opcode;
        last_2_opcode = last_opcode;
        last_opcode = next_opcode(last_2_opcode);
    }
    real_jass_executed_opcode_add = last_4_opcode;
    jass_executed_opcode_add_orig.resize(last_3_opcode - last_4_opcode);
    memcpy(&jass_executed_opcode_add_orig[0], (void*)last_4_opcode, last_3_opcode - last_4_opcode);
}

void __cdecl X_EnableOpcodeLimit(uint32_t flag) {
    if (flag)
        WriteMemoryEx<uint16_t>(real_jass_executed_opcode_check, 0x8C0F); // jl
    else
        WriteMemoryEx<uint16_t>(real_jass_executed_opcode_check, 0xE990); // NOP, jmp
}

void __cdecl X_EnableOpcodeCounter(uint32_t flag) {
    DWORD old;
    VirtualProtect((void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size(), PAGE_EXECUTE_READWRITE, &old);
    if (flag)
        memcpy((void*)real_jass_executed_opcode_add, &jass_executed_opcode_add_orig[0], jass_executed_opcode_add_orig.size()); // 1.26 add eax, 1 | 1.27 inc esi
    else
        memset((void*)real_jass_executed_opcode_add, 0x90, jass_executed_opcode_add_orig.size());
    VirtualProtect((void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size(), old, &old);
    FlushInstructionCache(GetCurrentProcess(), (void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size());
}

void __cdecl X_SetOpcodeBehavior(uint32_t enableCheck, uint32_t enableCounter) {
    X_EnableOpcodeLimit(enableCheck);
    X_EnableOpcodeCounter(enableCounter);
}

init(JassVM) {
    real_jass_executed_opcode_check = get_jass_executed_opcode_check();
    init_search_jass_executed_opcode_add();
    jass::japi_add((uint32_t)X_EnableOpcodeLimit,   "X_EnableOpcodeLimit",   "(B)V");
    jass::japi_add((uint32_t)X_EnableOpcodeCounter, "X_EnableOpcodeCounter", "(B)V");
    jass::japi_add((uint32_t)X_SetOpcodeBehavior,   "X_SetOpcodeBehavior",   "(BB)V");
    event_game_reset([]() {
        X_SetOpcodeBehavior(true, true);
    });
}
