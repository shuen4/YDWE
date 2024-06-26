﻿#include "MainWindow.h"
#include "Regedit.h"
#include "Shortcuts.h"
#include <base/file/stream.h>
#include <base/path/self.h>
#include <base/path/ydwe.h>
#include <bee/utility/path_helper.h>
#include <bee/utility/unicode_win.h>
#include <bee/platform/version.h>
#include <bee/utility/module_version_win.h>
#include <base/win/font/utility.h>
#include <base/util/ini.h>
#include <base/i18n/gettext.h>

#define YDWE_WAR3_INLINE
#include <warcraft3/directory.h>
#include <warcraft3/directory.cpp>

std::wstring CComboUI_GetSelectText(DuiLib::CComboUI* pui)
{
	int i = pui->GetCurSel();
	if (i < 0 || i >= pui->GetCount())
	{
		return std::wstring();
	}
	DuiLib::CListLabelElementUI* elem = dynamic_cast<DuiLib::CListLabelElementUI*>(pui->GetItemAt(i));
	if (!elem)
	{
		return std::wstring();
	}
	return elem->GetText();
}

namespace 
{
	class Attribute
	{
	public:
		enum e_type_t {
			e_CheckBox,
			e_RadioButton,
			e_ComboBox,
		};

		Attribute(uint8_t count)
			: m_type(e_RadioButton)
			, m_section("Option")
			, m_count(count)
		{ }

		Attribute(std::string const& section)
			: m_type(e_CheckBox)
			, m_section(section)
			, m_count(0)
		{ }

		Attribute(std::string const& section, e_type_t type)
			: m_type(type)
			, m_section(section)
			, m_count(0)
		{ }

		e_type_t           Type() const { return m_type; }
		std::string const& Section() const { return m_section; }
		uint8_t            Count() const { return m_count; }

	private:
		e_type_t    m_type;
		std::string m_section;
		uint8_t     m_count;
	};

	std::map<std::string, Attribute> configAttribute = {
		{ "MapSave",                                 Attribute(4) },
		{ "War3Patch",                               Attribute(3) },
		{ "ScriptInjection",                         Attribute(2) },
		{ "HostTest",                                Attribute(2) },
		{ "EnableJassHelper",                        Attribute("ScriptCompiler") },
		{ "EnableJassHelperDebug",                   Attribute("ScriptCompiler") },
		{ "EnableJassHelperOptimization",            Attribute("ScriptCompiler") },
		{ "PJass",                                   Attribute(2) },
		{ "LaunchRenderingEngine",                   Attribute("MapTest", Attribute::e_ComboBox) },
		{ "LaunchWindowed",                          Attribute("MapTest") },
		{ "LaunchFullWindowed",                      Attribute("MapTest") },
		{ "LaunchLockingMouse",                      Attribute("MapTest") },
		{ "LaunchFixedRatioWindowed",                Attribute("MapTest") },
		{ "LaunchWideScreenSupport",                 Attribute("MapTest") },
		{ "LaunchDisableSecurityAccess",             Attribute("MapTest") },
		{ "EnableHost",                              Attribute("MapTest") },
		{ "EnableMapSlk",                            Attribute("MapTest") },
		{ "EnableDotNetSupport",                     Attribute("ThirdPartyPlugin") },
		{ "EnableTesh",                              Attribute("ThirdPartyPlugin") },
		{ "EnableManualNewId",                       Attribute("FeatureToggle") },
		{ "FontEnable",                              Attribute("Font") },
		{ "FontName",                                Attribute("Font", Attribute::e_ComboBox) },
		{ "FontSize",                                Attribute("Font", Attribute::e_ComboBox) },
	};
}

CMainWindow::CMainWindow() 
{
	try
	{
		m_ydwe_path = base::path::ydwe(false) / L"YDWE.exe";
	}
	catch (...)
	{
	}
}

LPCTSTR CMainWindow::GetWindowClassName() const 
{ 
	return L"YDWEConfig"; 
}

void CMainWindow::OnFinalMessage(HWND /*hWnd*/) 
{ 
	delete this; 
}

fs::path CMainWindow::GetSkinZip() const 
{
	return std::move(base::path::ydwe(true) / L"share" / L"skins"); 
}

void CMainWindow::ContrlSelected(std::string const& name, bool bSelect)
{
	DuiLib::CCheckBoxUI* ctrl = m_controls[name];
	if (ctrl) ctrl->Selected(bSelect);
}

void CMainWindow::ContrlSetEnabled(std::string const& name, bool bEnable)
{
	DuiLib::CCheckBoxUI* ctrl = m_controls[name];
	if (ctrl) ctrl->SetEnabled(bEnable);
}

void CMainWindow::InitWindow()
{
	foreach(auto it, configAttribute)
	{
		std::string const& name = it.first;
		Attribute const& attribute = it.second;
		if (attribute.Type() == Attribute::e_RadioButton)
		{
			for (uint8_t i = 0; i < attribute.Count(); ++i)
			{
				std::string radioname = name + "_" + std::to_string((long long)i);
				m_controls[radioname] = dynamic_cast<DuiLib::CRadioButtonUI*>(m_pm.FindControl(bee::u2w(radioname).c_str()));
			}
		}
		else if (attribute.Type() == Attribute::e_CheckBox)
		{
			m_controls[name] = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(bee::u2w(name).c_str()));
		}
		else if (attribute.Type() == Attribute::e_ComboBox)
		{
			m_comboboxs[name] = dynamic_cast<DuiLib::CComboUI*>(m_pm.FindControl(bee::u2w(name).c_str()));
		}
	}

	m_pEnableJassHelper = m_controls["EnableJassHelper"];
	m_pLaunchWindowed   = m_controls["LaunchWindowed"];
	m_pLaunchWideScreenSupport = m_controls["LaunchWideScreenSupport"];
	m_pLaunchFixedRatioWindowed = m_controls["LaunchFixedRatioWindowed"];
	m_pEnableHost       = m_controls["EnableHost"];
	m_pFontEnable       = m_controls["FontEnable"];
	m_pFontName         = m_comboboxs["FontName"];
	m_pFontSize         = m_comboboxs["FontSize"];

	m_pFileAssociation_w3x = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(L"FileAssociation_w3x"));
	m_pFileAssociation_w3m = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(L"FileAssociation_w3m"));
	m_pShortcuts_desktop   = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(L"Shortcuts_desktop"));
	m_pShortcuts_taskbar   = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(L"Shortcuts_taskbar"));	
	m_pAllowLocalFiles     = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(L"AllowLocalFiles"));
	m_pWar3PatchList       = dynamic_cast<DuiLib::CVerticalLayoutUI*>(m_pm.FindControl(L"War3PatchList"));
	m_pWar3PluginList      = dynamic_cast<DuiLib::CVerticalLayoutUI*>(m_pm.FindControl(L"War3PluginList"));
	m_pWarcraft3Directory  = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(L"Warcraft3Directory"));
	m_pWarcraft3Directory1 = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(L"Warcraft3Directory1"));
	m_pFontPreview         = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(L"FontPreview"));

	m_pm.AddNotifier(this);
}

void CMainWindow::ResetConfig(base::ini::table& table)
{
	table["MapSave"]["Option"] = "0";
	table["War3Patch"]["Option"] = "0";
	table["MapTest"]["LaunchRenderingEngine"] = "Direct3D 8";
	table["MapTest"]["LaunchWindowed"] = "1";
	table["MapTest"]["LaunchFullWindowed"] = "0";
	table["MapTest"]["LaunchWideScreenSupport"] = "1";
	table["MapTest"]["LaunchLockingMouse"] = "0";
	table["MapTest"]["LaunchFixedRatioWindowed"] = "0";
	table["MapTest"]["LaunchDisableSecurityAccess"] = "0";
	table["MapTest"]["EnableHost"] = "0";
	table["MapTest"]["EnableMapSlk"] = "0";
	table["ScriptCompiler"]["EnableJassHelper"] = "1";
	table["ScriptCompiler"]["EnableJassHelperDebug"] = "0";
	table["ScriptCompiler"]["EnableJassHelperOptimization"] = "1";
	table["PJass"]["Option"] = "0";
	table["ScriptInjection"]["Option"] = "0";
	table["HostTest"]["Option"] = "0";
	table["ThirdPartyPlugin"]["EnableDotNetSupport"] = "0";
	table["ThirdPartyPlugin"]["EnableTesh"] = "1";
	table["FeatureToggle"]["EnableManualNewId"] = "0";
	table["Font"]["FontEnable"] = "0";
	table["Font"]["FontName"] = "";
	table["Font"]["FontSize"] = "12";
}

std::wstring Warcraft3_custom_path;
bool CMainWindow::LoadConfig(base::ini::table& table)
{
	try
	{
		ResetConfig(table);
		auto buf = base::file::read_stream(base::path::self().parent_path() / L"EverConfig.cfg").read<std::string>();
		base::ini::read(table, buf.c_str());
		try {
			bee::registry::key_w reg(L"HKEY_CURRENT_USER\\Software\\YDWE");
			Warcraft3_custom_path = reg[L"DIR_custom"].get_string();
		}
		catch (...) {}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CMainWindow::SaveConfig(base::ini::table const& table)
{
	try
	{
		base::file::write_stream(base::path::self().parent_path() / L"EverConfig.cfg").write(base::ini::write(table));
		bee::registry::key_w(L"HKEY_CURRENT_USER\\Software\\YDWE")[L"DIR_custom"] = Warcraft3_custom_path;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void CMainWindow::ConfigToUI(base::ini::table& table)
{
	foreach(auto it, configAttribute)
	{
		std::string const& name = it.first;
		Attribute const& attribute = it.second;
		if (attribute.Type() == Attribute::e_RadioButton)
		{
			int val = std::stoi(table[name][attribute.Section()]);

			if ((val >= 0) && (unsigned int(val) < attribute.Count()))
			{
				DuiLib::CCheckBoxUI* ctrl = m_controls[name + "_" + std::to_string((long long)val)];
				if (ctrl)
				{
					ctrl->Selected(true);
				}
			}
		}
		else if (attribute.Type() == Attribute::e_CheckBox)
		{
			DuiLib::CCheckBoxUI* ctrl = m_controls[name];
			if (ctrl)
			{
				ctrl->Selected(table[attribute.Section()][name] == "0");
				ctrl->Selected(table[attribute.Section()][name] != "0");
			}
		}
		else if (attribute.Type() == Attribute::e_ComboBox)
		{
			if (m_comboboxs[name]) {
				std::wstring font_name = bee::u2w(table[attribute.Section()][name]);
				for (int i = 0; i < m_comboboxs[name]->GetCount(); ++i)
				{
					if (font_name == ((DuiLib::CListLabelElementUI*)m_comboboxs[name]->GetItemAt(i))->GetText())
					{
						m_comboboxs[name]->SelectItem(i);
						break;
					}
				}
			}
		}
	}

	m_pm.SendNotify(m_pEnableJassHelper, DUI_MSGTYPE_SELECTCHANGED);
}

void CMainWindow::UIToConfig(base::ini::table& table)
{
	foreach(auto it, configAttribute)
	{
		std::string const& name = it.first;
		Attribute const& attribute = it.second;
		if (attribute.Type() == Attribute::e_RadioButton)
		{
			for (uint8_t i = 0; i < attribute.Count(); ++i)
			{
				DuiLib::CRadioButtonUI* ctrl = dynamic_cast<DuiLib::CRadioButtonUI*>(m_controls[name + "_" + std::to_string((long long)i)]);
				if (ctrl && ctrl->IsSelected())
				{
					table[name]["Option"] = std::to_string((long long)i);
					break;
				}
			}
		}
		else if (attribute.Type() == Attribute::e_CheckBox)
		{
			DuiLib::CCheckBoxUI* ctrl = m_controls[name];
			if (ctrl)
			{
				table[attribute.Section()][name] = ctrl->IsSelected()? "1" : "0";
			}
		}
		else if (attribute.Type() == Attribute::e_ComboBox)
		{
			if (m_comboboxs[name]) {
				table[attribute.Section()][name] = bee::w2u(CComboUI_GetSelectText(m_comboboxs[name]));
			}
		}
	}
}

void CMainWindow::EnableMapSave(bool bEnable)
{
	ContrlSetEnabled("MapSave_0", bEnable);
	ContrlSetEnabled("MapSave_1", bEnable);
	ContrlSetEnabled("MapSave_2", bEnable);
	ContrlSetEnabled("MapSave_3", bEnable);
	if (!bEnable) ContrlSelected("MapSave_0", true);
}

void CMainWindow::EnableScriptInjection(bool bEnable)
{
	ContrlSetEnabled("ScriptInjection_0", bEnable);
	ContrlSetEnabled("ScriptInjection_1", bEnable);
	if (!bEnable) ContrlSelected("ScriptInjection_1", true);
}

void CMainWindow::EnableHostTest(bool bEnable)
{
	ContrlSetEnabled("HostTest_0", bEnable);
	ContrlSetEnabled("HostTest_1", bEnable);
	if (!bEnable) ContrlSelected("HostTest_0", true);
}

void CMainWindow::EnableJassHelper(bool bEnable)
{
	ContrlSetEnabled("EnableJassHelperOptimization", bEnable);
	ContrlSetEnabled("EnableJassHelperDebug", bEnable);
}

void CMainWindow::InitRegistryUI()
{
	if (m_pAllowLocalFiles)
		m_pAllowLocalFiles->Selected(Registry::has(HKEY_CURRENT_USER, L"Software\\Blizzard Entertainment\\Warcraft III", L"Allow Local Files"));
}

void CMainWindow::DoneRegistryUI()
{
	if (m_pAllowLocalFiles)
		Registry::set(HKEY_CURRENT_USER, L"Software\\Blizzard Entertainment\\Warcraft III", L"Allow Local Files", m_pAllowLocalFiles->IsSelected());
}

void CMainWindow::InitOSHelpUI()
{
	if (m_pFileAssociation_w3x && m_pFileAssociation_w3m) {
		if (!fs::exists(m_ydwe_path))
		{
			if (m_pFileAssociation_w3x)
				m_pFileAssociation_w3x->SetEnabled(false);

			if (m_pFileAssociation_w3m)
				m_pFileAssociation_w3m->SetEnabled(false);

			if (m_pShortcuts_desktop)
				m_pShortcuts_desktop->SetEnabled(false);

			if (m_pShortcuts_taskbar)
				m_pShortcuts_taskbar->SetEnabled(false);
		}
		else
		{
			FileAssociationS::initialize(m_ydwe_path);

			if (m_pFileAssociation_w3x)
				m_pFileAssociation_w3x->Selected(FileAssociationS::get()->has_w3x());

			if (m_pFileAssociation_w3m)
				m_pFileAssociation_w3m->Selected(FileAssociationS::get()->has_w3m());

			if (m_pShortcuts_desktop)
				m_pShortcuts_desktop->Selected(Shortcuts::Desktop::Has(m_ydwe_path));

			if (m_pShortcuts_taskbar)
				m_pShortcuts_taskbar->Selected(Shortcuts::Taskbar::Has(m_ydwe_path));
		}
	}

	if (bee::platform::get_version().ver >= bee::platform::WinVer::Win7)
	{
		if (m_pShortcuts_taskbar)
			m_pShortcuts_taskbar->SetEnabled(false);
	}

	DuiLib::CRadioButtonUI* ui_lang = dynamic_cast<DuiLib::CRadioButtonUI*>(m_pm.FindControl(L"Language-" + base::i18n::get_language()));
	if (ui_lang) {
		ui_lang->Selected(true);
	}
}

void CMainWindow::DoneOSHelpUI()
{
	if (fs::exists(m_ydwe_path))
	{
		if (m_pFileAssociation_w3x && m_pFileAssociation_w3m)
		{
			FileAssociationS::get()->remove();

			if (m_pFileAssociation_w3x->IsSelected())
				FileAssociationS::get()->set_w3x();

			if (m_pFileAssociation_w3m->IsSelected())
				FileAssociationS::get()->set_w3m();

			if (m_pFileAssociation_w3x->IsSelected() || m_pFileAssociation_w3m->IsSelected())
				FileAssociationS::get()->set_classes();
		}

		if (m_pShortcuts_desktop)
		{
			if (m_pShortcuts_desktop->IsSelected())
			{
				Shortcuts::Desktop::CreateOrUpdate(m_ydwe_path);
			}
			else
			{
				Shortcuts::Desktop::Remove(m_ydwe_path);
			}
		}

		if (m_pShortcuts_taskbar)
		{
			if (m_pShortcuts_taskbar->IsSelected())
			{
				Shortcuts::Taskbar::CreateOrUpdate(m_ydwe_path);
			}
			else
			{
				Shortcuts::Taskbar::Remove(m_ydwe_path);
			}
		}
	}

	DuiLib::CStdPtrArray* group = m_pm.GetOptionGroup(L"Language");
	for (int i = 0; i < group->GetSize(); i++)
	{
		DuiLib::CRadioButtonUI* control = static_cast<DuiLib::CRadioButtonUI*>(group->GetAt(i));
		if (control && control->IsSelected() && control->GetName().substr(0, 9) == L"Language-")
		{
			auto newlang = control->GetName().substr(9);
			if (newlang != base::i18n::get_language()) {
				base::i18n::set_language(newlang, true);
			}
		}
	}
}

void CMainWindow::InitPluginUI()
{
	if (!m_pWar3PluginList)
		return;

	try {
		fs::path plugin_path = m_ydwe_path.parent_path() / L"plugin" / L"warcraft3";

		base::ini::table table;
		try {
			auto buf = base::file::read_stream(plugin_path / L"config.cfg").read<std::string>();
			base::ini::read(table, buf.c_str());	
		}
		catch(...) {
		}

		fs::directory_iterator end_itr;
		for (fs::directory_iterator itr(plugin_path); itr != end_itr; ++itr)
		{
			try {
				if (!fs::is_directory(*itr))
				{
					if (bee::path_helper::equal(itr->path().extension(), L".dll") 
						&& (!bee::path_helper::equal(itr->path().filename(), L"yd_loader.dll")))
					{
                        bee::module_version fv(itr->path().c_str());
						fv.select_language(base::i18n::get_languageid());

						DuiLib::CCheckBoxUI* node = new DuiLib::CCheckBoxUI;
						m_pWar3PluginList->Add(node);
						node->SetManager(m_pWar3PluginList->GetManager(), m_pWar3PluginList, false);
						node->SetAttribute(L"class", L"CheckBox");
						node->SetAttribute(L"text", fv[L"FileDescription"]);
						node->SetAttribute(L"tooltip", itr->path().filename().c_str());
						node->Selected("0" != table["Enable"][bee::w2u(itr->path().filename().wstring())]);
					}
				}
			}
			catch(...) {
			}
		}
	}
	catch(...) {
	}
}

void CMainWindow::DonePluginUI()
{
	if (!m_pWar3PluginList)
		return ;

	try {
		base::ini::table table;

		for (int i = 0; i < m_pWar3PluginList->GetCount(); ++i) 
		{
			DuiLib::CCheckBoxUI* pCheckBox = dynamic_cast<DuiLib::CCheckBoxUI*>(m_pWar3PluginList->GetItemAt(i));

			if (pCheckBox)
			{
				
				table["Enable"][bee::w2u(pCheckBox->GetToolTip())] = pCheckBox->IsSelected()? "1": "0";
			}
		}

		fs::path plugin_path = m_ydwe_path.parent_path() / L"plugin" / L"warcraft3";
		base::file::write_stream(plugin_path / L"config.cfg").write(base::ini::write(table));
	}
	catch (...) {
	}
}

void CMainWindow::InitPatchUI(base::ini::table& table)
{
	if (!m_pWar3PatchList)
		return;

	bool empty = true;
	fs::path patch_path = m_ydwe_path.parent_path() / L"share" / L"patch";

	if (fs::exists(patch_path))
	{
		fs::directory_iterator endItr;
		for (fs::directory_iterator itr(patch_path); itr != endItr; ++itr)
		{
			fs::path patch = itr->path();
			if (fs::is_directory(patch))
			{
				fs::path game_dll  = patch / L"Game.dll";
				fs::path patch_mpq = patch / L"Patch.mpq";
				if (fs::exists(game_dll) && fs::exists(patch_mpq))
				{
					patch = fs::absolute(patch);
					bee::module_version versionInfo(game_dll.c_str());

					DuiLib::CRadioButtonUI* node = new DuiLib::CRadioButtonUI;
					m_pWar3PatchList->Add(node);
					node->SetManager(m_pWar3PatchList->GetManager(), m_pWar3PatchList, false);
					node->SetAttribute(L"class", L"NormalRadioButton");
					node->SetAttribute(L"group", L"War3PatchItem");
					node->SetAttribute(L"text", (std::wstring(L"[") + versionInfo[L"FileVersion"] + L"]" + patch.filename().wstring()).c_str());
					node->SetAttribute(L"tooltip", patch.c_str());
					node->Selected(bee::path_helper::equal(patch.filename(), table["War3Patch"]["DirName"]));
					empty = false;
				}
			}
		}

		m_pWar3PatchList->SetEnabled("2" == table["War3Patch"]["Option"]);
	}

	if (empty)
	{
		DuiLib::CLabelUI* node;

		node = new DuiLib::CLabelUI;
		m_pWar3PatchList->Add(node);
		node->SetManager(m_pWar3PatchList->GetManager(), m_pWar3PatchList, false);
		node->SetAttribute(L"font", L"2");
		node->SetAttribute(L"text", L"请先去www.ydwe.net下载补丁包");
	}
}

void CMainWindow::DonePatchUI(base::ini::table& table)
{
	if (m_pWar3PatchList && m_controls["War3Patch_2"] && m_controls["War3Patch_2"]->IsSelected())
	{
		m_pWar3PatchList->ForeachControl([&table](DuiLib::CControlUI* pControl)->bool
		{
			DuiLib::CRadioButtonUI* pRadioButton = dynamic_cast<DuiLib::CRadioButtonUI*>(pControl);
			if (pRadioButton && pRadioButton->IsSelected())
			{
				table["War3Patch"]["DirName"] = bee::w2u(fs::path(pRadioButton->GetToolTip()).filename().wstring());
				return false;
			}
			return true;
		});
	}
}

void CMainWindow::InitFontUI()
{
	if (m_pFontName) {
		std::set<std::wstring> font_names = base::font::get_list();
		for (auto it = font_names.begin(); it != font_names.end(); ++it)
		{
			DuiLib::CListLabelElementUI* elem = new DuiLib::CListLabelElementUI;
			elem->SetText(it->c_str());
			elem->SetManager(m_pFontName->GetManager(), m_pFontName, false);
			m_pFontName->Add(elem);
		}
	}
}

void CMainWindow::UpdateWarcraft3Directory()
{
	if (m_pWarcraft3Directory)
	{
		fs::path result;
		if (warcraft3::directory::read(result))
		{
			m_pWarcraft3Directory->SetText(result.c_str());
		}
		else
		{
			m_pWarcraft3Directory->SetText(L"");
		}
	}
}

void CMainWindow::UpdateWarcraft3Directory1()
{
	if (m_pWarcraft3Directory1)
		m_pWarcraft3Directory1->SetText(Warcraft3_custom_path.c_str());
}

void CMainWindow::Notify(DuiLib::TNotifyUI& msg)
{
	try
	{
		if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
		{
			base::ini::table table;
			if (!LoadConfig(table)) { ResetConfig(table); }
			InitFontUI();
			ConfigToUI(table);
			InitRegistryUI();
			InitOSHelpUI();
			InitPatchUI(table);
			InitPluginUI();
			UpdateWarcraft3Directory();
			UpdateWarcraft3Directory1();
			m_username = table["MapTest"]["UserName"];
			m_virtualmpq = table["MapTest"]["VirtualMpq"];
		}
		else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED) 
		{
			if (m_pEnableJassHelper && m_pEnableJassHelper == msg.pSender)
			{
				bool bEnable = m_pEnableJassHelper->IsSelected();
				EnableJassHelper(bEnable);
			}
			else if (m_pLaunchWindowed && m_pLaunchWindowed == msg.pSender)
			{
				bool bEnable = m_pLaunchWindowed->IsSelected();
				ContrlSetEnabled("LaunchFullWindowed",       bEnable);
				ContrlSetEnabled("LaunchLockingMouse",       bEnable);
				ContrlSetEnabled("LaunchFixedRatioWindowed", bEnable);
			}
			else if (m_pFontEnable && m_pFontEnable == msg.pSender)
			{
				bool bEnable = m_pFontEnable->IsSelected();
				m_pFontName->SetEnabled(bEnable);
				m_pFontSize->SetEnabled(bEnable);
				m_pFontPreview->SetEnabled(bEnable);
			}
			else if (m_pEnableHost && m_pEnableHost == msg.pSender)
			{
				bool bEnable = m_pEnableHost->IsSelected();
				EnableHostTest(bEnable);
			}
			else if (m_pLaunchWideScreenSupport && m_pLaunchWideScreenSupport == msg.pSender)
			{
				bool bEnable = m_pLaunchWideScreenSupport->IsSelected();
				ContrlSelected("LaunchFixedRatioWindowed", !bEnable);
			}
			else if (m_pLaunchFixedRatioWindowed && m_pLaunchFixedRatioWindowed == msg.pSender)
			{
				bool bEnable = m_pLaunchFixedRatioWindowed->IsSelected();
				ContrlSelected("LaunchWideScreenSupport", !bEnable);
			}
		}
		else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if (msg.pSender == m_pFontName || msg.pSender == m_pFontSize)
			{
				static size_t dynfont = m_pm.AddFont(L"system", 12, false, false, false);

				try {
					m_pm.ReplaceFont(dynfont, CComboUI_GetSelectText(m_pFontName).c_str(), base::font::size_to_height(std::stoi(CComboUI_GetSelectText(m_pFontSize))), false, false, false);
				} catch (...) { }
				m_pFontPreview->SetFont(dynfont);
			}
		}
		else if (msg.sType == DUI_MSGTYPE_CLICK) 
		{
			std::wstring const& name = msg.pSender->GetName();
			if (name == L"ok") 
			{
				base::ini::table table;
				UIToConfig(table);
				DoneRegistryUI();
				DoneOSHelpUI();
				DonePatchUI(table);
				DonePluginUI();
				table["MapTest"]["UserName"] = m_username;
				table["MapTest"]["VirtualMpq"] = m_virtualmpq;
				SaveConfig(table);

				::PostQuitMessage(0L);
			}
			else if (name == L"cancel")
			{
				::PostQuitMessage(0L);
			}
			else if (name == L"reset")
			{
				base::ini::table table;
				ResetConfig(table);
				table["MapTest"]["UserName"] = m_username;
				table["MapTest"]["VirtualMpq"] = m_virtualmpq;
				ConfigToUI(table);
			}
			else if (name == L"choose_war3_dir")
			{
				if (warcraft3::directory::choose(nullptr))
				{
					UpdateWarcraft3Directory();
				}

			}
			else if (name == L"choose_war3_dir1")
			{
				std::wstring retval;
				if (!warcraft3::directory::open_file_dialog(nullptr, L"Warcraft III.exe\0Warcraft III.exe\0", NULL, &retval))
					return;
				fs::path result = fs::path(retval).parent_path();
				if (!fs::exists(result))
					return;
				Warcraft3_custom_path = result.wstring();
				UpdateWarcraft3Directory1();
			}
			else if (name.size() == 11 && name.substr(0, 10) == L"War3Patch_")
			{
				if (m_pWar3PatchList)
				{
					long n = std::stol(name.substr(10));
					m_pWar3PatchList->SetEnabled(n == 2);
				}
			}
			else if (name.size() == 5 && name.substr(0, 4) == L"tab_")
			{
				DuiLib::CTabLayoutUI* tabs = dynamic_cast<DuiLib::CTabLayoutUI*>(m_pm.FindControl(L"tabs"));
				if (tabs)
				{
					tabs->SelectItem(std::stol(name.substr(4)));
				}
			}
		}
	}
	catch (std::exception const& /*e*/)
	{
		assert(false);
	}
}
