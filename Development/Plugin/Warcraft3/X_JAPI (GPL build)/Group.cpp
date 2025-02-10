#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t __cdecl X_GroupGetSize(uint32_t group) {
    uint32_t pGroup = handle_to_object(group);
    if (!pGroup || !type_check_s(pGroup, '+grp'))
        return 0;
    return ReadMemory(pGroup + 0x34);
}

uint32_t searchCUnitSet_At() {
    uint32_t ptr = get_war3_searcher().search_string("FirstOfGroup");
    ptr = ReadMemory(ptr + 0x05);

    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 0x05;
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

uint32_t __cdecl X_GroupUnitAt(uint32_t group, uint32_t index) {
    uint32_t pGroup = handle_to_object(group);
    if (!pGroup || !type_check_s(pGroup, '+grp')/* || index >= ReadMemory(pGroup + 0x34) 原生函数会检查 */)
        return 0;
    static uint32_t pCUnitSet_At = searchCUnitSet_At();
    uint32_t pUnit = base::this_call<uint32_t>(pCUnitSet_At, pGroup + 0x24, index);
    if (!pUnit)
        return 0;
    return create_handle(pUnit);
}

init(Group) {
    jass::japi_add((uint32_t)X_GroupGetSize,       "X_GroupGetSize",    "(Hgroup;)I");
    jass::japi_add((uint32_t)X_GroupUnitAt,        "X_GroupUnitAt",     "(Hgroup;I)Hunit;");
}