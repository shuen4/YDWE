#include <warcraft3/jass/nf_register.h>
#include <base/hook/inline.h>
#include <base/util/do_once.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>

namespace warcraft3::jass {
    void nfunction_add();
    void nfunction_hook();
}

namespace warcraft3::jass::nf_register {
    base::signal<void, void> event_hook;
    base::signal<void, void> event_add;

    uint32_t searchRegisterJassNatives() {
        auto& s = get_war3_searcher();
        return s.current_function(s.search_string("S2I"));
    }
    uint32_t searchRegisterType() {
        war3_searcher& s = get_war3_searcher();

        for (uint32_t ptr = s.search_int_in_text('wscd'); ptr; ptr = s.search_int_in_text('wscd', ptr + 1)) {
            ptr += 4;
            if (ReadMemory<uint8_t>(ptr) == 0xE8) // call
                return s.current_function(ptr);
        }
        return 0;
    }
    uint32_t real_RegisterJassNatives;
    void fake_RegisterJassNatives() {
        base::c_call<void>(real_RegisterJassNatives);
        event_add();
        nfunction_add();
        nfunction_hook();
    }
    uint32_t real_RegisterType;
    void fake_RegisterType() {
        base::c_call<void>(real_RegisterType);
        event_hook();
    }
    bool initialize() {
        DO_ONCE_NOTHREADSAFE() {
            real_RegisterJassNatives = searchRegisterJassNatives();
            base::hook::install(&real_RegisterJassNatives, (uint32_t)fake_RegisterJassNatives);
            real_RegisterType = searchRegisterType();
            base::hook::install(&real_RegisterType, (uint32_t)fake_RegisterType);
            return true;
        }
        return false;
    }
}
