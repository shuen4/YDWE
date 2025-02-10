#pragma once
#include <stdint.h>
#include <Windows.h>

// 只是为了方便理解
template<class t = uint32_t>
inline t ReadMemory(uint32_t addr) {
	return *(t*)addr;
}
template<class t = uint32_t>
inline void WriteMemory(uint32_t addr, t value) {
	*(t*)addr = value;
}
template<class t = uint32_t>
void WriteMemoryEx(uint32_t addr, t value) {
    DWORD old;
    VirtualProtect((void*)addr, sizeof(t), PAGE_EXECUTE_READWRITE, &old); // 不检查成功与否 因为失败了游戏可能就会异步 不如直接崩游戏
    WriteMemory(addr, value);
    VirtualProtect((void*)addr, sizeof(t), old, &old);
    FlushInstructionCache(GetCurrentProcess(), (void*)addr, sizeof(t)); // 安全起见还是加上好了
}
template<class t>
inline void PatchCallRelative(uint32_t addr, t func) {
    WriteMemoryEx(addr + 1, (uint32_t)func - addr - 5);
}