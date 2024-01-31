#pragma once
#include <stdint.h>

// 只是为了方便理解
template<class t = uint32_t>
inline t ReadMemory(uint32_t addr) {
	return *(t*)addr;
}
template<class t = uint32_t>
inline void WriteMemory(uint32_t addr, t value) {
	*(t*)addr = value;
}