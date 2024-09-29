// w4454962 fix (issue 197)
#include <warcraft3/jass/trampoline.h>
#include <warcraft3/hashtable.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>

uintptr_t get_conver_func_addr()
{
	warcraft3::war3_searcher& game = warcraft3::get_war3_searcher();
	uintptr_t addr = game.search_string("config");
	addr += sizeof uintptr_t;

	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索下面第一个call
	addr = warcraft3::convert_function(addr);			//进入这个函数

	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索第一个call
	addr += 0x5;										//+5
	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索到第二个call
	addr = warcraft3::convert_function(addr);			//进入


	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索第一个call
	addr = warcraft3::convert_function(addr);			//进入

	addr += 0x8A;										//偏移到swatch跳转表
	addr = *(uintptr_t*)addr;							//取到跳转地址表地址
	addr += 0x48;										//跳转到偏移0x12 * 4的地址处
	addr = *(uintptr_t*)addr;
	addr += 0x22;										//偏移到目标函数位置
	addr = warcraft3::convert_function(addr);			//取目标地址

	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索第一个call
	addr += 0x5;										//+5
	addr = warcraft3::next_opcode(addr, 0xE8, 5);		//搜索到第二个call
	addr = warcraft3::convert_function(addr);			//进入

	return addr;

}

//将指定jass函数地址转换为code id
int convert_code_id(uintptr_t func_addr)
{
	static uintptr_t addr = get_conver_func_addr();
	uintptr_t jvm = (uintptr_t)warcraft3::get_jass_vm();
	return base::this_call<int>(addr, jvm, func_addr);
}

namespace warcraft3 { namespace jass {

	trampoline::trampoline()
		: mybase()
	{

	}


	uintptr_t trampoline::code() const
	{
		uintptr_t address = (uintptr_t)this;
		return convert_code_id(address);
	}
}}