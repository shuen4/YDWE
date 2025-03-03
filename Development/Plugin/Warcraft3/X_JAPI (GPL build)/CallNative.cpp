#include <base/hook/inline.h>

#include "util.h"

uint32_t __fastcall fake_JassInstance_CallNative(JassInstance* _this, uint32_t, char* funcName) {
    JassThreadLocal* pJassThreadLocal = (JassThreadLocal*)FUNC::GetInstance(InstanceType::JASS);
    NativeFunc* pNativeFunc = pJassThreadLocal->stringhash_to_nf.find_by_hash(FUNC::SStrHash(funcName));
    const char* param = pNativeFunc->param + 1; // 跳过前面的"("
    JassFrame* stack = _this->stackframe.next;
    uint32_t param_count = pNativeFunc->param_count;
    std::vector<uint32_t> args(param_count);
    std::vector<float> args_float(param_count);
    uint32_t tmp;

    while (param_count) {
        ScriptData* var = stack->stack.data[(stack->current - param_count--)];
        switch (*param++) {
        case 'C': {
            if (var->var_type == ScriptData::VARIABLE_TYPE::CODE)
                args[param_count] = FUNC::JassInstance_ConvertCode(_this, var->value);
            else
                args[param_count] = 0;
            break;
        }
        case 'R':
            args_float[param_count] = var->value; // 防止nf修改参数
            args[param_count] = (uint32_t)&args_float[param_count];
            break;
        case 'S':
            args[param_count] = (uint32_t)&_this->strings->data[(uint32_t)var->value];
            break;
        case 'H':
            if (param_count) {// 最后一个参数没必要循环
                while (*++param != ';')
                    ;
                ++param;
            }
            args[param_count] = var->value;
            break;
        default:
            args[param_count] = var->value;
            break;
        }
    }
    stack->current -= pNativeFunc->param_count;

    for (auto i = args.begin(); i != args.end(); i++) {
        tmp = (*i);
        _asm push tmp;
    }

    _asm mov tmp, esp;
    tmp += pNativeFunc->param_count * 4;
    ((uint32_t(__cdecl*)())(pNativeFunc->address))();
    _asm {
        mov esp, tmp;
        mov tmp, eax;
    }

    return tmp;
}

// 原生只支持15个参数, 大于15会造成缓冲区溢出
init_L(Fix_JassInstance_CallNative) {
    uint32_t real_JassInstance_CallNative = (uint32_t)FUNC::JassInstance_CallNative;
    base::hook::install(&real_JassInstance_CallNative, (uint32_t)fake_JassInstance_CallNative);
}