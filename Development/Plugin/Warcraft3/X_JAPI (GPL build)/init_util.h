#pragma once
#include <vector>

extern std::vector<void(*)()> init_func;
struct init {
    init(void(*func)()) {
        init_func.push_back(func);
    };
};
#define init(x) void Initialize_##x(); init initialize_##x(Initialize_##x); void Initialize_##x()
