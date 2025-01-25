#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include "init_util.h"

namespace warcraft3::japi {

	uintptr_t searchGetTerrainZ() {
		uintptr_t ptr;

		//=========================================
		// (1)
		//
		// push		"()V"
		// mov		edx, "GetLocationZ"
		// mov		ecx, [GetLocationZº¯ÊýµÄµØÖ·]  <----
		// call		BindNative
		//=========================================
		ptr = get_war3_searcher().search_string("GetLocationZ");
		ptr = *(uintptr_t*)(ptr + 0x05);

		//=========================================
		// (2)
		//  GetLocationZ:
		//    ...
		//    call		ConvertLocationHandle
		//    ...
		//    call      GetLocationZ               <----
		//    
		//=========================================
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr += 5;
		ptr = next_opcode(ptr, 0xE8, 5);
		ptr = convert_function(ptr);

		return ptr;
	}

	uint32_t __cdecl X_GetTerrainZ(jass::jreal_t* x, jass::jreal_t* y) {

		static uint32_t GetTerrainZ = searchGetTerrainZ();

		return jass::to_real((float)base::fast_call<double>(GetTerrainZ, -1, 0, jass::from_real(*x), jass::from_real(*y), 1));
	}

    init(Terrain) {
		jass::japi_add((uintptr_t)X_GetTerrainZ,		"X_GetTerrainZ",		"(RR)R");
	}
}
