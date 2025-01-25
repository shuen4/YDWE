#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include <base/util/memory.h>
#include "init_util.h"

namespace warcraft3::japi {

	uint32_t __cdecl X_BitAnd(jass::jinteger_t a, jass::jinteger_t b) {
		return a & b;
	}

	uint32_t __cdecl X_BitOr(jass::jinteger_t a, jass::jinteger_t b) {
		return a | b;
	}

	uint32_t __cdecl X_BitXor(jass::jinteger_t a, jass::jinteger_t b) {
		return a ^ b;
	}

	uint32_t __cdecl X_IsBitSet(jass::jinteger_t a, jass::jinteger_t b) {
		return (a & b) == b;
	}

    init(Bit) {
		jass::japi_add((uintptr_t)X_BitAnd,			"X_BitAnd",		"(II)I");
		jass::japi_add((uintptr_t)X_BitOr,			"X_BitOr",		"(II)I");
		jass::japi_add((uintptr_t)X_BitXor,			"X_BitXor",		"(II)I");
		jass::japi_add((uintptr_t)X_IsBitSet,		"X_IsBitSet",	"(II)B");
	}
}
