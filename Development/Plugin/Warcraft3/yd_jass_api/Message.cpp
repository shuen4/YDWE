#include <warcraft3/jass.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/war3_searcher.h>
#include <base/hook/fp_call.h>
#include <string>

namespace warcraft3::japi {


uintptr_t searchInGameChatWhat()
{
	//=========================================
	// (1)
	//
	//  mov     ecx, "InGameChatWhat" 
	//  call    sub_6F3304C0
	//  ds:flt_6F946378
	//  mov     eax, [esi+98h]
	//  movzx   edx, byte ptr [esi+15h]
	//  push    ecx             ; txtTime
	//  lea     ecx, [esp+114h+text]
	//  fstp    [esp+114h+var_114]
	//  push    eax             ; ctype
	//  push    ecx             ; text
	//  push    edx             ; PlayerID
	//  mov     ecx, ebx        ; this
	//  call    InGameChatWhat                  <----
	//=========================================
	uintptr_t ptr = get_war3_searcher().search_string("InGameChatWhat");
	ptr += 0x04;
	ptr = next_opcode(ptr, 0xE8, 5);
	ptr += 0x05;
	ptr = next_opcode(ptr, 0xE8, 5);
	return convert_function(ptr);
}

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

void __cdecl EXDisplayChat(uint32_t player_handle, uint32_t chat_recipient, uint32_t message) {
	static uintptr_t InGameChatWhat = searchInGameChatWhat();

	uint32_t CGameUI = get_war3_searcher().get_gameui(0, 0);
	if (CGameUI != 0)
	{
        base::this_call<void>(InGameChatWhat, CGameUI, jass::call("GetPlayerId", player_handle), jass::from_trigstring(jass::from_string(message)), chat_recipient, (float)10.0f);
	}
}

uint32_t __cdecl EXGetColor(uint32_t a, uint32_t r, uint32_t g, uint32_t b) {
	return (b | (g << 8) | (r << 16) | a << 24);
}

// 透明度有效 但时间一到渐变会重置透明度
// 位置:
// 0 错误消息 (魔法值不够, ...)
// 1 游戏消息 (DisplayTextToPlayer, ...)
// 2 聊天消息 (EXDisplayChat, ...)
// 3 维修等级 (低维修等级, ...)
void __cdecl EXDisplayMessage(uint32_t message, jass::jreal_t* duration, uint32_t where, uint32_t color) {
	if (where > 3)
		return;

	uint32_t CGameUI = get_war3_searcher().get_gameui(0, 0);

	if (CGameUI != 0) {
		uint32_t pFrame = *(uint32_t*)(CGameUI + 0x3E4 + 4 * where);
		base::this_call<void>(*(uint32_t*)(*(uint32_t*)pFrame + 108), pFrame, jass::from_trigstring(jass::from_string(message)), &color, jass::from_real(*duration), 0);
	}
}

// 位置同上
void __cdecl EXClearMessage(uint32_t where) {
	if (where > 3)
		return;

	static uint32_t ClearMessage = searchClearMessage();
	uint32_t CGameUI = get_war3_searcher().get_gameui(0, 0);

	if (CGameUI != 0)
		base::this_call<void>(ClearMessage, *(uint32_t*)(CGameUI + 0x3E4 + 4 * where));
}

// 懒人版 (可能性能比在JASS用两个nf好)
// 位置同上
void __cdecl EXSetMessage(uint32_t message, jass::jreal_t* duration, uint32_t where, uint32_t color) {
	EXClearMessage(where);
	EXDisplayMessage(message, duration, where, color);
}

void InitializeMessage()
{
	jass::japi_add((uintptr_t)EXDisplayChat, "EXDisplayChat", "(Hplayer;IS)V");
	jass::japi_add((uintptr_t)EXGetColor, "EXGetColor", "(IIII)I");
	jass::japi_add((uintptr_t)EXDisplayMessage, "EXDisplayMessage", "(SRII)V");
	jass::japi_add((uintptr_t)EXClearMessage, "EXClearMessage", "(I)V");
	jass::japi_add((uintptr_t)EXSetMessage, "EXSetMessage", "(SRII)V");
}
}
