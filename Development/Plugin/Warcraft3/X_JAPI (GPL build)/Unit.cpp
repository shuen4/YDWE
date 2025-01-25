#include <base/hook/fp_call.h>
#include <base/hook/iat.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/jass.h>
#include <cassert>	 
#include <warcraft3/hashtable.h>  
#include "init_util.h"

namespace warcraft3::japi {	  

	jass::jboolean_t _cdecl X_IsUnitInvulnerable(jass::jhandle_t unit)
	{
		uint32_t pUnit = handle_to_object(unit);

		if (pUnit)
			return *(uint32_t*)(pUnit + 0x20) & 0b00001000;

		return false;
	}
    
    init(Unit) {
		jass::japi_add((uintptr_t)X_IsUnitInvulnerable,		"X_IsUnitInvulnerable",		"(Hunit;)B");
	}
}
