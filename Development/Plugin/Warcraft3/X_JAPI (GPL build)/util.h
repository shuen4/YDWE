#pragma once
#include <vector>

#include "war3.h"

extern std::vector<bool(*)()>* init_H_func;
extern std::vector<void(*)()>* init_M_func;
extern std::vector<void(*)()>* init_L_func;
struct init_H {
    init_H(bool(*func)()) {
        if (!init_H_func)
            init_H_func = new std::vector<bool(*)()>();
        init_H_func->push_back(func);
    };
};
struct init_M {
    init_M(void(*func)()) {
        if (!init_M_func)
            init_M_func = new std::vector<void(*)()>();
        init_M_func->push_back(func);
    };
};
struct init_L {
    init_L(void(*func)()) {
        if (!init_L_func)
            init_L_func = new std::vector<void(*)()>();
        init_L_func->push_back(func);
    };
};

#define init_H(x) bool Init_H_##x(); init_H init_H_##x(Init_H_##x); bool Init_H_##x()
#define init_M(x) void Init_M_##x(); init_M init_M_##x(Init_M_##x); void Init_M_##x()
#define init_L(x) void Init_L_##x(); init_L init_L_##x(Init_L_##x); void Init_L_##x()

using namespace warcraft3;
using namespace war3;
