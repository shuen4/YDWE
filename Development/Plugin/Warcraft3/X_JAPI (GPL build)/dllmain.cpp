#include <windows.h>
#include <vector>

std::vector<void(*)()> init_func;
extern "C" __declspec(dllexport) void __stdcall InitializeLowPriority() {
    for (auto i = init_func.begin(); i != init_func.end(); i++)
        (*i)();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*pReserved*/) {
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(module);
    return TRUE;
}