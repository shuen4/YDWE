#pragma once

#include <warcraft3/config.h>
#include <warcraft3/basic_searcher.h>

namespace warcraft3 {

	class _WAR3_API war3_searcher : public basic_searcher
	{
		typedef basic_searcher _Mybase;
	public:
		war3_searcher();
		war3_searcher(HMODULE hGameDll);

		uint32_t  get_version() const;
		uint32_t  get_instance(uint32_t index);
		uint32_t  get_gameui(uint32_t unk0, uint32_t unk1);
		uint32_t  get_gamewar3();
		bool      is_gaming();
		uintptr_t current_function(uintptr_t ptr);

    public:
        struct CreateObjectByTypeID {
            union {
                uint32_t pGetAgileTypeDataByTypeId;
                struct {
                    uint32_t pAgileTypeData;
                    uint32_t pGetAgileTypeDataByTypeIdFunc1;
                    uint32_t pGetAgileTypeDataByTypeIdFunc2;
                } inlined;
            };
            uint32_t pInitAgent;
            uint32_t pCreateAgentAbs;
        } create_obj;
        struct CreateHandle {
            uint32_t GetDataNode;
            uint32_t CreateOrGetHandleId;
        } create_handle;

	private:
		uint32_t  search_version() const;
		uintptr_t search_get_instance() const;
		uintptr_t search_get_gameui() const;
		uintptr_t search_gamestate_ptr() const;
        void search_create_object_by_type_id();
        void search_create_handle();

	private:
		uint32_t  version_;
		uintptr_t get_instance_;
		uintptr_t get_gameui_;
		uintptr_t gamewar3_ptr_;
	};

	struct objectid_64
	{
		uint32_t a;
		uint32_t b;

		inline objectid_64(uint32_t a_, uint32_t b_)
			: a(a_)
			, b(b_)
		{ }

		inline bool is_valid() const
		{
			return (a & b) != (uint32_t)(-1);
		}
	};

	_WAR3_API war3_searcher& get_war3_searcher();
	_WAR3_API uintptr_t   find_objectid_64(const objectid_64& id);
	_WAR3_API const char* get_class_name(uintptr_t ptr);
	_WAR3_API uintptr_t   get_vfn_ptr(const char* name);
	_WAR3_API uint32_t    get_object_type(uintptr_t ptr);
	_WAR3_API uintptr_t   handle_to_object(uint32_t handle);
	_WAR3_API uint32_t    object_to_handle(uintptr_t obj);
    _WAR3_API bool        type_check(uint32_t childTypeID, uint32_t parentTypeID);
    _WAR3_API uint32_t**  reference_copy_ptr(uint32_t** _this, uint32_t* a2);
    _WAR3_API uint32_t**  reference_copy_ptr_typesafe(uint32_t** _this, uint32_t* a2, uint32_t typeID);
    _WAR3_API void        reference_free_ptr(uint32_t** _this, uint32_t offset = 0);
    _WAR3_API uint32_t    create_by_typeid(uint32_t typeID);
    _WAR3_API uint32_t    create_handle(uint32_t pObject);
    _WAR3_API uint32_t    GetObjectByHash(uint32_t a, uint32_t b);
}
