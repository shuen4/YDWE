#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>

namespace warcraft3::japi {

	uint32_t __cdecl EXBitAnd(jass::jinteger_t a, jass::jinteger_t b) {
		return a & b;
	}

	uint32_t __cdecl EXBitOr(jass::jinteger_t a, jass::jinteger_t b) {
		return a | b;
	}

	uint32_t __cdecl EXBitXor(jass::jinteger_t a, jass::jinteger_t b) {
		return a ^ b;
	}

	uint32_t __cdecl EXIsBitSet(jass::jinteger_t a, jass::jinteger_t b) {
		return (a & b) == b;
	}


	void InitializeBit()
	{
		jass::japi_add((uintptr_t)EXBitAnd, "EXBitAnd", "(II)I");
		jass::japi_add((uintptr_t)EXBitOr, "EXBitOr", "(II)I");
		jass::japi_add((uintptr_t)EXBitXor, "EXBitXor", "(II)I");
		jass::japi_add((uintptr_t)EXIsBitSet, "EXIsBitSet", "(II)B");
	}
}
