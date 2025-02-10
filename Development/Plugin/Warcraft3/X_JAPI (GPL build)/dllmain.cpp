#include <vector>
#include <windows.h>

std::vector<void(*)()>* init_func = NULL;
// Development\Component\script\war3\main.lua line:68
extern "C" __declspec(dllexport) void __cdecl InitializeLowPriority() {
    for (auto i = init_func->begin(); i != init_func->end(); i++)
        (*i)();
    delete init_func;
    init_func = NULL;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*pReserved*/) {
    return TRUE;
}