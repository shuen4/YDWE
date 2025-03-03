#include <warcraft3/jass/hook.h>

#include "util.h"

uint32_t __cdecl X_LoadHandleId(uint32_t hTable, uint32_t parentKey, uint32_t childKey) {
    CGameHashTable* pGameHashTable = ConvertHandle<CGameHashTable>(hTable);

    CGameHashTableManager* pGameHashTableManager = CGameWar3::GetInstance()->HashTableManager;
    if (!pGameHashTableManager)
        return 0;
    return pGameHashTableManager->GetStoredHandle(pGameHashTable->key, parentKey, childKey);
}

// type:
//      1: 所有引用计数的类型 (会改变计数)
//      2: texttag
//      3: lightning
//      4: image
//      5: ubersplat
//      6: fogstate
uint32_t __cdecl X_SaveHandleIdEx(uint32_t hTable, uint32_t parentKey, uint32_t childKey, uint32_t handleId, uint32_t type) {
    CGameHashTable* pGameHashTable = ConvertHandle<CGameHashTable>(hTable);

    CGameHashTableManager* pGameHashTableManager = CGameWar3::GetInstance()->HashTableManager;
    if (!pGameHashTableManager)
        return false;
    
    pGameHashTableManager->StoreHandle(pGameHashTable->key, parentKey, childKey, handleId, type);
    return true;
}

// 不改变计数
uint32_t __cdecl X_SaveHandleId(uint32_t hTable, uint32_t parentKey, uint32_t childKey, uint32_t handleId) {
    return X_SaveHandleIdEx(hTable, parentKey, childKey, handleId, 6);
}

init_L(Hashtable) {
    jass::japi_add((uint32_t)X_LoadHandleId,       "X_LoadHandleId",       "(Hhashtable;II)I");
    jass::japi_add((uint32_t)X_SaveHandleId,       "X_SaveHandleId",       "(Hhashtable;III)B");
    jass::japi_add((uint32_t)X_SaveHandleIdEx,     "X_SaveHandleIdEx",     "(Hhashtable;IIII)B");
}