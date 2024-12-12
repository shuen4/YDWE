#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>
#include <warcraft3/event.h>

namespace warcraft3::japi {
    uintptr_t search_jass_vmmain()
    {
        war3_searcher& s = get_war3_searcher();
        uintptr_t ptr = 0;

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
        ptr += sizeof uintptr_t;
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

    uintptr_t search_jass_executed_opcode_check() {
        uintptr_t ptr = search_jass_vmmain();
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
    uintptr_t real_jass_executed_opcode_check = 0;
    uintptr_t real_jass_executed_opcode_add = 0;
    std::vector<uint8_t> jass_executed_opcode_add_orig;

    void init_search_jass_executed_opcode_add() {
        uint32_t stop = get_jass_executed_opcode_check();
        uintptr_t last_4_opcode = search_jass_vmmain();
        uintptr_t last_3_opcode = next_opcode(last_4_opcode);
        uintptr_t last_2_opcode = next_opcode(last_3_opcode);
        uintptr_t last_opcode = next_opcode(last_2_opcode);
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

    void __cdecl EXEnableOpcodeLimit(uint32_t flag) {
        if (flag)
            WriteMemoryEx<uint16_t>(real_jass_executed_opcode_check, 0x8C0F); // jl
        else
            WriteMemoryEx<uint16_t>(real_jass_executed_opcode_check, 0xE990); // NOP, jmp
    }

    void __cdecl EXEnableOpcodeCounter(uint32_t flag) {
        DWORD old;
        VirtualProtect((void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size(), PAGE_EXECUTE_READWRITE, &old);
        if (flag)
            memcpy((void*)real_jass_executed_opcode_add , &jass_executed_opcode_add_orig[0], jass_executed_opcode_add_orig.size()); // 1.26 add eax, 1 | 1.27 inc esi
        else
            for (uint32_t i = 0; i < jass_executed_opcode_add_orig.size(); i++)
                WriteMemory<uint8_t>(real_jass_executed_opcode_add + i, 0x90); // NOP
        VirtualProtect((void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size(), old, &old);
        FlushInstructionCache(GetCurrentProcess(), (void*)real_jass_executed_opcode_add, jass_executed_opcode_add_orig.size());
    }

    void __cdecl EXSetOpcodeBehavior(uint32_t enableCheck, uint32_t enableCounter) {
        EXEnableOpcodeLimit(enableCheck);
        EXEnableOpcodeCounter(enableCounter);
    }

    void InitializeJassVM() {
        real_jass_executed_opcode_check = get_jass_executed_opcode_check();
        init_search_jass_executed_opcode_add();
        jass::japi_add((uint32_t)EXEnableOpcodeLimit,   "EXEnableOpcodeLimit",   "(B)Z");
        jass::japi_add((uint32_t)EXEnableOpcodeCounter, "EXEnableOpcodeCounter", "(B)Z");
        jass::japi_add((uint32_t)EXSetOpcodeBehavior,   "EXSetOpcodeBehavior",   "(BB)Z");
        event_game_reset([]() {
            EXSetOpcodeBehavior(true, true);
        });
    }
}