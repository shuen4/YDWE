#pragma once

#include <warcraft3/config.h>
#include <functional>

namespace warcraft3 {
    _WAR3_API void event_game_reset(std::function<void()> func);
    _WAR3_API void event_agent_destructor(void(*func)(uint32_t));
}
