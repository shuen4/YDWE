#include <warcraft3/jass/opcode.h>
#include <warcraft3/hashtable.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/inline.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>
#include <vector>
#include "util.h"

uint32_t searchJassVM_CallNative() {
    warcraft3::war3_searcher& game = warcraft3::get_war3_searcher();
    uint32_t addr = game.search_string("config");
    addr += sizeof uint32_t;

    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索下面第一个call
    addr = warcraft3::convert_function(addr);           //进入这个函数

    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索第一个call
    addr += 0x5;                                        //+5
    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索到第二个call
    addr = warcraft3::convert_function(addr);           //进入

    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索第一个call
    addr = warcraft3::convert_function(addr);           //进入

    addr = warcraft3::next_opcode(addr, 0xFF, 7);       //搜索第一个call
    addr += 0x3;
    addr = *(uint32_t*)addr;                           //取到跳转地址表地址
    addr += 0x4C;                                       //跳转到偏移0x13 * 4的地址处
    addr = *(uint32_t*)addr;
    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索第一个call
    addr += 0x5;                                        //+5
    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索到第二个call
    addr = warcraft3::convert_function(addr);           //进入

    return addr;

}

uint32_t getJassVM_CallNative (){
    static uint32_t ret = searchJassVM_CallNative();
    return ret;
}

uint32_t searchConvertFuncAddr() {
    uint32_t addr = getJassVM_CallNative();
    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索第一个call
    addr += 0x5;                                        //+5
    addr = warcraft3::next_opcode(addr, 0xE8, 5);       //搜索到第二个call
    addr = warcraft3::convert_function(addr);           //进入
    return addr;
}

uint32_t __fastcall fake_JassVM_CallNative(jass_vm_t* _this, uint32_t edx, char* funcName) {
    edx;
    hashtable::native_func_node* nf = get_native_function_hashtable()->find(funcName);
    char* param = (char*)nf->param_list_ + 1; // 跳过前面的"("
    stackframe_t* stack = _this->stackframe;
    uint32_t param_count = nf->param_num_;
    std::vector<uint32_t> args(param_count);
    std::vector<float> args_float(param_count);
    uint32_t tmp;
    while (param_count) {
        hashtable::variable_node* var = ReadMemory<hashtable::variable_node*>((uint32_t)stack + 4 * (stack->index - param_count--) + 12);
        switch (*param++) {
        case 'C': {
            static uint32_t convertFuncAddr = searchConvertFuncAddr();
            if (var->array_ == jass::OPCODE_VARIABLE_TYPE::OPCODE_VARIABLE_CODE)
                args[param_count] = base::this_call<int>(convertFuncAddr, _this, var->value_);
            else
                args[param_count] = 0;
            break;
        }
        case 'R':
            args_float[param_count] = *(float*)&var->value_; // 防止nf修改参数
            args[param_count] = (uint32_t)&args_float[param_count];
            break;
        case 'S':
            args[param_count] = _this->string_table->get_no_ref(var->value_);
            break;
        case 'H':
            if (param_count) {// 最后一个参数没必要循环
                while (*++param != ';')
                    ;
                ++param;
            }
            args[param_count] = var->value_;
            break;
        default:
            args[param_count] = var->value_;
            break;
        }
    }
    stack->index -= nf->param_num_;

    for (auto i = args.begin(); i != args.end(); i++) {
        tmp = (*i);
        _asm push tmp;
    }

    /*
    tmp = nf->param_num_ * 4;
    ((uint32_t(__cdecl*)())(nf->func_address_))();
    _asm {
        add esp, tmp;
        mov tmp, eax;
    }
    */

    _asm mov tmp, esp; // return bug 可能会修改函数指针为调用stdcall函数, 这边先计算并保存一下stack指针
    tmp += nf->param_num_ * 4;
    ((uint32_t(__cdecl*)())(nf->func_address_))();
    _asm {
        mov esp, tmp;
        mov tmp, eax;
    }

    return tmp;
}

// 原生只支持15个参数, 大于15会造成缓冲区溢出
init(Fix_JassVM_CallNative) {
    uint32_t real_JassVM_CallNative = getJassVM_CallNative();
    base::hook::install(&real_JassVM_CallNative, (uint32_t)fake_JassVM_CallNative);
}