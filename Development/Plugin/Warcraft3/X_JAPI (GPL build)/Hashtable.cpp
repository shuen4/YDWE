#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t searchLoadHandleId() {
    uint32_t ptr;
    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "LoadPlayerHandle"
    // mov      ecx, [LoadPlayerHandle函数的地址]  <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("LoadPlayerHandle");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  LoadPlayerHandle:
    //    ...
    //    call      LoadPlayerHandle1              <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    //=========================================
    // (3)
    //  LoadPlayerHandle1:
    //    ...
    //    call      ConvertHandle
    //    ...
    //    call      GetGameHashTableManager
    //    ...
    //    call      GetHashTableHandleId           <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    return ptr;
}

uint32_t searchSaveHandleId() {
    uint32_t ptr;
    //=========================================
    // (1)
    //
    // push     "()V"
    // mov      edx, "SavePlayerHandle"
    // mov      ecx, [SavePlayerHandle函数的地址]  <----
    // call     BindNative
    //=========================================
    ptr = get_war3_searcher().search_string("SavePlayerHandle");
    ptr = *(uint32_t*)(ptr + 0x05);

    //=========================================
    // (2)
    //  SavePlayerHandle:
    //    ...
    //    call      SavePlayerHandle1              <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    //=========================================
    // (3)
    //  SavePlayerHandle1:
    //    ...
    //    call      ConvertHandle (hashtable)
    //    ...
    //    call      ConvertHandle (player)
    //    ...
    //    call      GetGameHashTableManager
    //    ...
    //    call      SaveHashTableHandleId          <----
    //    
    //=========================================
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr = convert_function(ptr);

    return ptr;
}

uint32_t __cdecl X_LoadHandleId(uint32_t hTable, uint32_t parentKey, uint32_t childKey) {

    static uint32_t LoadHandleId = searchLoadHandleId();

    uint32_t table = handle_to_object(hTable);
    if (table) {
        uint32_t ptr = ReadMemory(get_war3_searcher().get_gamewar3() + 0x404);
        if (ptr)
            return base::this_call<uint32_t>(LoadHandleId, ptr, ReadMemory(table + 0x24), parentKey, childKey);
    }
    return 0;
}

// type:
//      1: 所有引用计数的类型 (会改变计数)
//      2: texttag
//      3: lightning
//      4: image
//      5: ubersplat
//      6: fogstate
uint32_t __cdecl X_SaveHandleIdEx(uint32_t hTable, uint32_t parentKey, uint32_t childKey, uint32_t handleId, uint32_t type) {

    static uint32_t SaveHandleId = searchSaveHandleId();

    uint32_t table = handle_to_object(hTable);
    if (table) {
        uint32_t ptr = ReadMemory(get_war3_searcher().get_gamewar3() + 0x404);
        if (ptr)
            return base::this_call<uint32_t>(SaveHandleId, ptr, ReadMemory(table + 0x24), parentKey, childKey, handleId, type);
    }
    return 0;
}

// 不改变计数
uint32_t __cdecl X_SaveHandleId(uint32_t hTable, uint32_t parentKey, uint32_t childKey, uint32_t handleId) {
    return X_SaveHandleIdEx(hTable, parentKey, childKey, handleId, 6);
}

init(Hashtable) {
    jass::japi_add((uint32_t)X_LoadHandleId,       "X_LoadHandleId",       "(Hhashtable;II)I");
    jass::japi_add((uint32_t)X_SaveHandleId,       "X_SaveHandleId",       "(Hhashtable;III)B");
    jass::japi_add((uint32_t)X_SaveHandleIdEx,     "X_SaveHandleIdEx",     "(Hhashtable;IIII)B");
}