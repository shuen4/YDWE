#include "handle_scanner.h"
#include "hashtable.h"
#include <base/warcraft3/war3_searcher.h>
#include <base/warcraft3/version.h>
#include <base/util/format.h>

#pragma execution_character_set("utf-8")

namespace handles {
	void table::add_handle(uint32_t handle, uint32_t object, uint32_t reference) {
		if (object && base::warcraft3::get_object_type(object) == 'ghth') {
			uint32_t id = *(uint32_t*)(object + 0x24);
			htmgr[id] = handle;
		}

		node hi;
		hi.handle = handle;
		hi.object = object;
		hi.reference = reference;
		insert(std::make_pair(handle, hi));
	}

	void table::add_global_reference(base::warcraft3::hashtable::variable_node& var) {
		base::warcraft3::jass::global_variable gv(&var);
		if (base::warcraft3::jass::OPCODE_VARIABLE_HANDLE == gv.type()) {
			add_reference(table::e_type::global, (uint32_t)gv, gv.name());
		}
		else if (base::warcraft3::jass::OPCODE_VARIABLE_HANDLE_ARRAY == gv.type()) {
			for (uint32_t i = 0; i < gv.array_size(); ++i) {
				add_reference(table::e_type::global, gv[i], base::format("%s[%d]", gv.name(), i));
			}
		}
	}

	void table::add_local_reference(base::warcraft3::hashtable::variable_node& var, const std::string& funcname) {
		base::warcraft3::jass::global_variable gv(&var);
		if (base::warcraft3::jass::OPCODE_VARIABLE_HANDLE == gv.type()) {
			add_reference(table::e_type::local, (uint32_t)gv, base::format("%s!%s", funcname, gv.name()));
		}
		else if (base::warcraft3::jass::OPCODE_VARIABLE_HANDLE_ARRAY == gv.type()) {
			for (uint32_t i = 0; i < gv.array_size(); ++i) {
				add_reference(table::e_type::local, gv[i], base::format("%s!%s[%d]", funcname, gv.name(), i));
			}
		}
	}

	void table::add_hashtable_reference(uint32_t ht, uint32_t t, uint32_t k, uint32_t handle) {
		auto it = htmgr.find(ht);
		if (it == htmgr.end()) {
			add_reference(table::e_type::hashtable, handle, base::format("handle: unknown [%d][%d]", t, k));
			return;
		}
		add_reference(table::e_type::hashtable, handle, base::format("handle: 0x%08X [%d][%d]", it->second, t, k));
	}

	void table::add_reference(e_type type, uint32_t handle, const std::string& name) {
		auto it = find(handle);
		if (it == end()) {
			return;
		}
		switch (type) {
		case e_type::local:
			it->second.local_reference.push_back(name);
			break;
		case e_type::global:
			it->second.global_reference.push_back(name);
			break;
		case e_type::hashtable:
			it->second.hashtable_reference.push_back(name);
			break;
		}
	}

	std::map<uint32_t, std::string_view> ot2ht = {
		{ '+dlb', "button" },
		{ '+dlg', "dialog" },
		{ '+w3d', "destructable" },
		{ '+EIP', "effect" },
		{ '+EIm', "effect" },
		{ '+rev', "event" },
		{ 'alvt', "event" },
		{ 'bevt', "event" },
		{ 'devt', "event" },
		{ 'gevt', "event" },
		{ 'gfvt', "event" },
		{ 'pcvt', "event" },
		{ 'pevt', "event" },
		{ 'psvt', "event" },
		{ 'tmet', "event" },
		{ 'tmvt', "event" },
		{ 'uevt', "event" },
		{ '+flt', "filter" },
		{ '+fgm', "fogmodifier" },
		{ '+frc', "force" },
		{ '+grp', "group" },
		{ 'ghth', "hashtable" },
		{ 'item', "item" },
		{ '+loc', "location" },
		{ '+mdb', "multiboard" },
		{ '+ply', "player" },
		{ '+rct', "rect" },
		{ '+agr', "region" },
		{ '+snd', "sound" },
		{ '+tmr', "timer" },
		{ '+tid', "timerdialog" },
		{ '+trg', "trigger" },
		{ '+tac', "triggeraction" },
		{ 'tcnd', "triggercondition" },
		{ '+w3u', "unit" },
	};
	const char* ObjectTypeToHandleType(uint32_t type) {
		auto it = ot2ht.find(type);
		if (it == ot2ht.end()) {
			return 0;
		}
		return it->second.data();
	}

	void scanner_display(std::fstream& fs, node& h) {
		using namespace base::warcraft3;

		uint32_t ref = (h.object)? 1 : 0;
		ref += h.global_reference.size();
		ref += h.local_reference.size();
		ref += h.hashtable_reference.size();

		fs << base::format("handle: 0x%08X", h.handle) << std::endl;
		fs << base::format("  ����: %d/%d", ref, h.reference) << std::endl;
		if (h.object) {
			uint32_t type = get_object_type(h.object);
			const char* handletype = ObjectTypeToHandleType(type);
			if (handletype) {
				fs << base::format("  ����: %s", handletype) << std::endl;
			}
			else {
				fs << "  ����: δ֪" << std::endl;
			}
			fs << base::format("  ����: %c%c%c%c", ((const char*)&type)[3], ((const char*)&type)[2], ((const char*)&type)[1], ((const char*)&type)[0]) << std::endl;
		}
		auto pos = ht::getHandlePos(h.handle);
		if (!pos.empty()) {
			jass::opcode *current_op = pos[0];
			assert(current_op->op == jass::OPTYPE_CALLNATIVE);
			fs << base::format("  ����λ��: %s", jass::from_stringid(current_op->arg)) << std::endl;
			for (auto& cur : pos) {
				jass::opcode* op;
				for (op = cur; op->op != jass::OPTYPE_FUNCTION; --op)
				{
				}
				fs << base::format("    | %s+%d", jass::from_stringid(op->arg), cur - op) << std::endl;
			}
		}
		if (!h.global_reference.empty() || !h.local_reference.empty()) {
			fs << "  �������ı���:" << std::endl;
			for (auto gv = h.global_reference.begin(); gv != h.global_reference.end(); ++gv) {
				fs << base::format("    | %s", gv->c_str()) << std::endl;
			}
			for (auto gv = h.local_reference.begin(); gv != h.local_reference.end(); ++gv) {
				fs << base::format("    | %s", gv->c_str()) << std::endl;
			}
		}
		if (!h.hashtable_reference.empty()) {
			fs << "  �������Ĺ�ϣ��:" << std::endl;
			for (auto gv = h.hashtable_reference.begin(); gv != h.hashtable_reference.end(); ++gv) {
				fs << base::format("    | %s", gv->c_str()) << std::endl;
			}
		}
	}

	void scanner(std::fstream& fs, bool all) {
		using namespace base::warcraft3;

		table ht;
		handle_table_t** hts = get_jass_vm()->handle_table;
		if (hts) {
			hashtable::reverse_table& table = (*hts)->table;
			for (auto it = table.begin(); it != table.end(); ++it) {
				ht.add_handle(it->value, (uint32_t)table.at(3 * (it->value - 0x100000) + 1), (uint32_t)table.at(3 * (it->value - 0x100000)));
			}
		}

		for (int i = 1;; ++i) {
			jass_vm_t* vm = get_jass_vm(i);
			if (!vm) {
				break;
			}
			if (!vm->opcode) {
				continue;
			}

			jass::opcode* op = vm->opcode - 1;
			stackframe_t* frame = vm->stackframe;
			while ((intptr_t)frame > 0) {
				struct jass::opcode *func;
				for (func = op; func->op != jass::OPTYPE_FUNCTION; --func)
				{
				}
				std::string funcname = jass::from_stringid(func->arg);

				hashtable::variable_table* vt = &(frame->local_table);
				if (vt) {
					for (auto it = vt->begin(); it != vt->end(); ++it) {
						ht.add_local_reference(*it, funcname);
					}
				}
				frame = frame->next;
				if ((intptr_t)frame <= 0) {
					break;
				}
				uintptr_t code = frame->codes[frame->index]->code;
				op = (jass::opcode*)(vm->symbol_table->unk0 + code * 4);
				if (!op) {
					break;
				}
			}
		}

		hashtable::variable_table* vt = get_jass_vm()->global_table;
		for (auto it = vt->begin(); it != vt->end(); ++it) {
			ht.add_global_reference(*it);
		}

		if (base::warcraft3::get_war3_searcher().get_version() >= base::warcraft3::version_124b) {
			ht::hashtableEachHandle([&](uint32_t hashtable, uint32_t t, uint32_t k, uint32_t v) {
				ht.add_hashtable_reference(hashtable, t, k, v);
			});
		}

		if (all) {
			fs << "---------------------------------------" << std::endl;
			fs << "              й©��ⱨ��             " << std::endl;
			fs << "---------------------------------------" << std::endl;
			fs << "���: " << (hts ? (*hts)->table.size : 0) << std::endl;
			fs << "����: " << ht.size() << std::endl;
			fs << "---------------------------------------" << std::endl;
			for (auto& it : ht) { 
				node& h = it.second;
				scanner_display(fs, h);
			}
		}
		else {
			std::vector<node> leaks;
			for (auto it = ht.begin(); it != ht.end(); ++it) {
				node& h = it->second;
				uint32_t type = 0;
				if (h.object) {
					type = get_object_type(h.object);
					if (type == '+ply') {
						continue;
					}
				}
				uint32_t ref = 0;
				ref += h.global_reference.size();
				ref += h.local_reference.size();
				ref += h.hashtable_reference.size();
				if (h.object && ref == 0) {
					switch (type) {
					case '+loc': // location
					case '+grp': // group
					case '+agr': // region
					case '+rct': // rect
					case '+frc': // force
					case '+EIP': // effect
					case '+EIm':
						leaks.push_back(h);
						break;
					default:
						break;
					}
				}
				else if (ref + (h.object ? 1 : 0) < h.reference) {
					leaks.push_back(h);
				}
			}

			fs << "---------------------------------------" << std::endl;
			fs << "              й©��ⱨ��             " << std::endl;
			fs << "---------------------------------------" << std::endl;
			fs << "���: " << (hts ? (*hts)->table.size : 0) << std::endl;
			fs << "����: " << ht.size() << std::endl;
			fs << "й©: " << leaks.size() << std::endl;
			fs << "---------------------------------------" << std::endl;
			for (node& h : leaks) {
				scanner_display(fs, h);
			}
		}
	}
}