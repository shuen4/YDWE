#pragma once
#include <vector>

extern std::vector<void(*)()>* init_func;
struct init {
    init(void(*func)()) {
        if (!init_func)
            init_func = new std::vector<void(*)()>();
        init_func->push_back(func);
    };
};

using namespace warcraft3;
#define init(x) void Initialize_##x(); init initialize_##x(Initialize_##x); void Initialize_##x()
#define type_check_s(obj, typeID) type_check(get_object_type(obj), typeID)