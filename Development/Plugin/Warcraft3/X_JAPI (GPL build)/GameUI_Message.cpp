#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>
#include "init_util.h"

namespace warcraft3::japi {

uintptr_t searchClearMessage() {
	uintptr_t ptr;

	//=========================================
	// (1)
	//
	// push		"()V"
	// mov		edx, "ClearTextMessages"
	// mov		ecx, [ClearTextMessages函数的地址] <----
	// call		BindNative
	//=========================================
	ptr = get_war3_searcher().search_string("ClearTextMessages");
	ptr = *(uintptr_t*)(ptr + 0x05);

	//=========================================
	// (2)
	//  ClearTextMessages:
	//    xor		edx, edx
	//    lea		ecx, [edx + 1]
	//	  call		GetGameUI
	//    mov		ecx, eax
	//    jmp		ClearTextMessageFrame		   <----
	//=========================================
	ptr = next_opcode(ptr, 0xE9, 5);
	ptr = convert_function(ptr);

	//=========================================
	// (3)
	//  ClearTextMessageFrame:
	//    mov		ecx, [ecx + 0x3E8]
	//    jmp		ClearMessage				   <----
	//=========================================
	ptr = next_opcode(ptr, 0xE9, 5);
	ptr = convert_function(ptr);

	return ptr;
}

uint32_t __cdecl X_GetColor(uint32_t a, uint32_t r, uint32_t g, uint32_t b) {
	return (b | (g << 8) | (r << 16) | a << 24);
}

// 透明度有效 但时间一到渐变会重置透明度
// 位置:
// 0 错误消息 (魔法值不够, ...)
// 1 游戏消息 (DisplayTextToPlayer, ...)
// 2 聊天消息 (X_DisplayChat, ...)
// 3 维修等级 (低维修等级, ...)
void __cdecl X_DisplayMessage(uint32_t message, jass::jreal_t* duration, uint32_t where, uint32_t color) {
	if (where > 3)
		return;

	uint32_t CGameUI = get_war3_searcher().get_gameui(0, 0);

	if (CGameUI != 0) {
		uint32_t pFrame = *(uint32_t*)(CGameUI + 0x3E4 + 4 * where);
		base::this_call<void>(*(uint32_t*)(*(uint32_t*)pFrame + 108), pFrame, jass::from_trigstring(jass::from_string(message)), &color, jass::from_real(*duration), 0);
	}
}

// 位置同上
void __cdecl X_ClearMessage(uint32_t where) {
	if (where > 3)
		return;

	static uint32_t ClearMessage = searchClearMessage();
	uint32_t CGameUI = get_war3_searcher().get_gameui(0, 0);

	if (CGameUI != 0)
		base::this_call<void>(ClearMessage, *(uint32_t*)(CGameUI + 0x3E4 + 4 * where));
}

// 懒人版 (可能性能比在JASS用两个nf好)
// 位置同上
void __cdecl X_SetMessage(uint32_t message, jass::jreal_t* duration, uint32_t where, uint32_t color) {
	X_ClearMessage(where);
	X_DisplayMessage(message, duration, where, color);
}

init(GameUI_Message) {
	jass::japi_add((uintptr_t)X_GetColor,			"X_GetColor",			"(IIII)I");
	jass::japi_add((uintptr_t)X_DisplayMessage,		"X_DisplayMessage",		"(SRII)V");
	jass::japi_add((uintptr_t)X_ClearMessage,		"X_ClearMessage",		"(I)V");
	jass::japi_add((uintptr_t)X_SetMessage,			"X_SetMessage",			"(SRII)V");
}
}
