#include <vector>
#include <windows.h>

std::vector<void(*)()>* init_H_func = NULL;
std::vector<void(*)()>* init_M_func = NULL;
std::vector<void(*)()>* init_L_func = NULL;

// Development\Component\script\war3\main.lua line:68 
extern "C" __declspec(dllexport) void __cdecl InitializeLowPriority() {
    for (auto i = init_H_func->begin(); i != init_H_func->end(); i++)
        (*i)();
    delete init_H_func;

    for (auto i = init_M_func->begin(); i != init_M_func->end(); i++)
        (*i)();
    delete init_M_func;

    for (auto i = init_L_func->begin(); i != init_L_func->end(); i++)
        (*i)();
    delete init_L_func;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*pReserved*/) {
    return TRUE;
}