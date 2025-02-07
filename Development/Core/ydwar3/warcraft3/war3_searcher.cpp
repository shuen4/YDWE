#include <warcraft3/war3_searcher.h>
#include <warcraft3/version.h>
#include <warcraft3/hashtable.h>
#include <base/util/singleton.h>
#include <base/hook/fp_call.h>
#include <base/util/memory.h>

namespace warcraft3 {

	war3_searcher::war3_searcher()
		: _Mybase(::GetModuleHandleW(L"Game.dll"))
		, version_(search_version())
		, get_instance_(search_get_instance())
		, get_gameui_(search_get_gameui())
		, gamewar3_ptr_(search_gamestate_ptr())
	{
        search_create_object_by_type_id();
        search_create_handle();
    }
	
	war3_searcher::war3_searcher(HMODULE hGameDll)
		: _Mybase(hGameDll)
		, version_(search_version())
		, get_instance_(search_get_instance())
		, get_gameui_(search_get_gameui())
		, gamewar3_ptr_(search_gamestate_ptr())
    {
        search_create_object_by_type_id();
        search_create_handle();
    }
	
	uint32_t war3_searcher::get_version() const
	{
		return version_;
	}
	
	uint32_t war3_searcher::get_instance(uint32_t index)
	{
		return ((uint32_t(_fastcall*)(uint32_t))get_instance_)(index);
	}
	
	uint32_t war3_searcher::get_gameui(uint32_t createAndInitIfNotExist, uint32_t remove)
	{
		return ((uint32_t(_fastcall*)(uint32_t, uint32_t))get_gameui_)(createAndInitIfNotExist, remove);
	}

	uint32_t war3_searcher::get_gamewar3() {
		return *(uint32_t*)gamewar3_ptr_;
	}
	
	bool war3_searcher::is_gaming()
	{
		return (0 != get_gameui(0, 0));
	}
	
	uintptr_t war3_searcher::current_function(uintptr_t ptr)
	{
		static uint32_t nop = get_version() > version_121b ? 0xCCCCCCCC : 0x90909090;
        static uint32_t nop_func = get_version() > version_121b ? 0xEC8B55CC : 0xEC8B5590;
        static uint32_t nop_func1 = get_version() > version_121b ? 0xEC83CCCC : 0xEC839090;
        static uint32_t nop_func2 = get_version() > version_121b ? 0xEC83CCC3 : 0xEC8390C3;
        while (1) {
            uint32_t asm_code = *(uint32_t*)ptr;
            if (nop == asm_code)
                return ptr + 4;
            else if (0xEC8B55C3 == asm_code) // ret; push ebp; mov ebp, esp
                return ptr + 1;
            else if (nop_func1 == asm_code) // int 3; sub esp
                return ptr + 2;
            else if (nop_func2 == asm_code) // int 3; sub esp
                return ptr + 2;
            else if (nop_func == asm_code) // int 3; push ebp; mov ebp, esp
                return ptr + 1;
            ptr--;
        }
	}
	
	uint32_t war3_searcher::search_version() const
	{
		static const char warcraft3_version_string[] = "Warcraft III (build ";
	
		uintptr_t ptr;
		size_t size = sizeof(warcraft3_version_string) - 1;
		ptr = search_string_ptr(warcraft3_version_string, size);
		if (!ptr)
		{
			return 0;
		}
		uint32_t n = 0;
		ptr += size;
		while (isdigit(*(uint8_t*)ptr))
		{
			n = n * 10 + *(uint8_t*)ptr - '0';
			ptr++;
		}
		if (n == 52240) { n = version_127a; }
		return n;
	}

	uintptr_t war3_searcher::search_get_instance() const
	{
		uintptr_t get_instance;
		//=========================================
		//  (1)
		//
		//    push    493E0h
		//    push    1
		//    push    1
		//    push    0
		//    mov     edx, offset s_Config ; "config"
		//    mov     ecx, esi
		//    call    UnknowFunc  <----
		//=========================================
		get_instance = search_string("config");
		get_instance += sizeof uintptr_t;

		get_instance = next_opcode(get_instance, 0xE8, 5);
		get_instance = convert_function(get_instance);
		//=========================================
		//  (2)
		//
		//  UnknowFunc:
		//    push    esi
		//    mov     esi, edx
		//    call    jGetVMInstance <---
		//=========================================
		get_instance = next_opcode(get_instance, 0xE8, 5);
		get_instance = convert_function(get_instance);
		//=========================================
		//  (3)
		//
		//  jGetVMInstance:
		//    jmp    jGetVMInstance2 <----
		//=========================================
		get_instance = convert_function(get_instance);
		//=========================================
		//  (4)
		//
		//  jGetVMInstance2:
		//    push    esi
		//    mov     esi, ecx
		//    mov     ecx, 5
		//    call    GetInstance  <----
		//    push    esi
		//    mov     ecx, eax
		//    call    UnknowFunc
		//    pop     esi
		//    retn
		//=========================================
		get_instance = next_opcode(get_instance, 0xE8, 5);
		get_instance = convert_function(get_instance);
		return get_instance;
	}

	uintptr_t war3_searcher::search_get_gameui() const
	{
		uintptr_t get_gameui;

		//=========================================
		// 搜索 GetGameUI
		//=========================================
		//=========================================
		// (1)
		//
		// push "(BB)V"
		// mov edx, "EnableSelect"
		// mov ecx, [EnableSelect函数的地址] <----
		// call BindNative
		//=========================================
		get_gameui = search_string("EnableSelect");
		get_gameui = *(uintptr_t*)(get_gameui + 0x05);

		//=========================================
		// (2)
		//  EnableSelect:
		//    xor     edx, edx
		//    lea     ecx, [edx+1]
		//    call    GetGameUI  <----
		//    test    eax, eax
		//    jz      short to_retn
		//    mov     ecx, [esp+arg_4]
		//    mov     edx, [esp+arg_0]
		//    push    ecx
		//    push    edx
		//    call    sub_6F2F8710
		//  to_retn:
		//    retn
		//=========================================
		get_gameui = next_opcode(get_gameui, 0xE8, 5);
		get_gameui = convert_function(get_gameui);

		return get_gameui;
	}

	uintptr_t war3_searcher::search_gamestate_ptr() const {
		uintptr_t gamestate_ptr;

		//=========================================
		// (1)
		//
		// push		"(R)V"
		// mov		edx, "SetTimeOfDayScale"
		// mov		ecx, [SetTimeOfDayScale函数的地址] <----
		// call		BindNative
		//=========================================
		gamestate_ptr = search_string("SetTimeOfDayScale");
		gamestate_ptr = *(uintptr_t*)(gamestate_ptr + 0x05);

		//=========================================
		// (2)
		//  SetTimeOfDayScale:
		//    mov     ecx, [GameState指针的地址] <----
		//    call    UnknowFunc
		//=========================================
		gamestate_ptr = next_opcode(gamestate_ptr, 0x8B, 6);
		gamestate_ptr = *(uintptr_t*)(gamestate_ptr + 0x2);

		return gamestate_ptr;
	}

    void war3_searcher::search_create_object_by_type_id() {
        uintptr_t ptr = 0;

        // 搜索 运行玩家单位被攻击 的函数
        for (ptr = search_int_in_text(524818 /* 事件ID */); ptr; ptr = search_int_in_text(524818, ptr + 1)) {
            if (ReadMemory<uint8_t>(ptr - 1) == 0x68) { // push 0x80212
                ptr = current_function(ptr);
                break;
            }
        }

        //=========================================
        //  CPlayer_RunUnitAttackedEvent:
        //      ...
        //      call CPlayerUnitEventDataBase_GetTypeId
        //      ...
        //  if not inlined
        //      call CreateAgileTypeDataByTypeId
        //  else
        //      call reference_copy_ptr(&local_var, 0) // 实际动作只是把local_var设置0
        //      ...
        //      mov ebx, pAgileTypeData
        //      ...
        //      call CreateAgileTypeDataByTypeId_subfunc1
        //      ...
        //      call CreateAgileTypeDataByTypeId_subfunc2
        //  endif
        //      ...
        //      call pInitAgent
        //      ...
        //      call CreateAgentAbs
        //=========================================

        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        if (get_version() >= version_127a) {
            create_obj.pGetAgileTypeDataByTypeId = convert_function(ptr);
        }
        else {
            ptr += 5;
            ptr = next_opcode(ptr, 0x8B, 6);
            create_obj.inlined.pAgileTypeData = ReadMemory<uint32_t>(ptr + 2);
            ptr = next_opcode(ptr, 0xE8, 5);
            create_obj.inlined.pGetAgileTypeDataByTypeIdFunc1 = convert_function(ptr);
            ptr += 5;
            ptr = next_opcode(ptr, 0xE8, 5);
            create_obj.inlined.pGetAgileTypeDataByTypeIdFunc2 = convert_function(ptr);
        }
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        create_obj.pInitAgent = convert_function(ptr);
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
        create_obj.pCreateAgentAbs = convert_function(ptr);
    }

    void war3_searcher::search_create_handle() {
        uintptr_t ptr;

        //=========================================
        // (1)
        //
        // push		"()V"
        // mov		edx, "Player"
        // mov		ecx, [Player函数的地址] <----
        // call		BindNative
        //=========================================
        ptr = search_string("Player");
        ptr = *(uintptr_t*)(ptr + 0x05);

        //=========================================
        // (2)
        //  Player:
        //      ...
        //      call GetCPlayerById
        //      ...
        //      call GetDataNode
        //      ...
        //      call CreateOrGetHandleId
        //=========================================

        ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
        ptr += 5;
        ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
        create_handle.GetDataNode = convert_function(ptr);
        ptr += 5;
        ptr = warcraft3::next_opcode(ptr, 0xE8, 5);
        create_handle.CreateOrGetHandleId = convert_function(ptr);
    }

	war3_searcher& get_war3_searcher()
	{
		return base::singleton_nonthreadsafe<war3_searcher>::instance();
	}

	struct mapping_objectid
	{
		uint32_t unk00;
		uint32_t unk01;
		uint32_t unk02;
		uint32_t unk03;
		uint32_t unk04;
		uint32_t unk05;
		uint32_t unk06;
		uint32_t unk07;
		uint32_t unk08;
		uint32_t unk09;
		uint32_t unk0A;
		uint32_t unk0B;
		uint32_t unk0C;
		uint32_t unk0D;
		uint32_t unk0E;
		uint32_t unk0F;
	};

	namespace detail
	{
		mapping_objectid** search_mapping_objectid()
		{
			war3_searcher& s = get_war3_searcher();
			uintptr_t ptr = 0;
			if (s.get_version() > version_121b)
			{
				ptr = s.search_string_part("Engine\\Source\\Tempest/tempest_thread.h");
			}
			else
			{
				ptr = s.search_string("..\\Tempest/tempest_thread.h");
			}

			ptr += 0x04;
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr += 0x05;
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr = convert_function(ptr);
			ptr = next_opcode(ptr, 0x89, 6);
			ptr = *(uintptr_t*)(ptr + 0x02);
			return (mapping_objectid**)(ptr);
		}
	}

	uintptr_t find_objectid_64(const objectid_64& id)
	{
		static mapping_objectid** table_pptr = detail::search_mapping_objectid();

		mapping_objectid* table_ptr = *table_pptr;
		if (!table_ptr)
			return 0;

		if (id.a >> 31)
		{
			if ((id.a & 0x7FFFFFFF) < table_ptr->unk0F)
			{
				if (table_ptr->unk0B && *(uintptr_t*)(table_ptr->unk0B + 8 * id.a) == -2)
				{
					uintptr_t v4 = *(uintptr_t*)(table_ptr->unk0B + 8 * id.a + 4);
					if (v4 && (*(uintptr_t*)(v4 + 0x18) == id.b))
					{
						return v4;
					}
				}
			}
		}
		else
		{
			if (id.a < table_ptr->unk07)
			{
				if (table_ptr->unk03 && *(uintptr_t*)(table_ptr->unk03 + 8 * id.a) == -2)
				{
					uintptr_t v5 = *(uintptr_t*)(table_ptr->unk03 + 8 * id.a + 4);
					if (v5 && (*(uintptr_t*)(v5 + 0x18) == id.b))
					{
						return v5;
					}
				}
			}
		}
		return 0;
	}

	namespace detail
	{
		bool is_valid_ptr(uintptr_t ptr)
		{
			return (ptr >= get_war3_searcher().base() && ptr < (get_war3_searcher().base() + get_war3_searcher().size()));
		}
	}

	const char* get_class_name(uintptr_t ptr)
	{
		if (ptr && !::IsBadReadPtr(reinterpret_cast<const void*>(ptr), sizeof(uintptr_t)))
		{
			ptr = *(uintptr_t*)ptr - 0x04;
			if (detail::is_valid_ptr(ptr))
			{
				ptr = *(uintptr_t*)ptr + 0x0C;
				if (detail::is_valid_ptr(ptr))
				{
					ptr = *(uintptr_t*)ptr + 0x08;
					return (const char*)ptr;
				}
			}
		}
		return nullptr;
	}

    uintptr_t get_vfn_ptr(const char* name)
    {
        war3_searcher& s = get_war3_searcher();
        uintptr_t ptr = s.search_string_ptr(name, strlen(name));
        if (!ptr)
            return 0;
        ptr -= 0x08;
        for (uint32_t ptr1 = s.search_int_in_rdata(ptr); ptr1; ptr1 = s.search_int_in_rdata(ptr, ptr1 + 1)) {
            uint32_t ptr2 = ptr1 - 0x0C;
            ptr2 = s.search_int_in_rdata(ptr2);
            if (!ptr2)
                continue;
            ptr2 += 0x04;
            if (IsBadCodePtr((FARPROC)ptr2))
                continue;
            return ptr2;
        }
        return 0;
    }

	uint32_t get_object_type(uintptr_t ptr)
	{
		return base::this_call<uint32_t>(*(uintptr_t*)(*(uintptr_t*)ptr + 0x1C), ptr);
	}

	uintptr_t handle_to_object(uint32_t handle)
	{
		if (handle < 0x100000)
		{
			return 0;
		}

		handle_table_t** hts = get_jass_vm()->handle_table;
		if (!hts)
		{
			return 0;
		}

		uintptr_t object = (uintptr_t)(*hts)->table.at(3 * (handle - 0x100000) + 1);

		if (!object)
		{
			return 0;
		}

		uintptr_t ptr = find_objectid_64(*(objectid_64*)(object + 0x0C));
		if (!ptr || (*(uintptr_t*)(ptr + 0x0C) != '+agl') || *(uintptr_t*)(ptr + 0x20))
		{
			return 0;
		}

		return object;
	}

	uint32_t object_to_handle(uintptr_t obj)
	{
		handle_table_t** hts = get_jass_vm()->handle_table;
		if (!hts)
		{
			return 0;
		}

		hashtable::reverse_table& table = (*hts)->table;
		for (uint32_t i = 1; i < table.size*3; i += 3)
		{
			if (obj == (uintptr_t)table.at(i))
			{
				return 0x100000 + (i - 1) / 3;
			}
		}
		return 0;
	}

    bool type_check(uint32_t childTypeID, uint32_t parentTypeID) {
        if (childTypeID == parentTypeID)
            return true;
        static auto create_obj = get_war3_searcher().create_obj;
        uint32_t pAgileTypeData;
        while (true) {
            if (get_war3_searcher().get_version() >= version_127a)
                pAgileTypeData = base::this_call<uint32_t>(create_obj.pGetAgileTypeDataByTypeId, childTypeID);
            else
                pAgileTypeData = base::this_call<uint32_t>(create_obj.inlined.pGetAgileTypeDataByTypeIdFunc2, ReadMemory(create_obj.inlined.pAgileTypeData) + 0xC, base::this_call<uint32_t>(create_obj.inlined.pGetAgileTypeDataByTypeIdFunc1, &childTypeID), &childTypeID);
            if (!pAgileTypeData || ReadMemory(pAgileTypeData + 0x78) == childTypeID) // 无数据 / 父 == 子
                return false;
            childTypeID = ReadMemory(pAgileTypeData + 0x78); // 父类型ID
            if (childTypeID == parentTypeID)
                return true;
        }
    }

#pragma warning (push)
#pragma warning (disable:26429) // 你要让我用gsl::not_null<T> 然后还要自己下载gsl库 ?
#pragma warning (disable:26481)

    uint32_t** reference_copy_ptr(uint32_t** _this, uint32_t* a2) {
        // 是否相同
        if (_this[0] != a2) {
            // 已有数值
            if (_this[0])
                // 计数 - 1
                if (_this[0][1]-- == 1)
                    // 计数 0 则调用解构函数
                    base::this_call<void>(ReadMemory<uint32_t>(_this[0][0]), _this[0]);
            // 复制来源是否不空
            if (a2)
                // 计数 + 1
                a2[1]++;
            // 复制指针地址
            _this[0] = a2;
        }
        return _this;
    }

    uint32_t** reference_copy_ptr_typesafe(uint32_t** _this, uint32_t* a2, uint32_t typeID) {
        // 是否相同
        if (_this[0] != a2) {
            // 已有数值
            if (_this[0])
                // 计数 - 1
                if (_this[0][1]-- == 1)
                    // 计数 0 则调用解构函数
                    base::this_call<void>(ReadMemory<uint32_t>(_this[0][0]), _this[0]);
            // 复制来源是否不空
            if (a2)
                // 计数 + 1
                a2[1]++;
            // 检查类型 左边为要检查的类型 右边为兼容的类型
            // 用 JASS 简单说明就是: type 左 extends 右
            if (type_check(get_object_type((uint32_t)a2), typeID))
                // 复制指针地址
                _this[0] = a2;
            else
                // 清空
                _this[0] = 0;
        }
        return _this;
    }

    void reference_free_ptr(uint32_t** _this, uint32_t offset) {
        // 已有数值
        if (_this[0]) {
            // 计数 - 1
            --_this[0][1];
            if (!_this[0][1])
                // 计数 0 则调用解构函数
                base::this_call<void>(ReadMemory<uint32_t>(_this[0][offset]), _this[0]);
            // 清空
            _this[0] = 0;
        }
    }
#pragma warning(pop)

    uint32_t create_by_typeid(uint32_t typeID) {
        static auto create_obj = get_war3_searcher().create_obj;
        uint32_t pAgileTypeData;
        if (get_war3_searcher().get_version() >= version_127a)
            pAgileTypeData = base::this_call<uint32_t>(create_obj.pGetAgileTypeDataByTypeId, typeID);
        else
            pAgileTypeData = base::this_call<uint32_t>(create_obj.inlined.pGetAgileTypeDataByTypeIdFunc2, ReadMemory(create_obj.inlined.pAgileTypeData) + 0xC, base::this_call<uint32_t>(create_obj.inlined.pGetAgileTypeDataByTypeIdFunc1, &typeID), &typeID);
        uint32_t agent[11];
        base::fast_call<uint32_t>(create_obj.pInitAgent, agent, typeID, ReadMemory(pAgileTypeData + 0x70));
        agent[9] = 0xFFFFFFFF;
        uint32_t pCAgentAbs = base::fast_call<uint32_t>(create_obj.pCreateAgentAbs, agent, 1, 1);
        uint32_t pObj = 0;
        reference_copy_ptr((uint32_t**)&pObj, ReadMemory<uint32_t*>(pCAgentAbs + 0x54));
        return pObj;
    }

    // 差不多跟 object_to_handle 一样
    // 不同的就是 如果目标不存在于表内 会加进去然后返回handle id
    uint32_t create_handle(uint32_t pObject) {
        static auto& searcher = get_war3_searcher();
        return base::this_call<uint32_t>(
            searcher.create_handle.CreateOrGetHandleId,
            base::this_call<uint32_t>(
                searcher.create_handle.GetDataNode,
                searcher.get_gamewar3()
            ),
            pObject,
            0
        );
    }

    uint32_t GetObjectByHash(uint32_t a, uint32_t b) {
        uint32_t obj = find_objectid_64(objectid_64(a, b));
        if (obj && !ReadMemory<uint32_t>(obj + 0x20))
            return ReadMemory<uint32_t>(obj + 0x54);
        else
            return 0;
    }

    uint32_t GetObjectByHash_agl(uint32_t a, uint32_t b) {
        uint32_t obj = find_objectid_64(objectid_64(a, b));
        if (ReadMemory(obj + 0xC) != '+agl') {
            obj = ReadMemory(obj + 0x30);
            if (obj && !ReadMemory<uint32_t>(obj + 0x20))
                return ReadMemory<uint32_t>(obj + 0x54);
        }
        return 0;
    }
}
