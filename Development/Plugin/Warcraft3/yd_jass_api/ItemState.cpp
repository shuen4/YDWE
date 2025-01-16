#include <base/hook/fp_call.h>
#include <base/hook/iat.h>
#include <warcraft3/jass/hook.h>
#include <warcraft3/jass.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>	
#include <warcraft3/hashtable.h>
#include <array>
#include <string>
#include <base/util/memory.h>
#include <warcraft3/event.h>
#include "StringPool.h"

namespace warcraft3::japi {
	extern string_pool_t string_pool;

	enum ITEM_DATA_TYPE
	{
		ITEM_DATA_ART = 1,
		ITEM_DATA_TIP,
		ITEM_DATA_UBERTIP,
		ITEM_DATA_NAME,
		ITEM_DATA_DESCRIPTION,
	};

	struct item_ui_node : public hashtable::virtual_func_table, public hashtable::node
	{
		uint32_t unk0;
		uint32_t unk1;
		uint32_t unk2;
		uint32_t unk3;
		char**   name;	   // 540
		char*    description;
		uint32_t unk4[134];
		char**   art;	   // 760 
		uint32_t unk5[4];
		char**   tip;
		uint32_t unk6;
		uint32_t unk7;
		char**   ubertip;  // 780
	};
	typedef hashtable::table<item_ui_node> item_ui_table_t;

	static uintptr_t search_item_table()
	{
		war3_searcher& s = get_war3_searcher();
		uintptr_t ptr = s.search_string("GetItemName");
		ptr = *(uintptr_t*)(ptr + 0x05);
		if (s.get_version() > version_121b)
		{
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr += 5;
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr = convert_function(ptr);
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr = convert_function(ptr);
			ptr = next_opcode(ptr, 0xB9, 5);
			return *(uintptr_t*)(ptr + 1);
		}
		else
		{
			ptr = next_opcode(ptr, 0xC3, 1);
			ptr += 1;
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr = convert_function(ptr);
			ptr = next_opcode(ptr, 0xE8, 5);
			ptr = convert_function(ptr);
			ptr = next_opcode(ptr, 0x8B, 6);
			return *(uintptr_t*)(ptr + 2) - 0x24;
		}
	}

	static uintptr_t get_item_table()
	{
		static uintptr_t table = search_item_table();
		return table;
	}

	static uint32_t hashid(uint32_t id)
	{
		static uint32_t box[] = {
			0x098AA3D0C, 0x0F67BCA9E, 0x0C46CA84C, 0x02AC9D845,
			0x09A1CF1DD, 0x06450148E, 0x08516213D, 0x0C0882BBF,
			0x0F10C2A9C, 0x09D7CF013, 0x0CD845F5E, 0x01D4BD837,
			0x01055F69A, 0x0A6A87DCD, 0x0312D8D9E, 0x0645A1CEC,
		};
		uint32_t hashlo = 0x7FED7FED;
		uint32_t hashhi = 0xEEEEEEEE;
		for (; id > 0;)
		{
			hashlo = (box[(id >> 4) & 0x0F] - box[id & 0x0F]) ^ (hashhi + hashlo);
			hashhi = hashhi + (id & 0xFF) + 32 * hashhi + hashlo + 3;
			id >>= 8;
		}
		return hashlo;
	}

	uint32_t  __cdecl EXGetItemDataString(uint32_t code, uint32_t type)
	{
		item_ui_table_t* table = (item_ui_table_t*)get_item_table();
		if (!table) {
			return jass::create_string("");
		}
		item_ui_node* ptr = table->find(hashid(code));
		if (!ptr) {
			return jass::create_string("");
		}
		char** buf = 0;
		switch (type) {
		case ITEM_DATA_ART:
			buf = ptr->art;
			break;
		case ITEM_DATA_UBERTIP:
			buf = ptr->ubertip;
			break;
		case ITEM_DATA_NAME:
			buf = ptr->name;
			break;
		case ITEM_DATA_TIP:
			buf = ptr->tip;
			break;
		case ITEM_DATA_DESCRIPTION:
			buf = &(ptr->description);
			break;
		}
		if (!buf || !*buf)
		{
			return jass::create_string("");
		}
		return jass::create_string(*buf);
	}

	bool  __cdecl EXSetItemDataString(uint32_t code, uint32_t type, uint32_t value)
	{
		item_ui_table_t* table = (item_ui_table_t*)get_item_table();
		if (!table) {
			return false;
		}
		item_ui_node* ptr = table->find(hashid(code));
		if (!ptr) {
			return false;
		}
		char** buf = 0;
		switch (type) {
		case ITEM_DATA_ART:
			buf = ptr->art;
			break;
		case ITEM_DATA_UBERTIP:
			buf = ptr->ubertip;
			break;
		case ITEM_DATA_NAME:
			buf = ptr->name;
			break;
		case ITEM_DATA_TIP:
			buf = ptr->tip;
			break;
		case ITEM_DATA_DESCRIPTION:
			buf = &(ptr->description);
			break;
		}

		if (!buf || !*buf)
		{
			return false;
		}
		string_pool.free((uintptr_t)*buf);
		const char* value_str = jass::from_string(value);
		if (value_str)
		{
			size_t      value_len = strlen(value_str);
			uintptr_t   value_buf = string_pool.malloc(value_len + 1);
			*buf = (char*)value_buf;
			if (value_buf)
			{
				strncpy_s(*buf, value_len + 1, value_str, value_len);
			}
		}
		else
		{
			*buf = 0;
		}
		return true;
	}

    uintptr_t search_item_data_table() {
        war3_searcher& s = get_war3_searcher();
        uintptr_t ptr = s.search_string("IsItemIdSellable");
        ptr = *(uintptr_t*)(ptr + 0x05);
        ptr = next_opcode(ptr, 0xE9, 5);
        ptr = convert_function(ptr);
        if (s.get_version() != version_127a) {
            ptr = next_opcode(ptr, 0xB9, 5);
            return ReadMemory(ptr + 1);
        }
        else {
            // 被 inline 了
            // Game.dll + 0xBEC238
            ptr = next_opcode(ptr, 0xA1, 5);
            return ReadMemory(ptr + 1) - 0x24;
        }
    }
    uintptr_t get_item_data_table() {
        static uintptr_t table = search_item_data_table();
        return table;
    }
    enum class ITEM_DATA_TYPE_INTEGER
    {
        ITEM_DATA_LEVEL = 1,
        ITEM_DATA_TYPE,
        ITEM_DATA_POWERUP,
        ITEM_DATA_SELLABLE,
        ITEM_DATA_PAWNABLE,
        ITEM_DATA_COLOR,
    };
    struct item_data_node : public hashtable::node
    {
        uint32_t unk1[8];
        uint32_t level;
        uint32_t type;
        uint32_t unk2[3];
        uint32_t powerup;
        uint32_t sellable;
        uint32_t pawnable;
        uint32_t unk3[15];
        uint32_t color;
    };

    jass::jinteger_t __cdecl EXGetItemDataInteger(jass::jinteger_t typeID, jass::jinteger_t dataType) {
        hashtable::table<item_data_node>* table = (hashtable::table<item_data_node>*)get_item_data_table();
        if (!table)
            return false;

        item_data_node* ptr = table->find(hashid(typeID));
        if (!ptr)
            return false;

        switch ((ITEM_DATA_TYPE_INTEGER)dataType) {
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_LEVEL:
            return ptr->level;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_TYPE:
            return ptr->level;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_POWERUP:
            return ptr->powerup;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_SELLABLE:
            return ptr->sellable;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_PAWNABLE:
            return ptr->pawnable;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_COLOR:
            return ptr->color;
        default:
            return 0;
        }
    }
    jass::jboolean_t __cdecl EXSetItemDataInteger(jass::jinteger_t typeID, jass::jinteger_t dataType, jass::jinteger_t value) {
        hashtable::table<item_data_node>* table = (hashtable::table<item_data_node>*)get_item_data_table();
        if (!table) 
            return false;

        item_data_node* ptr = table->find(hashid(typeID));
        if (!ptr)
            return false;

        switch ((ITEM_DATA_TYPE_INTEGER)dataType) {
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_LEVEL:
            ptr->level = value;
            return true;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_TYPE:
            if (value < 0 || value > 7)
                return false;
            ptr->level = value;
            return true;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_POWERUP:
            ptr->powerup = (bool)value;
            return true;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_SELLABLE:
            ptr->sellable = (bool)value;
            return true;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_PAWNABLE:
            ptr->pawnable = (bool)value;
            return true;
        case ITEM_DATA_TYPE_INTEGER::ITEM_DATA_COLOR:
            ptr->color = value;
            return true;
        default:
            return false;
        }
    }

    std::map<uint32_t, uint32_t> item_color;
    uint32_t real_GetItemColorById;
    uint32_t* __fastcall fake_GetItemColorById(uint32_t* out, uint32_t pItemID /* 原本是物品类型, 换成 lea 了所以现在是指针 */) {
        uint32_t pItem = pItemID - 0x30;
        auto i = item_color.find(pItem);
        if (i == item_color.end())
            return base::fast_call<uint32_t*>(real_GetItemColorById, out, ReadMemory(pItemID));
        *out = i->second;
        return out;
    }
    void patchCItem_UpdateColor() {
        uint32_t ptr = ReadMemory(get_vfn_ptr(".?AVCItem@@") + 0x104);
        ptr = next_opcode(ptr, 0x8B, 3);
        WriteMemoryEx<uint8_t>(ptr, 0x8D); // mov -> lea
        ptr = next_opcode(ptr, 0xE8, 5);
        real_GetItemColorById = convert_function(ptr);
        PatchCallRelative(ptr, fake_GetItemColorById);
    }

    uint32_t __cdecl EXGetItemColor(uint32_t item) {
        uint32_t pItem = handle_to_object(item);
        if (!pItem || !type_check(get_object_type(pItem), 'item'))
            return 0xFFFFFFFF;
        return *fake_GetItemColorById(&pItem/* 复用变量 */, pItem + 0x30);
    }
    bool __cdecl EXSetItemColor(uint32_t item, uint32_t color) {
        uint32_t pItem = handle_to_object(item);
        if (!pItem || !type_check(get_object_type(pItem), 'item'))
            return false;
        item_color[pItem] = color;
        return true;
    }
    bool __cdecl EXResetItemColor(uint32_t item) {
        uint32_t pItem = handle_to_object(item);
        if (!pItem || !type_check(get_object_type(pItem), 'item'))
            return false;
        auto i = item_color.find(pItem);
        if (i == item_color.end())
            return false;
        item_color.erase(i);
        return true;
    }

	void InitializeItemState() {
		jass::japi_add((uintptr_t)EXGetItemDataString,  "EXGetItemDataString",      "(II)S");
		jass::japi_add((uintptr_t)EXSetItemDataString,  "EXSetItemDataString",      "(IIS)B");
        jass::japi_add((uintptr_t)EXGetItemDataInteger, "EXGetItemDataInteger",     "(II)I");
        jass::japi_add((uintptr_t)EXSetItemDataInteger, "EXSetItemDataInteger",     "(III)B");
        jass::japi_add((uintptr_t)EXGetItemColor,       "EXGetItemColor",           "(Hitem;)I");
        jass::japi_add((uintptr_t)EXSetItemColor,       "EXSetItemColor",           "(Hitem;I)B");
        jass::japi_add((uintptr_t)EXResetItemColor,     "EXResetItemColor",         "(Hitem;)B");
        patchCItem_UpdateColor();
        event_agent_destructor([](uint32_t _this) {
            auto i = item_color.find(_this);
            if (i != item_color.end()) {
                item_color.erase(i);
            }
        });
	}
}
