#include <warcraft3/event.h>
#include <warcraft3/jass/nf_register.h>
#include <warcraft3/war3_searcher.h>
#include <base/util/memory.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>

namespace warcraft3 {
	void event_game_reset(std::function<void()> func)
	{
		jass::nf_register::initialize();
		jass::nf_register::event_hook.connect(func);
	}

    std::vector<void(*)(uint32_t)> agent_destructor_callback;

    uint32_t real_CAgent_destructor;
    uint32_t __fastcall fake_CAgent_destructor(uint32_t _this) {
        for (auto i = agent_destructor_callback.begin(); i != agent_destructor_callback.end(); i++)
            (*i)(_this);
        return base::this_call<uint32_t>(real_CAgent_destructor, _this);
    }
    void event_agent_destructor(void(*func)(uint32_t)) {
        if (agent_destructor_callback.size() == 0) {
            real_CAgent_destructor = convert_function(next_opcode(ReadMemory(get_vfn_ptr(".?AVCAgent@@") + 0x4), 0xE8, 5));
            base::hook::install(&real_CAgent_destructor, (uint32_t)fake_CAgent_destructor);
        }
        agent_destructor_callback.push_back(func);
    }
}
