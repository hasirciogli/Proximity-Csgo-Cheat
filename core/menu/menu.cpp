#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "core/menu/hNotifier/hNotifier.hpp"

#include "./chatbox/ChatBox.h"

#include "socket/msoket.h"
#include "socket/packet/Packet.h"

#include "core/features/features.hpp"

#include "core/menu/menu.hpp"
#include "core/config/config.hpp"

#include "core/features/visuals/skin_changer/skin_changer.hpp"
 
#include "dependencies/imgui/imgui_internal.h" 
#include "snowflake/snowflake.hpp"


#define zxcmin(a, b) (((a) < (b)) ? (a) : (b))
#define zxcmax(a, b) (((a) > (b)) ? (a) : (b))

HNOTIF::HNotifier hNotifierObject;

auto getZtkColor = [](int red, int green, int blue, int alpha = 255)
{
	return ImVec4((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, (float)alpha / 255.0f);
};

void setOurCustomImguiColorsAndEtc(LPDIRECT3DDEVICE9);
struct windowSize {
	float width; 
	float height;
};
auto getMenuPos = [](ImVec2& pos) {
	pos = ImGui::GetWindowPos(); 
};
auto getCursorPos = [](ImVec2& pos) {
	pos = ImGui::GetCursorPos();
};
void imspaceMacro(float x, float y) {
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + x, ImGui::GetCursorPos().y + y));
}
void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	float ww = ImGui::GetColumnWidth();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float ds = variables::Menu_Settings::DpiSize;

	float height = 8 * ds;
	float width = height * 2.55f;
	float radius = height * 1.f; 

	p.y += height * .5;

	ImGui::InvisibleButton(str_id, ImVec2(ww, height * 2));
	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.13f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id)) // && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

	auto col_bg2 = ImGui::GetColorU32(ImLerp(ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1), t));


	draw_list->AddText(variables::Menu_Settings::fonts_gubi_36_font, 16 * ds, ImVec2(p.x, ((p.y - (height * .5)) + (height - ((16* ds) /2)))), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), str_id);
	
	draw_list->AddRectFilled(ImVec2(p.x + (ww - width - 4 * ds), p.y), ImVec2(p.x + ww - 4 * ds, p.y + height), col_bg, height * 0.5f);
	
	draw_list->AddCircleFilled(ImVec2(p.x + (ww - width - 4 * ds) + t * (radius * 2.5f), p.y + radius * .5), radius - 1.5f, col_bg2);
}
void GetWindowSizeX(windowSize& size, LPDIRECT3DDEVICE9 pDevice);
bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	// Render
	window->DrawList->PathClear();

	int num_segments = 30;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
			centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);
}
void GetWindowSizeX(windowSize& size, LPDIRECT3DDEVICE9 pDevice) {
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	pDevice->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);


	size.width = (float)rect.right - rect.left;
	size.height = (float)rect.bottom - rect.top;
}


bool renderLeftPageSelectorItem(std::string ButtonName, ImTextureID buttonImage, int selectedPageId) {
	float _iDpiSize = variables::Menu_Settings::DpiSize;

	imspaceMacro(30 * _iDpiSize, 16 * _iDpiSize);

	if (selectedPageId == variables::Menu_Settings::MenuSelectedPage)
	{
		if (variables::Menu_Settings::DarkMode)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.1, .1, .1, .1));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.5, .5, .5, .5));
		}

	}
	else {
		if (variables::Menu_Settings::DarkMode)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.4, .4, .4, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.6, 0.6, 1));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.5, .5, .5, .5));
		}
	}


	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);

	ImGui::BeginChild(ButtonName.c_str(), ImVec2((279 - 60) * _iDpiSize, 40 * _iDpiSize));
	{
		imspaceMacro(15 * _iDpiSize, 10 * _iDpiSize);
		ImGui::Image(buttonImage, ImVec2(20 * _iDpiSize, 20 * _iDpiSize));
		ImGui::SameLine();
		imspaceMacro(24 * _iDpiSize, 0);
		ImGui::Text(ButtonName.c_str());
	}
	ImGui::EndChild();

	ImGui::PopStyleVar();

	if (selectedPageId == variables::Menu_Settings::MenuSelectedPage)
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	else {
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	if (ImGui::IsItemClicked())
	{
		variables::Menu_Settings::MenuSelectedPage = selectedPageId;
		return true;
	};

	if (ImGui::IsItemHovered())
	{

	}


	return false;
}
bool ImagedSectionButton(const char* str_id, ImTextureID image, int v, const char* c = nullptr)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float ds = variables::Menu_Settings::DpiSize;

	float height = 48 * ds;
	float width = (279 - 20) * ds;

	imspaceMacro(10 * ds, 0);
	p.x += 10 * ds;

	if (c != nullptr)
		height += 13 * ds;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
		return true;

	bool hovered = false;
	bool selected = false;

	if (ImGui::IsItemHovered())
		hovered = true;

	selected = variables::Menu_Settings::MenuSelectedPage == v;

	float t = hovered ? 1.0f : 0.0f;

	ImU32 col_bg;


	col_bg = ImGui::GetColorU32(hovered || selected ? ImVec4(0.7f, 0.7f, 0.7f, 0.1f) : ImVec4(0, 0, 0, 0));


	if (c != nullptr)
	{
		ImVec2 cs = ImGui::CalcTextSize(c);

		draw_list->AddText(variables::Menu_Settings::fonts_kollektifn_20_font, 10 * ds, ImVec2(p.x, p.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), c);

		p.y += 10 * ds;
		p.y += 3 * ds;

		height -= 13 * ds;
	}

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, 3);
	draw_list->AddImage(image, ImVec2(p.x + 20 * ds, p.y + 9 * ds), ImVec2(p.x + 50 * ds, p.y + height - 9 * ds));

	draw_list->AddText(variables::Menu_Settings::fonts_gubi_36_font, 16 * ds, ImVec2(p.x + 65 * ds, p.y + 17 * ds), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), str_id);

	//draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, 3);
	//draw_list->AddCircleFilled(ImVec2(p.x + 0 + t * (radius * 2.5f), p.y + radius * .5), radius - 1.5f, IM_COL32(255, 255, 255, 255));

	return false;
}

#ifdef ZORT
using namespace ens;


HNOTIF::HNotifier hNotifierObject;

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))


//float alertboxX = 10.0f;
//float alertboxY = 10.0f;
//float alertboxWidth = 300.0f;
//float alertboxHeight = 100.0f;
//float lineLength = 300.0f;
//float lineThickness = 2.0f;
//float alpha = 255.0f;
//
//// Bu fonksiyon, alertbox'ý animasyonlu bir þekilde gösterir
//void showAlertBox() {
//	// Alertbox'ýn pozisyonunu ve özelliklerini güncelle
//	alertboxX += (targetX - alertboxX) * 0.1f;
//	alertboxY += (targetY - alertboxY) * 0.1f;
//	lineLength -= 3.0f * Time.deltaTime;
//	alpha -= Time.deltaTime * 255.0f;
//
//	// Alertbox'ý çiz
//	ImGui.SetNextWindowPos(new Vector2(alertboxX, alertboxY));
//	ImGui.SetNextWindowSize(new Vector2(alertboxWidth, alertboxHeight));
//	ImGui.Begin("Alert Box", ref showAlertBox, ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse);
//
//	// Ufak çizgi çiz
//	ImGui.GetWindowDrawList().AddLine(new Vector2(lineLength, alertboxHeight - lineThickness), new Vector2(lineLength, alertboxHeight), ImGui.GetColorU32(new Vector4(1, 1, 1, alpha / 255.0f)), lineThickness);
//
//	// Alertbox'ýn içeriðini oluþtur
//	ImGui.Text("This is an alert box!");
//	ImGui.End();
//}



std::string get_wep(int id, int custom_index = -1, bool knife = true)
{
	if (custom_index > -1)
	{
		if (knife)
		{
			switch (custom_index)
			{
			case 0: return "weapon_knife";
			case 1: return "weapon_bayonet";
			case 2: return "weapon_knife_css";
			case 3: return "weapon_knife_skeleton";
			case 4: return "weapon_knife_outdoor";
			case 5: return "weapon_knife_cord";
			case 6: return "weapon_knife_canis";
			case 7: return "weapon_knife_flip";
			case 8: return "weapon_knife_gut";
			case 9: return "weapon_knife_karambit";
			case 10: return "weapon_knife_m9_bayonet";
			case 11: return "weapon_knife_tactical";
			case 12: return "weapon_knife_falchion";
			case 13: return "weapon_knife_survival_bowie";
			case 14: return "weapon_knife_butterfly";
			case 15: return "weapon_knife_push";
			case 16: return "weapon_knife_ursus";
			case 17: return "weapon_knife_gypsy_jackknife";
			case 18: return "weapon_knife_stiletto";
			case 19: return "weapon_knife_widowmaker";
			}
		}
		else
		{
			switch (custom_index)
			{
			case 0: return "ct_gloves";
			case 1: return "studded_bloodhound_gloves";
			case 2: return "t_gloves";
			case 3: return "ct_gloves";
			case 4: return "sporty_gloves";
			case 5: return "slick_gloves";
			case 6: return "leather_handwraps";
			case 7: return "motorcycle_gloves";
			case 8: return "specialist_gloves";
			case 9: return "studded_hydra_gloves";
			}
		}
	}
	else
	{
		switch (id)
		{
		case 0: return "knife";
		case 1: return "gloves";
		case 2: return "weapon_ak47";
		case 3: return "weapon_aug";
		case 4: return "weapon_awp";
		case 5: return "weapon_cz75a";
		case 6: return "weapon_deagle";
		case 7: return "weapon_elite";
		case 8: return "weapon_famas";
		case 9: return "weapon_fiveseven";
		case 10: return "weapon_g3sg1";
		case 11: return "weapon_galilar";
		case 12: return "weapon_glock";
		case 13: return "weapon_m249";
		case 14: return "weapon_m4a1_silencer";
		case 15: return "weapon_m4a1";
		case 16: return "weapon_mac10";
		case 17: return "weapon_mag7";
		case 18: return "weapon_mp5sd";
		case 19: return "weapon_mp7";
		case 20: return "weapon_mp9";
		case 21: return "weapon_negev";
		case 22: return "weapon_nova";
		case 23: return "weapon_hkp2000";
		case 24: return "weapon_p250";
		case 25: return "weapon_p90";
		case 26: return "weapon_bizon";
		case 27: return "weapon_revolver";
		case 28: return "weapon_sawedoff";
		case 29: return "weapon_scar20";
		case 30: return "weapon_ssg08";
		case 31: return "weapon_sg556";
		case 32: return "weapon_tec9";
		case 33: return "weapon_ump45";
		case 34: return "weapon_usp_silencer";
		case 35: return "weapon_xm1014";
		default: return "unknown";
		}
	}
}

IDirect3DTexture9* get_skin_preview(const char* weapon_name, std::string skin_name, IDirect3DDevice9* device)
{
	IDirect3DTexture9* skin_image = nullptr;
	if (!skin_image)
	{
		std::string vpk_path;
		if (strcmp(weapon_name, "unknown") && strcmp(weapon_name, "knife") && strcmp(weapon_name, "gloves"))
		{
			if (skin_name == "" || skin_name == "default")
				vpk_path = "resource/flash/econ/weapons/base_weapons/" + std::string(weapon_name) + ".png";
			else
				vpk_path = "resource/flash/econ/default_generated/" + std::string(weapon_name) + "_" + std::string(skin_name) + "_light_large.png";
		}
		else
		{
			if (!strcmp(weapon_name, "knife"))
				vpk_path = "resource/flash/econ/weapons/base_weapons/weapon_knife.png";
			else if (!strcmp(weapon_name, "gloves"))
				vpk_path = "resource/flash/econ/weapons/base_weapons/ct_gloves.png";
			else if (!strcmp(weapon_name, "unknown"))
				vpk_path = "resource/flash/econ/weapons/base_weapons/weapon_snowball.png";

		}
		const auto handle = interfaces::bfsystem->Open(vpk_path.c_str(), "r", "GAME");
		if (handle)
		{
			int file_len = interfaces::bfsystem->Size(handle);
			char* image = new char[file_len];

			interfaces::bfsystem->Read(image, file_len, handle);
			interfaces::bfsystem->Close(handle);

			D3DXCreateTextureFromFileInMemory(device, image, file_len, &skin_image);

			delete[] image;
		}
	}
	if (skin_image != nullptr)
		return skin_image;
	else if (!skin_image)
	{
		std::string vpk_path;

		if (strstr(weapon_name, "bloodhound") != NULL || strstr(weapon_name, "hydra") != NULL)
			vpk_path = "resource/flash/econ/weapons/base_weapons/ct_gloves.png";
		else
			vpk_path = "resource/flash/econ/weapons/base_weapons/" + std::string(weapon_name) + ".png";

		const auto handle = interfaces::bfsystem->Open(vpk_path.c_str(), "r", "GAME");
		if (handle)
		{
			int file_len = interfaces::bfsystem->Size(handle);
			char* image = new char[file_len];

			interfaces::bfsystem->Read(image, file_len, handle);
			interfaces::bfsystem->Close(handle);

			D3DXCreateTextureFromFileInMemory(device, image, file_len, &skin_image);

			delete[] image;
		}
	}

	return skin_image;
}



/*
auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
	// Background
	render::draw_filled_rect(x, y, w, h, bg);
	// Header title
	render::draw_filled_rect(x, y, w, 30, header_text);
	render::draw_filled_rect(x, y + 30, w, 2, header_line);
	render::draw_text_string(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white(255));
};

void menu::render() {
	if (!variables::ui::menux::opened) return;

	// Menu title and all that
	do_frame(variables::ui::menux::x, variables::ui::menux::y, variables::ui::menux::w, variables::ui::menux::h,
		color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "Rogsoftware");

	#pragma region TABS

	constexpr int tab_number = 4;
	const int tab_size   = variables::ui::menux::w / tab_number;

	gui::tab(variables::ui::menux::x + (tab_size * 0), variables::ui::menux::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Aim", menu::current_tab, 0);
	gui::tab(variables::ui::menux::x + (tab_size * 1), variables::ui::menux::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Visuals", menu::current_tab, 1);
	gui::tab(variables::ui::menux::x + (tab_size * 2), variables::ui::menux::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Misc", menu::current_tab, 2);
	gui::tab(variables::ui::menux::x + (tab_size * 3), variables::ui::menux::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Config", menu::current_tab, 3);
	#pragma endregion

	#pragma region CONTAINERS

	gui::update_positions();		// Get the updated variables with current menux pos and all that

	switch (current_tab) {
		// Aim
		case 0: {
			gui::init_tab();

			gui::add_groupbox("General", 3); {
				gui::add_checkbox	("Also target friendly",				variables::aim::target_friends);
				gui::add_checkbox	("Enable noscope aimbot on snipers",	variables::aim::aimbot_noscope);
				gui::add_checkbox	("Enable non-rifle aim punch",			variables::aim::non_rifle_aimpunch);
			}

			gui::add_groupbox("Triggerbot", 3); {
				gui::add_checkbox	("Enable triggerbot",	variables::aim::triggerbot);
				gui::add_hotkey		("Triggerbot key",		variables::aim::triggerbot_key);
				gui::add_slider		("Triggerbot delay",	variables::aim::triggerbot_delay, 0.f, 30.f);
			}

			gui::add_groupbox("Aimbot", 13); {
				gui::add_checkbox		("Enable aimbot",			variables::aim::aimbot);
				gui::add_checkbox		("Autofire",				variables::aim::autofire);
				gui::add_checkbox		("Auto revolver",			variables::aim::autorevolver);
				gui::add_hotkey			("Only on key",				variables::aim::aimbot_key);
				gui::add_checkbox		("Silent",					variables::aim::silent);
				gui::add_combobox		("Autowall",				variables::aim::autowall_settings, variables::aim::autowall);
				gui::add_multicombobox	("Aimbot hitboxes",			variables::aim::hitboxes);
				gui::add_checkbox		("Bodyaim if lethal",		variables::aim::bodyaim_if_lethal);
				gui::add_checkbox		("Priorize lethal targets",	variables::aim::priorize_lethal_targets);
				gui::add_slider			("Minimum damage",			variables::aim::min_damage, 0.f, 100.f);
				gui::add_slider			("Aimbot fov",				variables::aim::aimbot_fov, 0.f, 180.f);
				gui::add_checkbox		("Draw fov",				variables::aim::draw_fov, variables::colors::aimbot_fov_c);
				gui::add_slider			("Aimbot smoothing",		variables::aim::aimbot_smoothing, 0.f, 1.f);
			}

			gui::add_column();

			gui::add_groupbox("Antiaim", 7); {
				gui::add_checkbox	("AntiAim",						variables::antiaim::antiaim);
				gui::add_slider		("Pitch (x)",					variables::antiaim::pitch, -89.f, 89.f);
				gui::add_slider		("Yaw (y)",						variables::antiaim::yaw, -180.f, 180.f);
				gui::add_checkbox	("Spinbot",						variables::antiaim::spinbot);
				gui::add_slider		("Spinbot speed",				variables::antiaim::spinbot_speed, 0.f, 100.f);
				gui::add_checkbox	("Peek antiaim (Manual)",		variables::antiaim::peek_aa);						// Probably needs name change
				gui::add_hotkey		("Peek antiaim toggle",			variables::antiaim::peek_aa_toggle_key);
			}
			break;
		}
		// Visuals
		case 1:	{
			gui::init_tab();

			gui::add_groupbox("Player ESP", 7); {
				gui::add_checkbox		("Enable team ESP (global)",	variables::player_visuals::showteamesp, variables::colors::friendly_color);
				gui::add_checkbox		("Box ESP",						variables::player_visuals::boxesp, variables::colors::enemy_color);
				gui::add_checkbox		("Skeleton ESP",				variables::player_visuals::skeletonesp, variables::colors::enemy_color_soft);
				gui::add_checkbox		("Name ESP",					variables::player_visuals::nameesp);
				gui::add_multicombobox	("Player info",					variables::player_visuals::playerinfo);
				gui::add_checkbox		("Health ESP",					variables::player_visuals::healthesp);
				gui::add_checkbox		("Line ESP",					variables::player_visuals::lineesp);
			}

			gui::add_groupbox("Chams", 13); {
				gui::add_checkbox("Only visible chams",			variables::chams::only_visible_chams);
				gui::add_checkbox("Wireframe chams",			variables::chams::wireframe_chams);
				gui::add_checkbox("Draw on top",				variables::chams::draw_chams_on_top);
				gui::add_checkbox("Player chams",				variables::chams::player_chams, variables::colors::chams_vis_friend_c, variables::colors::chams_vis_enemy_c);
				gui::add_combobox("Player chams material",		variables::chams::materials, variables::chams::player_chams_mat_id);
				gui::add_checkbox("Local player chams",			variables::chams::localplayer_chams, variables::colors::chams_localplayer);
				gui::add_combobox("Local chams material",		variables::chams::materials, variables::chams::localplayer_chams_mat_id);
				gui::add_checkbox("Viewmodel weapon chams",		variables::chams::vm_weapon_chams, variables::colors::chams_weapon_c);
				gui::add_combobox("Weapon chams material",		variables::chams::materials, variables::chams::weapon_chams_mat_id);
				gui::add_checkbox("Arms chams",					variables::chams::vm_arm_chams, variables::colors::chams_arms_c);
				gui::add_combobox("Arms chams material",		variables::chams::materials, variables::chams::arm_chams_mat_id);
				gui::add_checkbox("Sleeve chams",				variables::chams::vm_sleeve_chams, variables::colors::chams_sleeve_c);
				gui::add_combobox("Sleeve chams material",		variables::chams::materials, variables::chams::sleeve_chams_mat_id);
			}

			gui::add_column();

			gui::add_groupbox("Glow", 3); {
				gui::add_checkbox("Player glow",		variables::player_visuals::playerglow, variables::colors::friendly_glow_c, variables::colors::enemy_glow_c);
				gui::add_checkbox("Entity glow",		variables::entity_visuals::entityglow, variables::colors::entity_glow_c);
				gui::add_checkbox("Chicken pride",		variables::misc_visuals::chickenpride);
			}

			gui::add_groupbox("Other ESP", 3); {
				gui::add_checkbox("Bomb timer",				variables::entity_visuals::bombtimer);
				gui::add_checkbox("Nade projectile ESP",	variables::entity_visuals::nade_esp);
				gui::add_combobox("Entity info",			variables::entity_visuals::entity_esp_settings, variables::entity_visuals::entity_esp);
			}

			gui::add_groupbox("Misc", 9); {
				gui::add_slider			("No flash",				variables::misc_visuals::noflash_alpha, 0.f, 1.f);
				gui::add_checkbox		("Wireframe smoke",			variables::misc_visuals::wireframe_smoke);
				gui::add_checkbox		("Nade prediction",			variables::misc_visuals::nade_predict);
				gui::add_checkbox		("No sniper scope",			variables::misc_visuals::noscope);
				gui::add_checkbox		("Worldcolor",				variables::misc_visuals::worldcolor, variables::colors::worldcolor_c);
				gui::add_multicombobox	("Bullet tracers",			variables::misc_visuals::bulletracer_draw_target);
				gui::add_multicombobox	("Bullet tracer teams",		variables::misc_visuals::bulletracer_team_target);
				gui::add_checkbox		("Custom crosshair",		variables::misc_visuals::crosshair, variables::colors::crosshair_c);
				gui::add_checkbox		("Recoil crosshair",		variables::misc_visuals::recoil_crosshair, variables::colors::recoil_crosshair_c);
			}
			break;
		}
		case 2: {	// Misc
			gui::init_tab();

			gui::add_groupbox("Network", 4); {
				gui::add_checkbox	("Backtrack",			variables::misc::backtrack);
				gui::add_slider		("Backtrack ms",		variables::misc::backtrack_ticks, 0.f, 400.f);
				gui::add_checkbox	("Also friendly",		variables::misc::backtrack_team, variables::colors::bt_chams_friend_fade, variables::colors::bt_chams_friend);
				gui::add_checkbox	("Backtrack chams",		variables::chams::backtrack_chams, variables::colors::bt_chams_enemy_fade, variables::colors::bt_chams_enemy);
			}

			gui::add_groupbox("Movement", 11); {
				gui::add_checkbox	("Infinite duck (WARNING: Untrusted)",	variables::misc::infinite_duck);
				gui::add_checkbox	("Bhop",								variables::misc::bhop);
				gui::add_combobox	("Autostrafe",							variables::misc::autostrafe_options, variables::misc::autostrafe_target);
				gui::add_checkbox	("Enable edgejump",						variables::misc::edgejump);
				gui::add_hotkey		("Edgejump key",						variables::misc::ej_key);
				gui::add_checkbox	("Edgebug",								variables::misc::edgebug);
				gui::add_hotkey		("Edgebug key",							variables::misc::eb_key);
				gui::add_checkbox	("Jumpbug",								variables::misc::jumpbug);
				gui::add_hotkey		("Jumpbug key",							variables::misc::jb_key);
				gui::add_checkbox	("Slowwalk",							variables::misc::slowwalk);
				gui::add_hotkey		("Slowwalk key",						variables::misc::slowwalk_key);
			}

			gui::add_groupbox("Speedgraph", 3); {
				gui::add_multicombobox	("Speedgraph options",	variables::misc::speedgraph_target);
				gui::add_slider			("Speedgraph height",	variables::misc::speedgraph_h, 0.f, 100.f);
				gui::add_slider			("Speedgraph pos",		variables::misc::speedgraph_pos, 0.f, 100.f);
			}
			
			gui::add_column();

			gui::add_groupbox("Thirdperson", 3); {
				gui::add_checkbox	("Thirdperson",				variables::misc::thirdperson);
				gui::add_hotkey		("Thirdperson toggle key",	variables::misc::thirdperson_key);
				gui::add_slider		("Thirdperson distance",	variables::misc::thirdperson_dist, 50.f, 200.f);
			}

			gui::add_groupbox("Interface", 4); {
				gui::add_checkbox("Hide cheat on screenshots",	variables::misc::clean_screenshots);
				gui::add_multicombobox("Watermark",				variables::misc::watermark);
				gui::add_checkbox("Spectator list",				variables::ui::spectators::spectator_list);
				gui::add_checkbox("Reveal ranks",				variables::misc::reveal_ranks);
			}

			gui::add_groupbox("Fov", 2); {
				gui::add_slider("Custom FOV",				variables::misc_visuals::custom_fov_slider, 80.f, 130.f);
				gui::add_slider("Custom viewmodel FOV",		variables::misc_visuals::custom_vmfov_slider, 0.5f, 2.f);
			}

			gui::add_groupbox("Motion blur", 5); {
				gui::add_checkbox	("Enable motion blur",		variables::motion_blur.enabled);
				gui::add_checkbox	("Forward motion blur",		variables::motion_blur.forwardEnabled);
				gui::add_slider		("Strenght",				variables::motion_blur.strength, 0.f, 15.f);
				gui::add_slider		("Falling intensity",		variables::motion_blur.fallingIntensity, 0.f, 10.f);
				gui::add_slider		("Rotation intensity",		variables::motion_blur.rotationIntensity, 0.f, 10.f);
			}


			gui::add_bottom_groupbox(1);
			// Not as dynamic because its at the bottom and with the whole width
			gui::groupbox(gui::vars::o_container_left_pos, gui::vars::button_part_y, gui::vars::o_container_width, gui::vars::button_part_h, render::fonts::watermark_font, "Buttons", false); {
				gui::button(gui::vars::o_item_left_pos, gui::vars::button_base_item_y + (15 * 0), gui::vars::o_item_checkbox_pos - 20, render::fonts::watermark_font,		// Bigger "checkbox" as button
					"Exec autoexec", button_functions::exec_autoexec);																										// @todo: Pass size and pos
			}
			break;
		}
		case 3: {	// Config
			gui::init_tab();

			gui::add_groupbox("Config", 4); {
				gui::add_button		("Refresh configs",			config::refresh_list);
				gui::add_textbox	("Config name...",			config::new_config_name, config::create_new_config);
				gui::add_button		("Load selected config",	config::load_selected_config);
				gui::add_button		("Save selected config",	config::save_selected_config);
			}

			gui::add_groupbox("Skins", 2); {
				gui::add_button("Load skin config",					skins::read_skins);
				gui::add_button("Update game skins (Full update)",	button_functions::full_update);
			}

			gui::add_column();

			gui::add_groupbox("Config selection", config::max_configs); {
				gui::config_selection(gui::vars::container_left_pos, gui::vars::cur_base_item_y, gui::vars::container_width, render::fonts::watermark_font,
					config::config_names);
			}
			break;
		}
	}
	#pragma endregion

	#pragma region WINDOW MOVEMENTS
	spectator_framework::spec_list_movement(variables::ui::spectators::x, variables::ui::spectators::y, variables::ui::spectators::w, variables::ui::spectators::h);
	gui::menu_movement(variables::ui::menux::x, variables::ui::menux::y, variables::ui::menux::w, 30);
	#pragma endregion

}

// Toggle using IsPressed (GetAsyncKeyState)
void menu::check_toggle() {
	if (input::global_input.IsPressed(VK_XBUTTON2))
		variables::ui::menux::opened = !variables::ui::menux::opened;
}

// Makes the window positions relative based on screen size
void menu::init_windows() {
	int screen_w, screen_h;
	interfaces::engine->get_screen_size(screen_w, screen_h);

	variables::ui::menux::y = screen_h * 0.2;
	variables::ui::menux::x = screen_w * 0.2;
	variables::ui::spectators::y = screen_h * 0.5;		// For smaller screens
}
*/


/*void menux::renderImguiBasedMenu() {
	ImGui::ShowDemoWindow();
}*/

void imspaceMacro(float x, float y) {
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + x, ImGui::GetCursorPos().y + y));
}

bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	// Render
	window->DrawList->PathClear();

	int num_segments = 30;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
			centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);
}



void GetWindowSize(windowSize& size, LPDIRECT3DDEVICE9 pDevice) {
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	pDevice->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);


	size.width = (float)rect.right - rect.left;
	size.height = (float)rect.bottom - rect.top;
}

int calculateUiScalar(int scalaDef) {
	if (false)
	{
		if (variables::Menu_Settings::ui_scalar == 200)
			return scalaDef + ((scalaDef * 200) / 100);
		else if (variables::Menu_Settings::ui_scalar == 175)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 150)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 125)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 100)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 75)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 50)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 25)
			return 0000;
		else if (variables::Menu_Settings::ui_scalar == 0)
			return 0000;
		else
			return scalaDef;
	}

	int bclakcX = (scalaDef * variables::Menu_Settings::ui_scalar) / 100;

	if (bclakcX < 0)
		bclakcX = 0;
	return bclakcX;
}

void customComboBox(int& selectedItemID, std::vector<const char*>& lList) {
	ImGuiStyle& imguiStyles = ImGui::GetStyle();
	imguiStyles.FramePadding = ImVec2(4, 4);

	const char* selectedItemName = selectedItemID == -1 ? "none" : lList.at(selectedItemID);
	if (ImGui::BeginCombo("##combo", selectedItemName)) // The second parameter is the label previewed before opening the combo.
	{

		for (int n = 0; n < lList.size(); n++)
		{

			bool is_selected = (selectedItemID == n); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(lList.at(n), is_selected))
			{
				selectedItemID = is_selected ? -1 : n;
				ImGui::SetItemDefaultFocus();

			}

		}
		ImGui::EndCombo();
	}

	imguiStyles.FramePadding = ImVec2(1.2f, 1.2f);
}

void renderAimbotPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height - 60));

	ImGui::BeginChild("#aimbot_menu_basement", cMenuSize, false);
	{
		ImGui::BeginChild("#aa_state_left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Enabled", &variables::Aimbot_Settings::enabled);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Team Check", &variables::Aimbot_Settings::teamCheck);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("No Scope", &variables::Aimbot_Settings::noScope);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Fire", &variables::Aimbot_Settings::autofire);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Silent Aim", &variables::Aimbot_Settings::silentAim);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Non Rifle Aim Punch", &variables::Aimbot_Settings::non_rifle_aimpunch);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Revolver", &variables::Aimbot_Settings::auto_revolver);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Draw Fov", &variables::Aimbot_Settings::drawFov);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Priorize Lethal Targets", &variables::Aimbot_Settings::priorize_lethal_targets);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Wall", &variables::Aimbot_Settings::auto_wall);

			/*imspaceMacro(10, 10);

			
			if (ImGui::Button("add me"))
			{
				hNotifierObject.addNotification(-1, "test", "work");
			}
			*/




			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Aimbot Fov").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Aimbot Fov", &variables::Aimbot_Settings::aimbot_fov, 0, 180.f, "%.1f");

			ImGui::PopItemWidth();




			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Aimbot Hitchance").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Aimbot Hitchance", &variables::Aimbot_Settings::aimbot_hitchance, 0.f, 125.f, "%.1f");

			ImGui::PopItemWidth();





			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Smoothness").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Smoothness", &variables::Aimbot_Settings::aimbot_smoothing,0.f, 1.f);

			ImGui::PopItemWidth();





			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Min Damage").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Min Damage", &variables::Aimbot_Settings::min_damage, 0, 120.f, "%.0f");

			ImGui::PopItemWidth();

			 
			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);


		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#aa_state_right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			ImGui::PushItemWidth(cMenuSize.x / 2 - 30);
			{
				imspaceMacro(10, 10);
				ImGui::ListBoxHeader("#aimbot_hitboxes_list", 4, 10);
				{
					for (size_t i = 0; i < IM_ARRAYSIZE(variables::Aimbot_Settings::selected_hitboxes); i++)
					{
						bool* isSelected = &variables::Aimbot_Settings::selected_hitboxes[i];

						if (ImGui::Selectable(variables::Aimbot_Settings::selected_hitboxes_names[i], *isSelected))
						{
							*isSelected = !*isSelected;
						}
					}
				}
				ImGui::ListBoxFooter();
			}
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void renderAntiAimPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));

	ImGui::BeginChild("#AntiAimMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#aa_state_left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Enabled", &variables::AA_Settings::antiaim);

			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Pitch").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Pitch", &variables::AA_Settings::pitch, -90, 90);

			ImGui::PopItemWidth();


			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Yaw").x + 30));

			imspaceMacro(10, 10);
			ImGui::SliderFloat("Yaw", &variables::AA_Settings::yaw, -360, 360);

			ImGui::PopItemWidth();





			/*imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Enabled", &variables::Aimbot_Settings::enabled);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Team Check", &variables::Aimbot_Settings::teamCheck);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Fire", &variables::Aimbot_Settings::autofire);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Revolver", &variables::Aimbot_Settings::auto_revolver);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Auto Wall", &variables::Aimbot_Settings::auto_wall);
			imspaceMacro(10, 10);
			ImGui::PushItemWidth(cMenuSize.x / 2 - (ImGui::CalcTextSize("Min Damage").x + 30));
			ImGui::SliderFloat("Min Damage", &variables::Aimbot_Settings::min_damage, 0, 120.f, "%.0f");
			imspaceMacro(10, 10);
			ImGui::SliderFloat("Aimbot Fov", &variables::Aimbot_Settings::aimbot_fov, 0, 180.f, "%.1f");
			ImGui::PopItemWidth();*/

			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#aa_state_right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {

		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void renderEspPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));
	ImGuiStyle& imguiStyles2 = ImGui::GetStyle();
	ImGui::BeginChild("#EspMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#esp-state-left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			if (variables::Esp_Settings::selected_team == 0)
				imguiStyles2.Colors[ImGuiCol_Button] = ImColor(35, 90, 30);



			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Enabled Base", &variables::Esp_Settings::enabledBase);


			imspaceMacro(10, 10);
			if (ImGui::Button("Enemy", ImVec2((cMenuSize.x / 2) / 3 - 13.3, 30)))
				variables::Esp_Settings::selected_team = 0;

			ImGui::SameLine();

			imguiStyles2.Colors[ImGuiCol_Button] = ImColor(40, 40, 40);

			if (variables::Esp_Settings::selected_team == 1)
				imguiStyles2.Colors[ImGuiCol_Button] = ImColor(35, 90, 30);
			imspaceMacro(10, 0);
			if (ImGui::Button("Team", ImVec2((cMenuSize.x / 2) / 3 - 13.3, 30)))
				variables::Esp_Settings::selected_team = 1;
			ImGui::SameLine();
			imguiStyles2.Colors[ImGuiCol_Button] = ImColor(40, 40, 40);

			if (variables::Esp_Settings::selected_team == 2)
				imguiStyles2.Colors[ImGuiCol_Button] = ImColor(35, 90, 30);

			imspaceMacro(10, 0);
			if (ImGui::Button("Local", ImVec2((cMenuSize.x / 2) / 3 - 13.3, 30)))
				variables::Esp_Settings::selected_team = 2;

			imguiStyles2.Colors[ImGuiCol_Button] = ImColor(40, 40, 40);

			imspaceMacro(10, 20);
			ImGui::CheckboxTypeB("Name", &variables::Esp_Settings::enabledNameesp[variables::Esp_Settings::selected_team]);

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Health", &variables::Esp_Settings::enabledHealthesp[variables::Esp_Settings::selected_team]);

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Box", &variables::Esp_Settings::enabledBox[variables::Esp_Settings::selected_team]);

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Line", &variables::Esp_Settings::enabledLine[variables::Esp_Settings::selected_team]);

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Skeleton", &variables::Esp_Settings::enabledSkeleton[variables::Esp_Settings::selected_team]);

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Other Info", &variables::Esp_Settings::enabledGethers[variables::Esp_Settings::selected_team]);



			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#esp-state-right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			{
				imspaceMacro(10, 10);
				ImGui::Text("Enemy Box Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_box_color", variables::Esp_Settings::colors_box[0]);

				imspaceMacro(10, 10);
				ImGui::Text("Enemy Name Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_name_color", variables::Esp_Settings::colors_name[0]);

				imspaceMacro(10, 10);
				ImGui::Text("Enemy Line Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_line_color", variables::Esp_Settings::colors_line[0]);

				imspaceMacro(10, 10);
				ImGui::Text("Enemy Skeleton Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_skeleton_color", variables::Esp_Settings::colors_skeleton[0]);

				imspaceMacro(10, 10);
				ImGui::Text("Enemy Health Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_health_color", variables::Esp_Settings::colors_health[0]);

				imspaceMacro(10, 10);
				ImGui::Text("Enemy Info Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##enemy_info_color", variables::Esp_Settings::colors_info[0]);
			}

			imspaceMacro(10, 15);
			ImGui::Separator();

			{
				imspaceMacro(10, 10);
				ImGui::Text("Team Box Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_box_color", variables::Esp_Settings::colors_box[1]);

				imspaceMacro(10, 10);
				ImGui::Text("Team Name Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_name_color", variables::Esp_Settings::colors_name[1]);

				imspaceMacro(10, 10);
				ImGui::Text("Team Line Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_line_color", variables::Esp_Settings::colors_line[1]);

				imspaceMacro(10, 10);
				ImGui::Text("Team Skeleton Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_skeleton_color", variables::Esp_Settings::colors_skeleton[1]);

				imspaceMacro(10, 10);
				ImGui::Text("Team Health Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_health_color", variables::Esp_Settings::colors_health[1]);

				imspaceMacro(10, 10);
				ImGui::Text("Team Info Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##team_info_color", variables::Esp_Settings::colors_info[1]);
			}



			imspaceMacro(10, 15);
			ImGui::Separator();

			{
				imspaceMacro(10, 10);
				ImGui::Text("Local Box Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_box_color", variables::Esp_Settings::colors_box[2]);

				imspaceMacro(10, 10);
				ImGui::Text("Local Name Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_name_color", variables::Esp_Settings::colors_name[2]);

				imspaceMacro(10, 10);
				ImGui::Text("Local Line Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_line_color", variables::Esp_Settings::colors_line[2]);

				imspaceMacro(10, 10);
				ImGui::Text("Local Skeleton Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_skeleton_color", variables::Esp_Settings::colors_skeleton[2]);

				imspaceMacro(10, 10);
				ImGui::Text("Local Health Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_health_color", variables::Esp_Settings::colors_health[2]);

				imspaceMacro(10, 10);
				ImGui::Text("Local Info Color");
				imspaceMacro(10, 2);
				ImGui::ColorEdit4("##local_info_color", variables::Esp_Settings::colors_info[2]);
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void renderChamsPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));
	ImGuiStyle& imguiStyles2 = ImGui::GetStyle();
	ImGui::BeginChild("#ChamsMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#ChamsMenu-state-left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {

			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Chams General", &variables::chams::general_chams_enabled);
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Draw Top", &variables::chams::draw_chams_on_top);

			imspaceMacro(10, 30);
			ImGui::CheckboxTypeB("Enemy Chams", &variables::chams::enemy_chams);
			if (variables::chams::enemy_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Enemy Only Visible", &variables::chams::enemy_only_visible);


				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Enemy Wireframe", &variables::chams::enemy_wireframe);


				imspaceMacro(10, 10);
				if (ImGui::BeginCombo("##enemy-material", variables::chams::materials[variables::chams::enemy_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < variables::chams::materials.size(); n++) 
					{
						bool is_selected = (variables::chams::enemy_chams_material == n); // You can store your selection however you want, outside or inside your objects
						if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
						{
							variables::chams::enemy_chams_material = n;
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			imspaceMacro(0, 15);
			ImGui::Separator();



			imspaceMacro(10, 15);
			ImGui::CheckboxTypeB("Team Chams", &variables::chams::team_chams);

			if (variables::chams::team_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Team Only Visible", &variables::chams::team_only_visible);


				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Team Wireframe", &variables::chams::team_wireframe);
				imspaceMacro(10, 10);

				if (ImGui::BeginCombo("##team-material", variables::chams::materials[variables::chams::team_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < variables::chams::materials.size(); n++)
					{
						bool is_selected = (variables::chams::team_chams_material == n); // You can store your selection however you want, outside or inside your objects
						if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
						{
							variables::chams::team_chams_material = n;
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			imspaceMacro(0, 15);
			ImGui::Separator();


			imspaceMacro(10, 15);
			ImGui::CheckboxTypeB("Local Chams", &variables::chams::local_chams);

			if (variables::chams::local_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Local Only Visible", &variables::chams::local_only_visible);

				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Local Wireframe", &variables::chams::local_wireframe);


				imspaceMacro(10, 10);

				if (ImGui::BeginCombo("##local-material", variables::chams::materials[variables::chams::local_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < variables::chams::materials.size(); n++)
					{
						bool is_selected = (variables::chams::local_chams_material == n); // You can store your selection however you want, outside or inside your objects
						if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
						{
							variables::chams::local_chams_material = n;
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			imspaceMacro(0, 15);
			ImGui::Separator();

			imspaceMacro(10, 15);
			ImGui::CheckboxTypeB("Weapon Chams", &variables::chams::weapon_chams);
			if (variables::chams::weapon_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Weapon Wireframe", &variables::chams::weapon_wireframe);

				{
					imspaceMacro(10, 10);

					if (ImGui::BeginCombo("##weapon-material", variables::chams::materials[variables::chams::weapon_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < variables::chams::materials.size(); n++)
						{
							bool is_selected = (variables::chams::weapon_chams_material == n); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
							{
								variables::chams::weapon_chams_material = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
				}

			}


			imspaceMacro(0, 15);
			ImGui::Separator();


			imspaceMacro(10, 15);
			ImGui::CheckboxTypeB("Arm Chams", &variables::chams::arm_chams);
			if (variables::chams::arm_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Arm Wireframe", &variables::chams::arm_wireframe);

				{
					imspaceMacro(10, 10);

					if (ImGui::BeginCombo("##arm-material", variables::chams::materials[variables::chams::arm_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < variables::chams::materials.size(); n++)
						{
							bool is_selected = (variables::chams::arm_chams_material == n); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
							{
								variables::chams::arm_chams_material = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
				}

			}


			imspaceMacro(0, 15);
			ImGui::Separator();


			imspaceMacro(10, 15);

			ImGui::CheckboxTypeB("Sleeve Chams", &variables::chams::sleeve_chams);

			if (variables::chams::sleeve_chams)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Sleeve Wireframe", &variables::chams::sleeve_wireframe);

				{
					imspaceMacro(10, 10);

					if (ImGui::BeginCombo("##sleeve-material", variables::chams::materials[variables::chams::sleeve_chams_material].c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < variables::chams::materials.size(); n++)
						{
							bool is_selected = (variables::chams::sleeve_chams_material == n); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(variables::chams::materials[n].c_str(), is_selected))
							{
								variables::chams::sleeve_chams_material = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
				}

			}



			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);

			}
			ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#ChamsMenu-state-right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false);
		{
			if (variables::chams::general_chams_enabled)
			{
				if (variables::chams::enemy_chams)
				{

					imspaceMacro(10, 10);
					ImGui::Text("Enemy Chams Visible Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##enemy_chams_visible", variables::chams::player_colors_chams[ENEMY][VICIBLE]);

					if (!variables::chams::enemy_only_visible)
					{
						imspaceMacro(10, 10);
						ImGui::Text("Enemy Chams Hidden Color");
						imspaceMacro(10, 2);
						ImGui::ColorEdit4("##enemy_chams_invisible", variables::chams::player_colors_chams[ENEMY][INVICIBLE]);
					}
				}

				imspaceMacro(0, 15);

				if (variables::chams::team_chams)
				{
					ImGui::Separator();


					imspaceMacro(10, 15);
					ImGui::Text("Team Chams Visible Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##team_chams_visible", variables::chams::player_colors_chams[TEAM][VICIBLE]);

					if (!variables::chams::team_only_visible)
					{
						imspaceMacro(10, 10);
						ImGui::Text("Team Chams Hidden Color");
						imspaceMacro(10, 2);
						ImGui::ColorEdit4("##team_chams_invisible", variables::chams::player_colors_chams[TEAM][INVICIBLE]);
					}

				}

				imspaceMacro(0, 15);

				if (variables::chams::local_chams)
				{
					ImGui::Separator();


					imspaceMacro(10, 15);
					ImGui::Text("Local Chams Visible Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##local_chams_visible", variables::chams::player_colors_chams[LOCAL][VICIBLE]);

					if (!variables::chams::team_only_visible)
					{
						imspaceMacro(10, 10);
						ImGui::Text("Local Chams Hidden Color");
						imspaceMacro(10, 2);
						ImGui::ColorEdit4("##local_chams_invisible", variables::chams::player_colors_chams[LOCAL][INVICIBLE]);
					}

				}

				imspaceMacro(0, 15);

				if (variables::chams::weapon_chams)
				{
					ImGui::Separator();


					imspaceMacro(10, 15);
					ImGui::Text("Weapon Chams Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##weapon_chams_visible", variables::chams::colors_weapon);
				}

				imspaceMacro(0, 10);


				if (variables::chams::arm_chams)
				{
					ImGui::Separator();


					imspaceMacro(10, 15);
					ImGui::Text("Arm Chams Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##arm_chams_visible", variables::chams::colors_arm);
				}

				imspaceMacro(0, 15);

				if (variables::chams::sleeve_chams)
				{
					ImGui::Separator();


					imspaceMacro(10, 15);
					ImGui::Text("Sleeve Chams Color");
					imspaceMacro(10, 2);
					ImGui::ColorEdit4("##sleeve_chams_visible", variables::chams::colors_sleeve);
				}
			}

		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}
IDirect3DTexture9* all_skins[36];
void renderSkinsPage(LPDIRECT3DDEVICE9 pDevice) {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));
	
	ImGui::BeginChild("#SkinsMenu", cMenuSize, true);
	{
		static int selectedMenuWeaponId = -1;

		
		if (selectedMenuWeaponId == -1)
		{
			int i = 1;
			for (auto item : variables::Skin_Changer::menuSkinsItemDefinsV2)
			{
				if (all_skins[i - 1] == nullptr || item.weaponSkinConfig.pleaseUpdateMeInMenu)
				{
					all_skins[i - 1] = get_skin_preview(item.weapon_name_id, item.weaponSkinConfig.newSkinName, pDevice);
					item.weaponSkinConfig.pleaseUpdateMeInMenu = false;
				}

				if (i % 3 == 1)
				{
					imspaceMacro(10, 10);
				}
				else {
					imspaceMacro(10, 0);
				}

				if (ImGui::ImageButton(all_skins[i - 1], ImVec2((cMenuSize.x / 3) - (10 * 2), 100 * 1)))
				{
					selectedMenuWeaponId = item.menuId;
				}

				if (i > 1 && i % 3 == 0)
				{

				}
				else {
					ImGui::SameLine();
				}

				i++;
			}
		}
		else {

			if (ImGui::Button("Back", ImVec2(150, 30)))
			{
				selectedMenuWeaponId = -1;
			}

			//ImGui::InputInt("paint kit", "");

			if (ImGui::Button("Update", ImVec2(150, 30)))
			{
				selectedMenuWeaponId = -1;
			}
		}


		
	}
	ImGui::EndChild();

	return;
	ImGui::BeginChild("#SkinsMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#skins-state-left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false);
		{
			 
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Enabled", &variables::Skin_Changer::isEnabledBasement);

			int TsexVal = variables::Skin_Changer::skinlistSelectedWeaponID;
			imspaceMacro(10, 10);
			ImGui::PushItemWidth(cMenuSize.x / 2 - 20);
			{
				customComboBox(variables::Skin_Changer::skinlistSelectedWeaponID, variables::Skin_Changer::menuSkinsItemDefins);
			}
			ImGui::PopItemWidth();


			if (all_skins[2] == nullptr)
				all_skins[2] = get_skin_preview("weapon_ak47", "", pDevice);

			ImGui::ImageButton(all_skins[2], ImVec2(107 * 1, 76 * 1));



			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#skins-state-right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false);
		{
			if (variables::Skin_Changer::skinlistSelectedWeaponID != -1)
			{
				imspaceMacro(10, 10);
				ImGui::CheckboxTypeB("Enabled", &variables::Skin_Changer::isEnabled[variables::Skin_Changer::skinlistSelectedWeaponID]);

				if (variables::Skin_Changer::skinlistSelectedWeaponID == 0)
				{
					imspaceMacro(10, 10);
					ImGui::PushItemWidth(cMenuSize.x / 2 - 20);
					{
						customComboBox(variables::Skin_Changer::selectedKnifeNameID, variables::Skin_Changer::knifeNames);
					}
					ImGui::PopItemWidth();
				}
				else if (variables::Skin_Changer::skinlistSelectedWeaponID == 1)
				{
					/*imspaceMacro(10, 10);
					ImGui::PushItemWidth(cMenuSize.x / 2 - 20);
					{
						customComboBox(variables::Skin_Changer::selectedKnifeNameID, variables::Skin_Changer::knifeNames);
					}
					ImGui::PopItemWidth();*/

					imspaceMacro(10, 10);
					ImGui::Text("Wait next update :)");
				}

				imspaceMacro(10, 10);
				ImGui::InputInt("Paint Kit", &variables::Skin_Changer::newPaintKit[variables::Skin_Changer::skinlistSelectedWeaponID]);
				imspaceMacro(10, 10);
				ImGui::InputInt("Seed", &variables::Skin_Changer::newSeed[variables::Skin_Changer::skinlistSelectedWeaponID]);
				imspaceMacro(10, 10);
				ImGui::InputInt("Stat Track", &variables::Skin_Changer::newStatTrak[variables::Skin_Changer::skinlistSelectedWeaponID]);
				imspaceMacro(10, 10);
				ImGui::InputInt("Quality", &variables::Skin_Changer::newQuality[variables::Skin_Changer::skinlistSelectedWeaponID]);
				imspaceMacro(10, 10);
				ImGui::InputFloat("Wear", &variables::Skin_Changer::newWear[variables::Skin_Changer::skinlistSelectedWeaponID]);
				//imspaceMacro(10, 10);
				//ImGui::InputText("Custom Name", (char*)&variables::Skin_Changer::newCustomName, strlen(variables::Skin_Changer::newCustomName[variables::Skin_Changer::skinlistSelectedWeaponID]));

				imspaceMacro(10, 10);
				if (ImGui::Button("SAVE & UPDATE", ImVec2(150, 35)))
				{
					globals::forcing_update = true;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void renderMiscPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));
	ImGuiStyle& imguiStyles2 = ImGui::GetStyle();
	ImGui::BeginChild("#MiscMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#misc-state-left", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			
			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Thirdperson", &variables::misc::thirdperson);
			imspaceMacro(10, 10);
			ImGui::SliderFloat("Aimbot Fov", &variables::misc::thirdperson_dist, 30, 250.f, "%.1f");


			imspaceMacro(10, 10);
			ImGui::CheckboxTypeB("Bunny Hop", &variables::misc::bhop);


			ImVec2 nPos = {};

			getMenuPos(nPos);

			ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + 5), ImVec2(nPos.x + (cMenuSize.x / 2) - 2, nPos.y + cMenuSize.y - 10), ImColor(150, 150, 150), 2);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("#misc-state-right", ImVec2(cMenuSize.x / 2, cMenuSize.y), false); {
			imspaceMacro(10, 10);
			ImGui::Text("The Alpha Feature");
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void renderConfigsPage() {
	ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 - 8), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60));
	ImGuiStyle& imguiStyles2 = ImGui::GetStyle();
	ImGui::BeginChild("#SettingsMenu", cMenuSize, false);
	{
		ImGui::BeginChild("#settings_state_left", ImVec2(cMenuSize.x, cMenuSize.y), false);
		{
			for (auto item : config::configsList)
			{
				std::string configName = item.configName;
				std::string dateText = item.date;
				std::string authorName = (std::string("By ") + std::string(item.author));

				ImVec2 configNameSize = ImGui::CalcTextSize(configName.c_str());
				ImVec2 dateTextSize = ImGui::CalcTextSize(dateText.c_str());
				ImVec2 authorNameSize = ImGui::CalcTextSize(authorName.c_str());

				imspaceMacro(5, 5);

				ImGui::BeginChild(std::string(std::string("#cfg_item") + std::to_string(item.configId)).c_str(), ImVec2(cMenuSize.x - 10, 75), false);
				{
					ImGui::BeginChild(std::string(std::string("#cfg_item_text_side") + std::to_string(item.configId)).c_str(), ImVec2(configNameSize.x + 20 + authorNameSize.x + 20, 60), false);
					{
						ImGui::BeginChild(std::string(std::string("#cfg_item_text_inside") + std::to_string(item.configId)).c_str(), ImVec2(configNameSize.x + 20 + authorNameSize.x + 20 + 10, 40), false);
						{
							imspaceMacro(10, 60 / 2 - configNameSize.y / 2);
							ImGui::Text(configName.c_str());
						}
						ImGui::EndChild();

						imspaceMacro(10, 0);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							ImGui::Text(authorName.c_str());

							ImGui::SameLine();

							imspaceMacro(1, 0);
							ImGui::Text(" - ");

							ImGui::SameLine();

							imspaceMacro(1, 0);
							ImGui::Text(dateText.c_str());
						}
						ImGui::PopFont();
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImVec2 rsSize = ImVec2((cMenuSize.x - 10) - (configNameSize.x + 20 + authorNameSize.x + 20), 60);

					ImGui::BeginChild(std::string(std::string("#cfg-item-right-side") + std::to_string(item.configId)).c_str(), ImVec2(rsSize), false);
					{
						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							if (!mSocket::cfg::waiting_response)
							{
								imspaceMacro(rsSize.x - 146 - 5, 75 / 2 - (30 / 2));
								if (ImGui::Button("Save", ImVec2(60, 30)))
								{
									config::saveConfig(item);
								}

								ImGui::SameLine();
								imspaceMacro(10, 0);

								if (ImGui::Button("Load", ImVec2(60, 30)))
								{
									config::loadConfigFromServer(item);
								}
							}
						}
						ImGui::PopFont();
					}
					ImGui::EndChild();
					imspaceMacro(0, 14);

					ImGui::Separator();

				}
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}


void renderConnectingToServer(LPDIRECT3DDEVICE9 pDevice)
{
	

	ImGuiStyle& imguiStyles = ImGui::GetStyle();

	float FirstRadius = imguiStyles.WindowRounding;

	windowSize wsz;

	GetWindowSize(wsz, pDevice);

	std::string loadString = "";

	if (!mSocket::cfg::authed && mSocket::cfg::socketIsConnected)
		loadString = "Waiting authentication...";
	else if (variables::NetworkUser::fuckThisCheat)
		loadString = "Wtf are you doing?";
	else
		loadString = "Connecting to server";

	ImVec2 cTSize = ImGui::CalcTextSize(loadString.c_str());
	ImVec2 cWSize = ImVec2(0, 0);

	cWSize.x = 40 + cTSize.x + 10;
	cWSize.y = 40;

	imguiStyles.WindowRounding = 5.f;


	ImGui::SetNextWindowPos(ImVec2(wsz.width - cWSize.x - 10, 10));
	ImGui::SetNextWindowSize(cWSize);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 255.f);
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));
			{
				ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1, 100, 50, 25));
				{
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(35.0f / 255.0f, 160.0f / 255.0f, 30.0f / 255.0f, 1.0f));
					{
						ImGui::Begin("Loader :)", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
						{
							imspaceMacro(10, 10 - (3 / 2));
							Spinner("#19954", 10, 3, ImColor(35, 160, 30));

							ImGui::SameLine();

							imspaceMacro(10, 0);
							ImGui::Text(loadString.c_str());
						}
						ImGui::End();
					}
					ImGui::PopStyleColor();
				}
				ImGui::PopStyleColor();
			}
			ImGui::PopStyleColor();
		}
		ImGui::PopStyleVar();
	}

	imguiStyles.WindowRounding = FirstRadius;
}

char dcbText[250] = {};
#define SNOW_LIMIT 60
std::vector<Snowflake::Snowflake> snow;
#endif






void CheckboxMy(ImVec2 ts, bool* v, const char* name, const char* name_id = "") {
	ImGui::BeginGroup();
	{
		ToggleButton(name, v);
	}
	ImGui::EndGroup();
}

void renderColorableButton(const char* name, float* color) {
	float ds = variables::Menu_Settings::DpiSize;
	ImVec2 ns = ImGui::CalcTextSize(name);
	ImVec2 ws = ImGui::GetContentRegionMax();
	ImGui::Text(name);
	ImGui::SameLine();
	imspaceMacro((ws.x - ns.x) - (20 * ds) - 10, 0);
	ImGui::ColorEdit4(name, color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
}

void renderAimbotPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;
	
	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	if (!isTop)
	{
		ImGui::BeginChild("aimbot-papge-basement", ImVec2(-1, -1), false);
		{

			imspaceMacro(0, 10 * ds);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Main");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subl-main", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
					{
						imspaceMacro(0, 10 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::enabled, "Master", "##aimbotmaster");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::autofire, "Auto Fire", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::drawFov, "Draw Fov", "##aimbotdrawfov");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::teamCheck, "Team Check", "##aimbotteamcheck");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::silentAim, "Silent Aim", "##aimbotsilentaim");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::auto_wall, "Wall Penetration", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::auto_revolver, "Auto Revolver", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						ImGui::SliderFloat("Field Of View", &variables::Aimbot_Settings::aimbot_fov, 0, 180);

						ImGui::PopFont();
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();












				imspaceMacro(10 * ds, 20 * ds);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Exploits");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subl-exploits", ImVec2(bs_lift_size_x - 20 * ds, 200 * ds), true);
					{
						static bool doubleTapEnabled = false;
						static bool hideshotTapEnabled = false;
						static bool forceDtTeleportEnabled = false;

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &doubleTapEnabled, "Double Tap Enabled", "##aimbotexploitsdoubletapenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &hideshotTapEnabled, "Hide Shots Enabled", "##aimbotexploitshideshotsenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &forceDtTeleportEnabled, "Force Teleport", "##aimbotexploitsforceteleportenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::backtrack, "Backtrack Enabled", "##aimbotexploitsbacktrackenabled");

						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Backtrack Tick", &variables::misc::backtrack_ticks, 0, 200);

						ImGui::PopFont();
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();


			ImGui::SameLine();
			
			
			imspaceMacro(10 * ds, 0);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Weapon");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subr-weapon", ImVec2(bs_lift_size_x - 10 * ds, 220 * ds), true);
					{
						imspaceMacro(0, 10 * ds);
						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						ImGui::SliderFloat("Hitchance", &variables::Aimbot_Settings::aimbot_hitchance, 0, 160);
						
						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Min damage", &variables::Aimbot_Settings::min_damage, 1, 160);

						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Smoothness", &variables::Aimbot_Settings::aimbot_smoothing, .01, 1);

						ImGui::PopFont();


						{
							imspaceMacro(0, 20 * ds);
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
							{
								static const char* pw = "Target Hitboxes";
								ImGui::PushItemWidth(-1);
								if (ImGui::BeginCombo("##aimbot_target_hitboxes", pw, ImGuiComboFlags_NoArrowButton))
								{
									ImGui::PushItemWidth(-1);
									ImGui::ListBoxHeader("#aimbot_hitboxes_liswet", 4, 6);
									{
										for (size_t i = 0; i < IM_ARRAYSIZE(variables::Aimbot_Settings::selected_hitboxes); i++)
										{
											bool* isSelected = &variables::Aimbot_Settings::selected_hitboxes[i];

											if (ImGui::Selectable(variables::Aimbot_Settings::selected_hitboxes_names[i], *isSelected))
											{
												*isSelected = !*isSelected;
											}
										}
									}
									ImGui::ListBoxFooter();
									ImGui::PopItemWidth();

									ImGui::EndCombo();
								}
								ImGui::PopItemWidth();
							}
							ImGui::PopFont();

							ImGui::PopStyleVar();
							ImGui::PopStyleVar();
						}

					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();

		}
		ImGui::EndChild();

	}
	else {

	}
}
void renderAntiAimPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	ImGui::BeginChild("aimbot-papge-basement", ImVec2(-1, -1), false);
	{
		ImVec2 _ts = ImGui::CalcTextSize("Currently Unavailable");
		imspaceMacro(ts.x / 2 - _ts.x / 2, ts.y / 2 - _ts.y / 2);
		ImGui::Text("Currently Unavailable");
	}
	ImGui::EndChild();
	return;

	if (!isTop)
	{
		ImGui::BeginChild("aimbot-papge-basement", ImVec2(-1, -1), false);
		{

			imspaceMacro(0, 10 * ds);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Main");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subl-main", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
					{
						imspaceMacro(0, 10 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::enabled, "Master", "##aimbotmaster");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::autofire, "Auto Fire", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::drawFov, "Draw Fov", "##aimbotdrawfov");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::teamCheck, "Team Check", "##aimbotteamcheck");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::silentAim, "Silent Aim", "##aimbotsilentaim");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::auto_wall, "Wall Penetration", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Aimbot_Settings::auto_revolver, "Auto Revolver", "##aimbotautofire");

						imspaceMacro(0, 15 * ds);
						ImGui::SliderFloat("Field Of View", &variables::Aimbot_Settings::aimbot_fov, 0, 180);

						ImGui::PopFont();
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();












				imspaceMacro(10 * ds, 20 * ds);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Exploits");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subl-exploits", ImVec2(bs_lift_size_x - 20 * ds, 200 * ds), true);
					{
						static bool doubleTapEnabled = false;
						static bool hideshotTapEnabled = false;
						static bool forceDtTeleportEnabled = false;

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &doubleTapEnabled, "Double Tap Enabled", "##aimbotexploitsdoubletapenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &hideshotTapEnabled, "Hide Shots Enabled", "##aimbotexploitshideshotsenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &forceDtTeleportEnabled, "Force Teleport", "##aimbotexploitsforceteleportenabled");

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::backtrack, "Backtrack Enabled", "##aimbotexploitsbacktrackenabled");

						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Backtrack Tick", &variables::misc::backtrack_ticks, 0, 200);

						ImGui::PopFont();
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();


			ImGui::SameLine();


			imspaceMacro(10 * ds, 0);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Weapon");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("aimbot-papge-basement-subr-weapon", ImVec2(bs_lift_size_x - 10 * ds, 220 * ds), true);
					{
						imspaceMacro(0, 10 * ds);
						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						ImGui::SliderFloat("Hitchance", &variables::Aimbot_Settings::aimbot_hitchance, 0, 160);

						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Min damage", &variables::Aimbot_Settings::min_damage, 1, 160);

						imspaceMacro(0, 20 * ds);
						ImGui::SliderFloat("Smoothness", &variables::Aimbot_Settings::aimbot_smoothing, .01, 1);

						ImGui::PopFont();


						{
							imspaceMacro(0, 20 * ds);
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
							{
								static const char* pw = "Target Hitboxes";
								ImGui::PushItemWidth(-1);
								if (ImGui::BeginCombo("madafaka", pw, ImGuiComboFlags_NoArrowButton))
								{
									ImGui::PushItemWidth(-1);
									ImGui::ListBoxHeader("#aimbot_hitboxes_liswet", 4, 6);
									{
										for (size_t i = 0; i < IM_ARRAYSIZE(variables::Aimbot_Settings::selected_hitboxes); i++)
										{
											bool* isSelected = &variables::Aimbot_Settings::selected_hitboxes[i];

											if (ImGui::Selectable(variables::Aimbot_Settings::selected_hitboxes_names[i], *isSelected))
											{
												*isSelected = !*isSelected;
											}
										}
									}
									ImGui::ListBoxFooter();
									ImGui::PopItemWidth();

									ImGui::EndCombo();
								}
								ImGui::PopItemWidth();
							}
							ImGui::PopFont();

							ImGui::PopStyleVar();
							ImGui::PopStyleVar();
						}

					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();

		}
		ImGui::EndChild();

	}
	else {

	}
}
void renderEspPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);
	
	static int espteamsub = 0;

	ImVec2 ws = ImGui::GetContentRegionMax();

	if (!isTop)
	{
		ImGui::BeginChild("esp-papge-basement", ImVec2(-1, -1), false);
		{
			imspaceMacro(10 * ds, 10 * ds);
			int epbtsbw = (ws.x / 3) - 10 * ds;

			ImGui::BeginGroup();
			{
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.74f, 0.34f, .1));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.64f, 0.83f, 0.34f, .1));
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				
				{
					ImGui::PushStyleColor(ImGuiCol_Button, espteamsub == 0 ? ImVec4(0.64f, 0.83f, 0.34f, .1) : ImVec4(.16, .16, .16, .1));
					if (ImGui::Button("Enemy", ImVec2(epbtsbw, 20))) espteamsub = 0;
					ImGui::PopStyleColor();
				}

				ImGui::SameLine();
				
				{
					ImGui::PushStyleColor(ImGuiCol_Button, espteamsub == 1 ? ImVec4(0.64f, 0.83f, 0.34f, .1) : ImVec4(.16, .16, .16, .1));
					if(ImGui::Button("Team", ImVec2(epbtsbw, 20))) espteamsub = 1;
					ImGui::PopStyleColor();
				}

				ImGui::SameLine();

				{
					ImGui::PushStyleColor(ImGuiCol_Button, espteamsub == 2 ? ImVec4(0.64f, 0.83f, 0.34f, .1) : ImVec4(.16, .16, .16, .1));
						if (ImGui::Button("Self", ImVec2(epbtsbw, 20))) espteamsub = 2;
					ImGui::PopStyleColor();
				}

				ImGui::PopFont();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();

			imspaceMacro(0, 10 * ds);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Esp");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("esp-papge-basement-subl-1", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
					{

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledBase, "Esp Master Switch", "##espmasterenabled");

						imspaceMacro(0, 20 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledBox[espteamsub], "Esp Box Enabled", "##espboxenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledGethers[espteamsub], "Esp Gathers Enabled", "##espgathersenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledHealthesp[espteamsub], "Esp Health Enabled", "##esphealtenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledLine[espteamsub], "Esp Line Enabled", "##espenabledline");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledNameesp[espteamsub], "Esp Name Enabled", "##espnameesp");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Esp_Settings::enabledSkeleton[espteamsub], "Esp Skeleton Enabled", "##espskeletonenabled");

						ImGui::PopFont();

					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();


				imspaceMacro(10 * ds, 15 * ds);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Chams");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("esp-papge-basement-subl-2", ImVec2(bs_lift_size_x - 20 * ds, 586 * ds), true);
					{

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::draw_chams_on_top, "Chams Draw On Top", "##chamsdrawontop");

						imspaceMacro(0, 20 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::chams_enableds[espteamsub], "Chams Enabled", "##chamsenabledsitem");

						imspaceMacro(0, 15 * ds);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							ImGui::Text("Player Material");
							imspaceMacro(0, 5 * ds);
							ImGui::PushItemWidth(-1);
							if (ImGui::BeginCombo("##chamsplayermaterials", variables::Chams_Settings::materials.at((variables::Chams_Settings::chams_materialids[espteamsub])).c_str(), ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
							{
								for (int n = 0; n < variables::Chams_Settings::materials.size(); n++)
								{
									bool is_selected = (variables::Chams_Settings::chams_materialids[espteamsub] == n); // You can store your selection however you want, outside or inside your objects
									if (ImGui::Selectable(variables::Chams_Settings::materials.at(n).c_str(), is_selected))
									{
										variables::Chams_Settings::chams_materialids[espteamsub] = n;
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								ImGui::EndCombo();
								ImGui::PopItemWidth();
							}
						}
						ImGui::PopFont();

						ImGui::PopStyleVar();
						ImGui::PopStyleVar();
						

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::chams_onlyvisibleds[espteamsub], "Chams Only Visible", "##chamsonlyvisibleitem");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::chams_wireframes[espteamsub], "Chams Wireframe", "##chamswireframeitem");




						imspaceMacro(0, 20 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::sleeve_chams, "Chams Sleeve", "##chamssleeve");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::sleeve_wireframe, "Chams Sleeve Wireframe", "##chamssleevewireframe");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::weapon_chams, "Chams Weapon", "##chamsweapon");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::weapon_wireframe, "Chams Weapon Wireframe", "##chamsweaponwireframe");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::arm_chams, "Chams Arms", "##chamsarms");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::Chams_Settings::arm_wireframe, "Chams Arms Wireframe", "##chamsarmswireframe");


						



						imspaceMacro(0, 15 * ds);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							ImGui::Text("Sleeve Material");
							imspaceMacro(0, 5 * ds);
							ImGui::PushItemWidth(-1);
							if (ImGui::BeginCombo("##chamssleevematerials", variables::Chams_Settings::materials.at(variables::Chams_Settings::sleeve_chams_material).c_str(), ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
							{
								for (int n = 0; n < variables::Chams_Settings::materials.size(); n++)
								{
									bool is_selected = (variables::Chams_Settings::sleeve_chams_material == n); // You can store your selection however you want, outside or inside your objects
									if (ImGui::Selectable(variables::Chams_Settings::materials.at(n).c_str(), is_selected))
									{
										variables::Chams_Settings::sleeve_chams_material = n;
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								ImGui::EndCombo();
								ImGui::PopItemWidth();
							}
						}
						ImGui::PopFont();

						ImGui::PopStyleVar();
						ImGui::PopStyleVar();




						imspaceMacro(0, 15 * ds);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							ImGui::Text("Weapon Material");
							imspaceMacro(0, 5 * ds);
							ImGui::PushItemWidth(-1);
							if (ImGui::BeginCombo("##chamsweaponmaterials", variables::Chams_Settings::materials.at(variables::Chams_Settings::weapon_chams_material).c_str(), ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
							{
								for (int n = 0; n < variables::Chams_Settings::materials.size(); n++)
								{
									bool is_selected = (variables::Chams_Settings::weapon_chams_material == n); // You can store your selection however you want, outside or inside your objects
									if (ImGui::Selectable(variables::Chams_Settings::materials.at(n).c_str(), is_selected))
									{
										variables::Chams_Settings::weapon_chams_material = n;
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								ImGui::EndCombo();
								ImGui::PopItemWidth();
							}
						}
						ImGui::PopFont();

						ImGui::PopStyleVar();
						ImGui::PopStyleVar();




						imspaceMacro(0, 15 * ds);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
						{
							ImGui::Text("Arms Material");
							imspaceMacro(0, 5 * ds);
							ImGui::PushItemWidth(-1);
							if (ImGui::BeginCombo("##chamsarmsmaterials", variables::Chams_Settings::materials.at(variables::Chams_Settings::arm_chams_material).c_str(), ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
							{
								for (int n = 0; n < variables::Chams_Settings::materials.size(); n++)
								{
									bool is_selected = (variables::Chams_Settings::arm_chams_material == n); // You can store your selection however you want, outside or inside your objects
									if (ImGui::Selectable(variables::Chams_Settings::materials.at(n).c_str(), is_selected))
									{
										variables::Chams_Settings::arm_chams_material = n;
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								ImGui::EndCombo();
								ImGui::PopItemWidth();
							}
						}
						ImGui::PopFont();

						ImGui::PopStyleVar();
						ImGui::PopStyleVar();



						ImGui::PopFont();

					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();


			ImGui::SameLine();


			imspaceMacro(10 * ds, 0);
			ImGui::BeginGroup();
			{
				{
					imspaceMacro(10 * ds, 0);
					ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
					ImGui::Text("Esp Colors");
					ImGui::PopFont();


					imspaceMacro(10 * ds, 10 * ds);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
					ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
					{
						ImGui::BeginChild("esp-papge-basement-subr-1", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
						{

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Box Enabled", variables::Esp_Settings::colors_box[espteamsub]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Gathers Enabled", variables::Esp_Settings::colors_info[espteamsub]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Health Enabled", variables::Esp_Settings::colors_health[espteamsub]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Line Enabled", variables::Esp_Settings::colors_line[espteamsub]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Name Enabled", variables::Esp_Settings::colors_name[espteamsub]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Esp Skeleton Enabled", variables::Esp_Settings::colors_skeleton[espteamsub]);

							ImGui::PopFont();

						}
						ImGui::EndChild();
					}
					ImGui::PopStyleVar();
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
				} 

				{
					imspaceMacro(10 * ds, 15);
					ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
					ImGui::Text("Chams Colors");
					ImGui::PopFont();


					imspaceMacro(10 * ds, 10 * ds);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
					ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
					{
						ImGui::BeginChild("esp-papge-basement-subr-2", ImVec2(bs_lift_size_x - 20 * ds, 370 * ds), true);
						{

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Chams In Visible", variables::Chams_Settings::player_colors_chams[espteamsub][ens::VICIBLE]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Chams In InVisible", variables::Chams_Settings::player_colors_chams[espteamsub][ens::INVICIBLE]);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Chams In Weapons", variables::Chams_Settings::colors_weapon);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Chams In Arms", variables::Chams_Settings::colors_arm);

							imspaceMacro(0, 15 * ds);
							renderColorableButton("Chams In Sleeves", variables::Chams_Settings::colors_sleeve);

							ImGui::PopFont();

						}
						ImGui::EndChild();
					}
					ImGui::PopStyleVar();
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
				}
			}
			ImGui::EndGroup();

		}
		ImGui::EndChild();

	}
	else {

	}
}
void renderInventoryPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	ImGui::BeginChild("inventory-papge-basement", ImVec2(-1, -1), false);
	{
		ImVec2 _ts = ImGui::CalcTextSize("Temporary Unavailable");
		imspaceMacro(ts.x / 2 - _ts.x / 2, ts.y / 2 - _ts.y / 2);
		ImGui::Text("Temporary Unavailable");
	}
	ImGui::EndChild();
	return;

	if (!isTop)
	{

	}
	else {

	}
}
void renderMiscPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	static int espteamsub = 0;

	ImVec2 ws = ImGui::GetContentRegionMax();

	if (!isTop)
	{
		ImGui::BeginChild("misc-papge-basement", ImVec2(-1, -1), false);
		{

			imspaceMacro(0, 10 * ds);
			ImGui::BeginGroup();
			{
				imspaceMacro(10 * ds, 0);
				ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
				ImGui::Text("Main");
				ImGui::PopFont();


				imspaceMacro(10 * ds, 10 * ds);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
				{
					ImGui::BeginChild("miscmain-papge-basement-subl-1", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
					{

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

						imspaceMacro(0, 10 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::bhop, "Bunny Hop", "##miscmasterenabled");


						{
							imspaceMacro(0, 20 * ds);
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
							{
								static const char* pw = "Auto Strafe Options";
								ImGui::PushItemWidth(-1);
								if (ImGui::BeginCombo("Auto Strafe Options", pw, ImGuiComboFlags_NoArrowButton))
								{
									for (size_t i = 0; i < variables::misc::autostrafe_options.size(); i++)
									{
										bool isSelected = variables::misc::autostrafe_target == i;

										if (ImGui::Selectable(variables::misc::autostrafe_options.at(i).c_str(), isSelected))
										{
											//pw = std::string(std::string("Auto Strafe Options: ") + std::string(variables::misc::autostrafe_options.at(i).c_str())).c_str();
											variables::misc::autostrafe_target = i;
										}
									}

									ImGui::EndCombo();
								}
								ImGui::PopItemWidth();
							}
							ImGui::PopFont();

							ImGui::PopStyleVar();
							ImGui::PopStyleVar();
						}

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::clean_screenshots, "Screenshot Clean", "##miscscreenshotcleanenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::infinite_duck, "Infinite Duck", "##miscmasterenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::reveal_ranks, "Reveal Ranks", "##miscmasterenabled");

						imspaceMacro(0, 15 * ds);
						CheckboxMy(ImVec2(0, 0), &variables::misc::slowwalk, "Slow Walk", "##miscmasterenabled");

						ImGui::PopFont();

					}
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();


			ImGui::SameLine();


			imspaceMacro(10 * ds, 0);
			ImGui::BeginGroup();
			{
				{
					imspaceMacro(10 * ds, 0);
					ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_10_font);
					ImGui::Text("Visuals");
					ImGui::PopFont();


					imspaceMacro(10 * ds, 10 * ds);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.16, .16, .16, .16));
					ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6 * ds);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * ds, 10 * ds));
					{
						ImGui::BeginChild("miscmain-papge-basement-subr-visuals", ImVec2(bs_lift_size_x - 20 * ds, 285 * ds), true);
						{

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);

							imspaceMacro(0, 15 * ds);
							ImGui::SliderFloat("Flash Alpha", &variables::misc_visuals::noflash_alpha, 0.f, 255.f);

							ImGui::PopFont();

						}
						ImGui::EndChild();
					}
					ImGui::PopStyleVar();
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
				}
			}
			ImGui::EndGroup();

		}
		ImGui::EndChild();

	}
	else {

	}
}
void renderWorldPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	ImGui::BeginChild("inventory-papge-basement", ImVec2(-1, -1), false);
	{
		ImVec2 _ts = ImGui::CalcTextSize("Temporary Unavailable");
		imspaceMacro(ts.x / 2 - _ts.x / 2, ts.y / 2 - _ts.y / 2);
		ImGui::Text("Temporary Unavailable");
	}
	ImGui::EndChild();
	return;

	if (!isTop)
	{

	}
	else {

	}
}
void renderConfigsPage(ImVec2 ts, float mpw, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);
	static bool createNewConfigState = false; 

	static bool targetting = false;
	static int opState = 0;

	static float aSpeed = 3;
	 
	static float previewAlpha = 255.f;

	ImGuiIO& io = ImGui::GetIO();

	if (targetting != createNewConfigState)
	{
		if (opState == 0 && previewAlpha > 0)
		{
			previewAlpha -= 3 * io.DeltaTime * aSpeed;
		}
		
		
		if (opState == 1 && previewAlpha < 1.f)
		{
			previewAlpha += 3 * io.DeltaTime * aSpeed;
		}




		if (opState == 0 && previewAlpha <= 0)
		{
			opState = 1;
			previewAlpha = 0;
			targetting = createNewConfigState;
		}

		if (opState == 1 && previewAlpha >= 1.f)
		{
			opState = 0;
			previewAlpha = 1;
		}
	}
	else {
		if (previewAlpha < 1.f)
		{
			previewAlpha += 3 * io.DeltaTime * aSpeed;
		}

		if (previewAlpha > 1.f)
		{
			opState = 0;
			previewAlpha = 1.f;
		}
	}

	 

	if (!isTop)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, mpw == 1.f ? previewAlpha : mpw);
		{



			if (targetting)
			{
				static char newConfigName[20] = "";
				ImGui::BeginChild("#settings_state_create", ImVec2(-1, -1), false);
				{
					ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
					{
						ImVec2 imvec2 = ImGui::CalcTextSize("x");
						imspaceMacro(ImGui::GetWindowWidth() - imvec2.x - 20 * ds, 10 * ds);
						ImGui::Text("x");

						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							createNewConfigState = !createNewConfigState;
						}
						imspaceMacro(ImGui::GetWindowWidth() / 2 - (250 * ds) / 2, (ImGui::GetWindowHeight() / 2 - (250 * ds) / 2) - imvec2.x);
						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
						{
							ImGui::BeginChild("#settings_state_create_layout", ImVec2(250 * ds, 130 * ds), false);
							{
								ImGui::PushItemWidth((250 - 20) * ds);
								{
									ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8 * ds);
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10 * ds, 5 * ds));

									ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, .3));
									ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.3f, 0.2f, .3));
									ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.2f, 0.4f, 0.2f, .3));

									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, .3));
									ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, .3));
									ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.9f, 0.3f, .3));

									{
										imspaceMacro(10 * ds, 10 * ds);
										ImGui::Text("Config Name");
										imspaceMacro(10 * ds, 10 * ds);
										ImGui::InputText("##ncfgname", newConfigName, sizeof(newConfigName));
										imspaceMacro(250 * ds - 80 - 10 * ds, 10 * ds);
										if (ImGui::Button("Create", ImVec2(80 * ds, 35 * ds)))
										{
											config::createConfig(newConfigName);
											config::refreshConfigs(false);
											variables::Menu_Settings::addNotification(-1, "Success", "Config has been created");
											createNewConfigState = !createNewConfigState;

											strcpy(newConfigName, "");
										}
									}

									ImGui::PopStyleColor();
									ImGui::PopStyleColor();
									ImGui::PopStyleColor();

									ImGui::PopStyleColor();
									ImGui::PopStyleColor();
									ImGui::PopStyleColor();

									ImGui::PopStyleVar();
									ImGui::PopStyleVar();
								}
								ImGui::PopItemWidth();
							}
							ImGui::EndChild();
						}
						ImGui::PopFont();
					}
					ImGui::PopFont();
				}
				ImGui::EndChild();
			}
			else {
				ImGui::BeginChild("#settings_state_basement", ImVec2(ts.x, ts.y), false);
				{
					ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
					{
						ImVec2 imvec2 = ImGui::CalcTextSize("new");
						imspaceMacro(ImGui::GetWindowWidth() - imvec2.x - 20 * ds, 10 * ds);
						ImGui::Text("new");
					}
					ImGui::PopFont();
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						createNewConfigState = !createNewConfigState;

					}

					const char* pleasedeleteme = "";

					imspaceMacro(0, -10 * ds);
					for (auto item : config::configsList)
					{
						std::string configName = item.configName;
						std::string dateText = item.date;
						std::string authorName = (std::string("By ") + std::string(item.author));

						ImVec2 configNameSize = ImGui::CalcTextSize(configName.c_str());
						ImVec2 dateTextSize = ImGui::CalcTextSize(dateText.c_str());
						ImVec2 authorNameSize = ImGui::CalcTextSize(authorName.c_str());

						imspaceMacro(20 * ds, 20 * ds);

						ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22, 0.22, 0.22, .2));
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8);


						ImGui::BeginChild(std::string(std::string("#cfg_item") + std::to_string(item.configId)).c_str(), ImVec2(ts.x - 40 * ds, 75 * ds), true);
						{
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22, 0.22, 0.22, 0));

							ImGui::BeginChild(std::string(std::string("#cfg_item_text_side") + std::to_string(item.configId)).c_str(), ImVec2(configNameSize.x + 20 + authorNameSize.x + 20, 60), false);
							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_18_font);
								{
									ImGui::BeginChild(std::string(std::string("#cfg_item_text_inside") + std::to_string(item.configId)).c_str(), ImVec2(configNameSize.x + 20 + authorNameSize.x + 20 + 10, 40), false);
									{
										imspaceMacro(10, 60 / 2 - configNameSize.y / 2);
										ImGui::Text(configName.c_str());
									}
									ImGui::EndChild();
								}
								ImGui::PopFont();

								imspaceMacro(10, 0);

								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									ImGui::Text(authorName.c_str());

									ImGui::SameLine();

									imspaceMacro(1, 0);
									ImGui::Text(" - ");

									ImGui::SameLine();

									imspaceMacro(1, 0);
									ImGui::Text(dateText.c_str());
								}
								ImGui::PopFont();
							}
							ImGui::EndChild();

							ImGui::SameLine();

							ImVec2 rsSize = ImVec2((ts.x - 20 * ds) - (configNameSize.x + 20 + authorNameSize.x + 20), 60);

							ImGui::BeginChild(std::string(std::string("#cfg-item-right-side") + std::to_string(item.configId)).c_str(), ImVec2(rsSize), false);
							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									if (!mSocket::cfg::waiting_response)
									{
										ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8);
										ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, .3));
										ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, .3));
										ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.9f, 0.3f, .3));



										ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
										{
											if (config::LoadedConfig.configName == item.configName)
											{
												ImVec2 imvec2 = ImGui::CalcTextSize("new");

												imspaceMacro((rsSize.x - 86 * ds - 5 * ds) - (imvec2.x + 30 * ds), 75 * ds / 2 - (30 * ds / 2));

												ImGui::BeginGroup();

												ImGui::BeginGroup();
												imspaceMacro(10 * ds, 30 * ds / 2 - imvec2.y / 2);
												ImGui::Text("delete");

												if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
												{
													config::deleteConfig(item.configName.c_str());
												}

												ImGui::EndGroup();

												ImGui::SameLine();

												ImGui::BeginGroup();
												imspaceMacro(10 * ds, 0);
												ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
												{
													ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.9f, 0.3f, .3));
													if (ImGui::Button("Save", ImVec2(60 * ds, 30 * ds)))
													{
														config::saveConfig(item);
													}
													ImGui::PopStyleColor();
												}
												ImGui::PopFont();

												ImGui::EndGroup();
												ImGui::EndGroup();
											}
											else {
												ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
												{
													ImVec2 imvec2 = ImGui::CalcTextSize("new");

													imspaceMacro((rsSize.x - 86 * ds - 5 * ds) - (imvec2.x + 30 * ds), 75 * ds / 2 - (30 * ds / 2));

													ImGui::BeginGroup();

													ImGui::BeginGroup();
													imspaceMacro(10 * ds, 30 * ds / 2 - imvec2.y / 2);
													ImGui::Text("delete");

													if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
													{
														config::deleteConfig(item.configName.c_str());
													}

													ImGui::EndGroup();

													ImGui::SameLine();

													ImGui::BeginGroup();
													imspaceMacro(10 * ds, 0);
													ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
													{
														if (ImGui::Button("Load", ImVec2(60 * ds, 30 * ds)))
														{
															config::loadConfigFromServer(item);

															console::log(std::string(config::LoadedConfig.configName).c_str());
															console::log(std::string(item.configName).c_str());
														}
													}
													ImGui::PopFont();

													ImGui::EndGroup();
													ImGui::EndGroup();
												}
												ImGui::PopFont();
											}

										}
										ImGui::PopFont();


										//ImGui::SameLine();
										//imspaceMacro(10, 0);

										ImGui::PopStyleColor();
										ImGui::PopStyleColor();
										ImGui::PopStyleColor();
										ImGui::PopStyleVar();
									}
								}
								ImGui::PopFont();
							}
							ImGui::EndChild();
							ImGui::PopStyleColor();

							//ImGui::Separator();

						}
						ImGui::EndChild();

						ImGui::PopStyleVar();
						ImGui::PopStyleColor();
					}

					config::refreshConfigs(false);
				}
				ImGui::EndChild();
			}

		}
		ImGui::PopStyleVar();
	}
	else {

	}
}
void renderScriptsPage(ImVec2 ts, bool isTop = false) {
	float ds = variables::Menu_Settings::DpiSize;

	float bs_lift_size_x = (ts.x / 2) - (10 * ds);

	ImGui::BeginChild("inventory-papge-basement", ImVec2(-1, -1), false);
	{
		ImVec2 _ts = ImGui::CalcTextSize("Temporary Unavailable");
		imspaceMacro(ts.x / 2 - _ts.x / 2, ts.y / 2 - _ts.y / 2);
		ImGui::Text("Temporary Unavailable");
	}
	ImGui::EndChild();
	return;

	if (!isTop)
	{

	}
	else {

	}
}
void renderConnectingToServer(LPDIRECT3DDEVICE9 pDevice)
{

	ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
	{
		ImGuiStyle& imguiStyles = ImGui::GetStyle();

		float FirstRadius = imguiStyles.WindowRounding;

		windowSize wsz;

		GetWindowSizeX(wsz, pDevice);

		std::string loadString = "";

		if (!mSocket::cfg::authed && mSocket::cfg::socketIsConnected)
			loadString = "Waiting authentication...";
		else if (variables::NetworkUser::fuckThisCheat)
			loadString = "Wtf are you doing?";
		else
			loadString = "Connecting to server";

		ImVec2 cTSize = ImGui::CalcTextSize(loadString.c_str());
		ImVec2 cWSize = ImVec2(0, 0);

		cWSize.x = 32 + cTSize.x + 10;
		cWSize.y = 20 + cTSize.y;

		imguiStyles.WindowRounding = 5.f;


		ImGui::SetNextWindowPos(ImVec2(wsz.width - cWSize.x - 10, 10));
		ImGui::SetNextWindowSize(cWSize);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, .9);
			{

				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.08f, .08f, .08f, .6f));
				{
					ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1, 100, 50, 25));
					{
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f));
						{
							ImGui::Begin("Loader :)", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
							{
								ImGui::BeginGroup();
								
								imspaceMacro(10, (cWSize.y / 2) - 6 - (2 / 2));
								Spinner("#connectingtoserverspinnerbuttonxd", 6, 2, ImColor(35, 160, 30));

								ImGui::EndGroup();

								
								
								ImGui::SameLine();



								ImGui::BeginGroup();

								imspaceMacro(10, (cWSize.y / 2) - (cTSize.y / 1.6));
								ImGui::Text(loadString.c_str());

								ImGui::EndGroup();
							}
							ImGui::End();
						}
						ImGui::PopStyleColor();
					}
					ImGui::PopStyleColor();
				}
				ImGui::PopStyleColor();

			}
			ImGui::PopStyleVar();
		}
		imguiStyles.WindowRounding = FirstRadius;

	}
	ImGui::PopFont();

}

void iXmenu::renderImguiBasedMenu(LPDIRECT3DDEVICE9 pDevice, bool isActive) {
	static int initializingMode = true;
	ImGui::StyleColors_ProDark();
	setOurCustomImguiColorsAndEtc(pDevice);
	

#ifndef _DEBUG
	if (initializingMode)
	{
		ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
		{
			static float loadPercent = 3;
			ImGuiStyle& imguiStyles = ImGui::GetStyle();

			float FirstRadius = imguiStyles.WindowRounding;

			windowSize wsz;

			GetWindowSizeX(wsz, pDevice);

			std::string loadString = std::string(std::string("init process 100/") + std::to_string((int)loadPercent));

			ImVec2 cTSize = ImGui::CalcTextSize(loadString.c_str());
			ImVec2 cWSize = ImVec2(0, 0);

			cWSize.x = 32 + cTSize.x + 10;
			cWSize.y = 20 + cTSize.y;

			imguiStyles.WindowRounding = 5.f;


			ImGui::SetNextWindowPos(ImVec2(wsz.width - cWSize.x - 10, 10));
			ImGui::SetNextWindowSize(cWSize);
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, .9);
				{

					ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.08f, .08f, .08f, .6f));
					{
						ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1, 100, 50, 25));
						{
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f));
							{
								ImGui::Begin("##proimityinitializer", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
								{
									ImGui::BeginGroup();

									imspaceMacro(10, (cWSize.y / 2) - 6 - (2 / 2));
									Spinner("#proimityinitializerspinnerxd", 6, 2, ImColor(35, 160, 30));

									ImGui::EndGroup();



									ImGui::SameLine();



									ImGui::BeginGroup();

									imspaceMacro(10, (cWSize.y / 2) - (cTSize.y / 1.6));
									ImGui::Text(loadString.c_str());

									ImGui::EndGroup();
								}
								ImGui::End();
							}
							ImGui::PopStyleColor();
						}
						ImGui::PopStyleColor();
					}
					ImGui::PopStyleColor();

				}
				ImGui::PopStyleVar();
			}

			loadPercent += ImGui::GetIO().DeltaTime * (1 * (float(rand()) / float((RAND_MAX)) * 30));

			if (loadPercent >= 100)
			{
				loadPercent = 100;
				initializingMode = false;
			}

			imguiStyles.WindowRounding = FirstRadius;

		}
		ImGui::PopFont();
		return;
	}

#endif // !_DEBUG


	if (!mSocket::cfg::socketIsConnected && mSocket::cfg::socketReconnect)
		renderConnectingToServer(pDevice);


	static int last_tab = variables::Menu_Settings::MenuSelectedPage;
	static bool preview_reverse = false;
	 
	static float m_alpha = 0.0002f;
	m_alpha = ImClamp(m_alpha + (3.f * ImGui::GetIO().DeltaTime * (variables::Menu_Settings::isOpened ? 3.f : -3.f)), 0.0001f, 1.f);

	hNotifierObject.runBasement(&pDevice, m_alpha);
	if (m_alpha <= 0.0001f)
		return;


	static bool bInit = false;

	if (!bInit)
	{
		variables::Menu_Settings::DpiSize = max(1, ImGui::GetIO().DisplaySize.y / 1080);
		bInit = true;
	}
	//ImGui::StyleColorsDark();


	float _iDpiSize = variables::Menu_Settings::DpiSize;
	variables::Menu_Settings::isInitialized = true;
	ImGuiStyle& imguiStyles = ImGui::GetStyle();
	//_iDpiSize *= 1.6;
	

	if (mSocket::cfg::socketIsConnected)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
		{

			{
				//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(variables::Menu_Settings::MenuWidth * _iDpiSize, variables::Menu_Settings::MenuHeight * _iDpiSize));
				ImGui::SetNextWindowBgAlpha(ImClamp(m_alpha, 0.f, .67f));

				imguiStyles.WindowRounding = 13;
			}

			ImGui::Begin("Proximity Menu Base", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
			{

				// font and window size setting
				//ImGui::SetWindowFontScale(3);

				ImGui::BeginChild("pmenu-base-left-c", ImVec2(280 * _iDpiSize, variables::Menu_Settings::MenuHeight * _iDpiSize));
				{
					ImGui::BeginChild("pmenu-base-left-c-top", ImVec2(279 * _iDpiSize, 60 * _iDpiSize));
					{
						ImGui::PushFont(variables::Menu_Settings::fonts_kollektifb_60_font);
						ImVec2 ts = ImGui::CalcTextSize("PROXIMITY");
						imspaceMacro(((279 * _iDpiSize / 2) - (ts.x / 2)), ((60 * _iDpiSize / 2) - (ts.y / 2)));
						ImGui::Text("PROXIMITY");
						//imspaceMacro(5 * _iDpiSize, 5 * _iDpiSize);
						//ImGui::TextColored(ImVec4(.8, .8, .8, .8), "Proximity CS:GO");
						ImGui::PopFont();
					}
					ImGui::EndChild();

					ImGui::BeginChild("pmenu-base-left-c-center", ImVec2(279 * _iDpiSize, (variables::Menu_Settings::MenuHeight - 60 * 2) * _iDpiSize));
					{
						imspaceMacro(0, 15 * _iDpiSize);
						if (ImagedSectionButton("Aimbot", variables::Menu_Settings::ragebotImage, 0, "Aimbot") && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse) variables::Menu_Settings::MenuSelectedPage = 0;

						imspaceMacro(0, 10 * _iDpiSize);
						if (ImagedSectionButton("Anti Aim", variables::Menu_Settings::antiaimImage, 1) && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 1;
							console::log("ok");
						};

						imspaceMacro(0, 15 * _iDpiSize);
						if (ImagedSectionButton("Esp", variables::Menu_Settings::espImage, 2, "Visuals") && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 2;
							console::log("ok");
						};

						imspaceMacro(0, 10 * _iDpiSize);
						if (ImagedSectionButton("Inventory", variables::Menu_Settings::skinsImage, 3) && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 3;
							console::log("ok");
						};

						imspaceMacro(0, 10 * _iDpiSize);
						if (ImagedSectionButton("World", variables::Menu_Settings::worldImage, 4) && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 4;
							console::log("ok");
						};


						imspaceMacro(0, 15 * _iDpiSize);
						if (ImagedSectionButton("Main", variables::Menu_Settings::miscImage, 5, "Misc") && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 5;
							console::log("ok");
						};

						imspaceMacro(0, 10 * _iDpiSize);
						if (ImagedSectionButton("Configs", variables::Menu_Settings::settingsImage, 6) && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 6;
							console::log("ok");
						};

						imspaceMacro(0, 10 * _iDpiSize);
						if (ImagedSectionButton("Scripts", variables::Menu_Settings::scriptsImage, 7) && last_tab == variables::Menu_Settings::MenuSelectedPage && !preview_reverse)
						{
							variables::Menu_Settings::MenuSelectedPage = 7;
							console::log("ok");
						};

						/*if (renderLeftPageSelectorItem("Misc", variables::Menu_Settings::antiaimImage, 5))
						{
							console::log("ok");
						};

						if (renderLeftPageSelectorItem("Sector", variables::Menu_Settings::antiaimImage, 6))
						{
							console::log("ok");
						};

						imspaceMacro(0, 16 * _iDpiSize);

						ImGui::Separator();


						if (renderLeftPageSelectorItem("Configs", variables::Menu_Settings::antiaimImage, 7))
						{
							console::log("ok");
						};

						if (renderLeftPageSelectorItem("Scripts", variables::Menu_Settings::antiaimImage, 8))
						{
							console::log("ok");
						};
						static bool x = 0;
						ToggleButton("we", &x);

						*/
					}
					ImGui::EndChild();


					ImGui::BeginChild("pmenu-base-left-c-bottom", ImVec2(279 * _iDpiSize, 60 * _iDpiSize));
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8);
						ImGui::BeginChild("pmenu-base-left-c-bottom-user_image", ImVec2(60 * _iDpiSize, 60 * _iDpiSize));
						{
							imspaceMacro(10 * _iDpiSize, 10 * _iDpiSize);
							ImGui::Image(variables::Menu_Settings::ragebotImage, ImVec2(40 * _iDpiSize, 40 * _iDpiSize));
						}
						ImGui::EndChild();
						ImGui::PopStyleVar();

						ImGui::SameLine();

						ImGui::BeginChild("pmenu-base-left-c-bottom-user_texts", ImVec2(((279 - 60) * .80) * _iDpiSize, 60 * _iDpiSize));
						{

							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
							imspaceMacro(10 * _iDpiSize, 60 * .25 * _iDpiSize);
							ImGui::Text(variables::NetworkUser::username.c_str());
							ImGui::PopFont();


							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_10_font);
							ImVec2 stillSize = ImGui::CalcTextSize(variables::NetworkUser::subtill.c_str());
							imspaceMacro(10 * _iDpiSize, 4 * _iDpiSize);
							ImGui::Text(variables::NetworkUser::subtill.c_str());
							ImGui::PopFont();
						}
						ImGui::EndChild();

						ImGui::SameLine();

						ImGui::BeginChild("pmenu-base-left-c-bottom-user_settings_btn", ImVec2(((279 - 60) * .20) * _iDpiSize, 60 * _iDpiSize));
						{
							float __cwdt = ((279 - 60) * .20);
							float __chdt = 60;

							imspaceMacro((__cwdt / 2 - (22 / 2)) * _iDpiSize, (__chdt / 2 - 22 / 2) * _iDpiSize);
							ImGui::Image(variables::Menu_Settings::dark_settingsIconImageByte, ImVec2(22 * _iDpiSize, 22 * _iDpiSize));
						}
						ImGui::EndChild();
					}
					ImGui::EndChild();

					ImVec2 wp = ImGui::GetWindowPos();

					ImGui::GetWindowDrawList()->AddLine(ImVec2(wp.x + 279 * _iDpiSize, wp.y + 0), ImVec2(wp.x + 279 * _iDpiSize, wp.y + variables::Menu_Settings::MenuHeight * _iDpiSize), ImColor(180, 180, 180, 60), ImClamp(1 * _iDpiSize, 1.f, 5.f));
				}
				ImGui::EndChild();

				ImGui::SameLine();


				ImGui::PushFont(variables::Menu_Settings::fonts_gubi_20_font);
				ImGui::BeginChild("pmenu-base-right-c", ImVec2(((variables::Menu_Settings::MenuWidth * _iDpiSize) - 280 * _iDpiSize), variables::Menu_Settings::MenuHeight * _iDpiSize), false);
				{
					/*

					if (last_tab != active_tab || (last_tab == active_tab && preview_reverse))
					{
						if (!preview_reverse)
						{
							if (preview_alpha == 1.f)
								preview_reverse = true;

							preview_alpha = math::clamp(preview_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
						}
						else
						{
							last_tab = active_tab;
							if (preview_alpha == 0.01f)
							{
								preview_reverse = false;
							}

							preview_alpha = math::clamp(preview_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
						}
					}
					else
						preview_alpha = math::clamp(preview_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);

					*/

					if (last_tab != variables::Menu_Settings::MenuSelectedPage || (last_tab == variables::Menu_Settings::MenuSelectedPage && preview_reverse))
					{
						if (!preview_reverse)
						{
							if (preview_alpha == 1.f)
								preview_reverse = true;

							preview_alpha = ImClamp(preview_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
						}
						else
						{
							last_tab = variables::Menu_Settings::MenuSelectedPage;
							if (preview_alpha <= 0.01f)
							{
								preview_reverse = false;
							}

							preview_alpha = ImClamp(preview_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
						}
					}
					else
						preview_alpha = ImClamp(preview_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);

					//m_page_change_alpha = ImClamp(m_page_change_alpha + (3.f * ImGui::GetIO().DeltaTime * (m_page_change_way ? 3.f : -3.f)), 0.0001f, 1.f);

					//if (m_page_change_alpha <= 0.0001f && m_page_changing && !m_page_change_way)

					ImGui::BeginChild("pmenu-base-right-base", ImVec2(-1, variables::Menu_Settings::MenuHeight * _iDpiSize), false);
					{
						ImGui::BeginChild("pmenu-base-right-base-t", ImVec2(-1, 60 * _iDpiSize), false);
						{
							/*imspaceMacro(10, 10 * _iDpiSize);
							ImGui::Text("Title");*/

							ImVec2 wp = ImGui::GetWindowPos();
							ImGui::GetWindowDrawList()->AddLine(ImVec2(wp.x, wp.y + 59 * _iDpiSize), ImVec2(wp.x + ((variables::Menu_Settings::MenuWidth * _iDpiSize) - 280 * _iDpiSize), wp.y + 59 * _iDpiSize), ImColor(180, 180, 180, 60), ImClamp(1 * _iDpiSize, 1.f, 5.f));
						}
						ImGui::EndChild();

						ImVec2 bsSize = ImVec2((variables::Menu_Settings::MenuWidth * _iDpiSize) - 280 * _iDpiSize, (variables::Menu_Settings::MenuHeight * _iDpiSize) - (60 * _iDpiSize));


						ImGui::BeginChild("pmenu-base-right-base-content", ImVec2(bsSize.x, bsSize.y), false);
						{
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (1.f - preview_alpha) * m_alpha);
							{
								switch (last_tab)
								{
								case 0:
									renderAimbotPage(bsSize);
									break;
								case 1:
									renderAntiAimPage(bsSize);
									break;
								case 2:
									renderEspPage(bsSize);
									break;

								case 3:
									renderInventoryPage(bsSize);
									break;

								case 4:
									renderWorldPage(bsSize);
									break;

								case 5:
									renderMiscPage(bsSize);
									break;

								case 6:
									renderConfigsPage(bsSize, (1.f - preview_alpha) * m_alpha);
									break;

								case 7:
									renderScriptsPage(bsSize);
									break;
								default:
									break;
								}
							}
							ImGui::PopStyleVar();
						}
						ImGui::EndChild();




					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
				ImGui::PopFont();
			}
			ImGui::End();
		}
		ImGui::PopStyleVar();

		ChatBox::runCustomGui(pDevice, false, m_alpha);

	}
	else {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
		{
			{
				//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(variables::Menu_Settings::MenuWidth * _iDpiSize, variables::Menu_Settings::MenuHeight * _iDpiSize));
				ImGui::SetNextWindowBgAlpha(ImClamp(m_alpha, 0.f, .67f));

				imguiStyles.WindowRounding = 13;
			}

			ImGui::Begin("Proximity Menu Base", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
			{
				imspaceMacro(((variables::Menu_Settings::MenuWidth* _iDpiSize) / 2) - 12 - (2 / 2), ((variables::Menu_Settings::MenuHeight* _iDpiSize) / 2) - 6 - (2 / 2));
				Spinner("#connectingtoserverspinnerbuttonxd2", 12, 2, ImColor(35, 160, 30));
			}
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}


#ifdef _DEBUG

	//ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin("Debug Window");
	{
		ImGui::Text("preview reverse: %d", preview_reverse);
		ImGui::Text("last tab: %d", last_tab);
		ImGui::Text("menu selected page: %d", variables::Menu_Settings::MenuSelectedPage);
		if (ImGui::Button("Unload Cheat", ImVec2(150, 50)))
		{
			variables::cheat::forceCloseCheat = true;
		}

		if (ImGui::Button("Load Map", ImVec2(150, 50)))
		{
			interfaces::engine->execute_cmd("map 848137275");
			variables::Menu_Settings::isOpened = false;
		}

		if (ImGui::Button("ht", ImVec2(150, 50)))
		{
			interfaces::engine->execute_cmd("exec ht");
		}

		if (ImGui::Button("h_ct", ImVec2(150, 50)))
		{
			interfaces::engine->execute_cmd("exec h_ct");
		}

		if (ImGui::Button("h_t", ImVec2(150, 50)))
		{
			interfaces::engine->execute_cmd("exec h_t");
		}

		if (ImGui::Button("h_twice", ImVec2(150, 50)))
		{
			interfaces::engine->execute_cmd("exec h_twice");
		}


		if (ImGui::Button("d p c 1", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 1;
		}

		if (ImGui::Button("d p c 2", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 2;
		}

		if (ImGui::Button("d p c 3", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 3;
		}

		if (ImGui::Button("d p c 4", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 4;
		}

		if (ImGui::Button("d p c 5", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 5;
		}

		if (ImGui::Button("d p c 6", ImVec2(150, 50)))
		{
			variables::Menu_Settings::SelectedDpiScalar = 6;
		}

		ImGui::SliderFloat("Dpi Size", &variables::Menu_Settings::DpiSize, .06, 2);
	}
	ImGui::End();
#endif

	return;

#ifdef ZORT
	{
		static bool loaded = false;
		static bool firstanim = true;
		int currenttime = (float)(clock() / 1000.f);
		static int startedtime = 0;
		static bool savetime = true;

		static bool initedFirstOpen = false;

		float deltaTime = ImGui::GetIO().DeltaTime;
		static float speed = 300.f;

		static float alpha = 0.0f;

		static int initSnowFlakes = false;
		int xxx, yyy;

		interfaces::engine->get_screen_size(xxx, yyy);
		if (!initSnowFlakes)
		{
			initSnowFlakes = true;
			Snowflake::CreateSnowFlakes(snow, SNOW_LIMIT, 1.f/*minimum size*/, 10.f/*maximum size*/, 0/*imgui window x position*/, 0/*imgui window y position*/, xxx, yyy, Snowflake::vec3(0.f, 0.002f)/*gravity*/, IM_COL32(255, 255, 255, 100)/*color*/);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, interfaces::engine->is_in_game() ? (alpha > 0.f ? alpha : 0) : 255);
		{

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(xxx, yyy));
			ImGui::SetNextWindowBgAlpha(0.f);
			POINT mouse;
			ImGui::Begin("##qeqweqwad4qw98e4qw651", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
			{
				RECT rc;
				HWND mhwnd = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
				GetWindowRect(mhwnd, &rc);

				if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
				{
					//MoveWindow(mhwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight(), TRUE); // you dont need those two if you have an imgui window inside an actual window
					ImGui::SetWindowPos(ImVec2(0.f, 0.f));
					// Snowflake::ChangeWindowPos(snow, ImGui::GetWindowPos().x, ImGui::GetWindowPos().y); // you need this if you have an imgui window inside an actual window
				}

				GetCursorPos(&mouse);
				// render this before anything else so it is the background
				Snowflake::Update(snow, Snowflake::vec3(mouse.x, mouse.y)/*mouse x and y*/, Snowflake::vec3(rc.left, rc.top)/*hWnd x and y positions*/); // you can change a few things inside the update function

				// render other stuff

			}
			ImGui::End();
		}
		ImGui::PopStyleVar();


		ImGuiStyle& imguiStyles = ImGui::GetStyle();

		ImClamp(alpha, 0.f, 255.0f);

#ifndef _DEBUG

		{
			if (savetime) {
				startedtime = (float)(clock() / 1000.f);
				savetime = false;
			}

			if (currenttime < startedtime + (firstanim ? 6.f : 2.f)) {

				ImGui::SetNextWindowPos(ImVec2(0, 0));

				ImGui::SetNextWindowSize(ImVec2(1920, 1080));

				windowSize wsz;

				GetWindowSize(wsz, pDevice);

				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.2f));

				ImGui::Begin("Loader :)", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

				imspaceMacro((wsz.width / 2) - 34, (wsz.height / 2) - 34);
				Spinner("#19954", 25, 4, ImColor(35, 160, 30));

				ImGui::End();

				ImGui::PopStyleColor();
			}

			//Erasing
			if (currenttime > startedtime + (firstanim ? 5.f : 1.f)) {
				loaded = true;
				firstanim = false;
			}

			if (!loaded)
				return;
		}


#endif

		variables::Menu_Settings::ui_width_s = calculateUiScalar(variables::Menu_Settings::ui_width);
		variables::Menu_Settings::ui_height_s = calculateUiScalar(variables::Menu_Settings::ui_height);

		if (!mSocket::cfg::socketIsConnected || !mSocket::cfg::authed && false)
		{
			renderConnectingToServer(pDevice);
			//return;
		}

		ImClamp(alpha, 0.f, 255.0f);

		if (!variables::Menu_Settings::isInitialized)
			variables::Menu_Settings::isInitialized = true;

		if (variables::Menu_Settings::isOpened)
		{
			if (alpha < 255)
			{
				alpha += speed * deltaTime;
			}
			else
				alpha = 255.f;
		}
		else
		{
			if (alpha > 0)
				alpha -= speed * deltaTime;
			else
				alpha = 0;
		}

		if (alpha > 255.0f)
		{
			alpha = 255.0f;
		}
		else if (alpha < 0.0f)
		{
			alpha = 0.0f;
		}

		if (!initedFirstOpen && alpha == 255.0f)
		{
			speed = 2250.f;
			initedFirstOpen = true;
		}


		imguiStyles.Alpha = alpha / 255.f;

		if (imguiStyles.Alpha <= 0.0f)
		{
			//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			return;
		}

		setOurCustomImguiColorsAndEtc(pDevice);

		ImGui::GetIO().FontGlobalScale = variables::Menu_Settings::uiSelectedDPI;
		variables::Menu_Settings::updateMenuScalar(variables::Menu_Settings::uiSelectedScalarID);

		imguiStyles.WindowRounding = 8;

		ImGui::Begin("NAME", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		{
			if ((!mSocket::cfg::socketIsConnected || !mSocket::cfg::authed || variables::NetworkUser::fuckThisCheat) && true)
			{
				ImVec2 nPos = {};
				ImVec2 nPos2 = {};
				std::string thd = "";
				std::string thd2 = "If you haven't done anything, you don't have to be afraid.";

				getMenuPos(nPos);
				getCursorPos(nPos2);

				if (!mSocket::cfg::authed && mSocket::cfg::socketIsConnected)
					thd = "Waiting authentication...";
				else if (variables::NetworkUser::fuckThisCheat)
					thd = "You need to reload your pc or we will ban you (Security PreProces Activated)";
				else
					thd = "Cheat need to connect to server";

				ImVec2 sthd = ImGui::CalcTextSize(thd.c_str());
				imspaceMacro(variables::Menu_Settings::ui_width_s / 2 - sthd.x / 2, variables::Menu_Settings::ui_height_s / 2 - sthd.y / 2);
				ImGui::Text(thd.c_str());



				if (variables::NetworkUser::fuckThisCheat)
				{
					ImVec2 sthd2 = ImGui::CalcTextSize(thd2.c_str());
					imspaceMacro(variables::Menu_Settings::ui_width_s / 2 - sthd2.x / 2, 20);
					ImGui::Text(thd2.c_str());
				}
			}
			else
			{
				imspaceMacro(5, 5);

				ImVec2 nPos = {};
				ImVec2 nPos2 = {};

				getMenuPos(nPos);
				getCursorPos(nPos2);


				ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(230), nPos.y), ImVec2(nPos.x + calculateUiScalar(230), nPos.y + calculateUiScalar(variables::Menu_Settings::ui_height_s)), ImColor(150, 150, 150, 60), 2);


				ImGui::BeginChild("#left-side", ImVec2(calculateUiScalar(220), calculateUiScalar(variables::Menu_Settings::ui_height_s - 10)));
				{

					ImGui::BeginChild("#left-upper", ImVec2(calculateUiScalar(220), calculateUiScalar(variables::Menu_Settings::ui_height_s - 60)));
					{
						ImGui::PushFont(variables::Menu_Settings::fonts_GUIBIGFONT2);
						imspaceMacro(calculateUiScalar(220) / 2 - (ImGui::CalcTextSize("PROXIMITY", nullptr).x / 2), 0);
						ImGui::Text("PROXIMITY");
						ImGui::PopFont();


						//ImGui::Text(std::to_string(ImGui::GetIO().FontGlobalScale).c_str());



						ImGui::GetStyle().FrameRounding = 4;

						imspaceMacro(0, 20);

						auto selButItem = [](std::string item_name, std::string item_id, int page_id, LPDIRECT3DTEXTURE9 theTexture = nullptr, int x = 13, int y = 13) {
							ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(variables::Menu_Settings::selected_page == page_id ? 0.1f : 0.f, 0.5f));
							{
								if (theTexture != nullptr)
								{
									imspaceMacro(10, 0);
									ImGui::BeginChild(std::string(std::string("##_zkdld_item-left-state1_") + item_id).c_str(), ImVec2(30, 30), false);
									{
										imspaceMacro(30 / 2 - x / 2, 30 / 2 - y / 2);
										ImGui::Image(theTexture, ImVec2(x, y));
									}
									ImGui::EndChild();

									ImGui::SameLine();

									imspaceMacro(5, 0);
									ImGui::BeginChild(std::string(std::string("##_zkdld_item-left-state2_") + item_id).c_str(), ImVec2(170, 30), false);
									{
										if (ImGui::Button(item_name.c_str(), ImVec2(calculateUiScalar(170), calculateUiScalar(30))))
											variables::Menu_Settings::selected_page = page_id;
									}
									ImGui::EndChild();
								}
								else
								{
									imspaceMacro(10, 0);
									if (ImGui::Button(item_name.c_str(), ImVec2(calculateUiScalar(180), calculateUiScalar(30))))
										variables::Menu_Settings::selected_page = page_id;
								}
							}
							ImGui::PopStyleVar();
						};

						ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
						{
							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									imspaceMacro(10, 0);
									ImGui::TextColored(ImVec4(1, 1, 1, 0.8), "Aimbot Side");
								}
								ImGui::PopFont();
								ImVec2 nPos = {};
								ImVec2 nPos2 = {};

								getMenuPos(nPos);
								getCursorPos(nPos2);

								ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(5), nPos.y + nPos2.y + 4), ImVec2(nPos.x + calculateUiScalar(220 - 100), nPos.y + nPos2.y + 4), ImColor(150, 150, 150, 40), 1);
								imspaceMacro(0, 10);
							}

							{
								selButItem("Aim Bot", "aimbot", 0, variables::Menu_Settings::ragebotImage, 26, 26);
								selButItem("Anti Aim", "antiaim", 1, variables::Menu_Settings::antiaimImage, 26, 26);
								imspaceMacro(0, 10);
							}



							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									imspaceMacro(10, 0);
									ImGui::TextColored(ImVec4(1, 1, 1, 0.8), "Some Visuals Side");
								}
								ImGui::PopFont();
								ImVec2 nPos = {};
								ImVec2 nPos2 = {};

								getMenuPos(nPos);
								getCursorPos(nPos2);

								ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(5), nPos.y + nPos2.y + 4), ImVec2(nPos.x + calculateUiScalar(220 - 100), nPos.y + nPos2.y + 4), ImColor(150, 150, 150, 40), 1);
								imspaceMacro(0, 10);
							}


							{
								selButItem("Esp", "esp", 2, variables::Menu_Settings::espImage, 26, 26);
								selButItem("Chams", "chams", 3, variables::Menu_Settings::chamsImage, 26, 26);
								selButItem("World", "world", 4, variables::Menu_Settings::worldImage, 26, 26);
								imspaceMacro(0, 10);
							}



							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									imspaceMacro(10, 0);
									ImGui::TextColored(ImVec4(1, 1, 1, 0.8), "Other Thinks Side");
								}
								ImGui::PopFont();
								ImVec2 nPos = {};
								ImVec2 nPos2 = {};

								getMenuPos(nPos);
								getCursorPos(nPos2);

								ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(5), nPos.y + nPos2.y + 4), ImVec2(nPos.x + calculateUiScalar(220 - 100), nPos.y + nPos2.y + 4), ImColor(150, 150, 150, 40), 1);
								imspaceMacro(0, 10);
							}


							{
								selButItem("Misc", "misc", 5, variables::Menu_Settings::miscImage, 26, 26);
								selButItem("Skins", "skins", 6, variables::Menu_Settings::skinsImage, 26, 26);
								imspaceMacro(0, 10);
							}



							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									imspaceMacro(10, 0);
									ImGui::TextColored(ImVec4(1, 1, 1, 0.8), "Network Side");
								}
								ImGui::PopFont();
								ImVec2 nPos = {};
								ImVec2 nPos2 = {};

								getMenuPos(nPos);
								getCursorPos(nPos2);

								ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(5), nPos.y + nPos2.y + 4), ImVec2(nPos.x + calculateUiScalar(220 - 100), nPos.y + nPos2.y + 4), ImColor(150, 150, 150, 40), 1);
								imspaceMacro(0, 10);
							}

							{
								selButItem("Scripts", "scripts", 8, variables::Menu_Settings::scriptsImage, 26, 26);
								selButItem("Settings", "settings", 8, variables::Menu_Settings::settingsImage, 26, 26);
							}

							ImGui::GetStyle().FrameRounding = 4;
						}
						ImGui::PopFont();
					}
					ImGui::EndChild();

					ImGui::BeginChild("#left-bottomer", ImVec2(calculateUiScalar(220), calculateUiScalar(50)));
					{
						ImGui::BeginChild("#networkusernameside_11", ImVec2(220 / 5, 50));
						{
							imspaceMacro(3, 50 / 2 - 40 / 2);
							ImGui::Image(variables::Menu_Settings::networkUserImage, ImVec2(40, 40));
						}
						ImGui::EndChild();

						ImGui::SameLine();

						ImGui::BeginChild("#networkusernameside_12", ImVec2((220 / 5) * 4, 50));
						{
							ImGui::PushFont(variables::Menu_Settings::fonts_gubi_16_font);
							{
								imspaceMacro(calculateUiScalar(5), 50 / 2 - ImGui::CalcTextSize(variables::NetworkUser::username.c_str()).y - 3);
								ImGui::Text(variables::NetworkUser::username.c_str());

								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									imspaceMacro(5, 3);
									ImGui::TextColored(ImVec4(0.8, 0.8, 0.8, 1), "Till:");
									ImGui::SameLine();
									imspaceMacro(5, 0);
									ImGui::TextColored(ImVec4(1, 1, 1, 1), variables::NetworkUser::subtill.c_str());
								}
								ImGui::PopFont();

							}
							ImGui::PopFont();
						}
						ImGui::EndChild();
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();

				ImGui::SameLine();

				ImVec2 cMenuSize = ImVec2(calculateUiScalar(variables::Menu_Settings::ui_width_s - 235 + 2), calculateUiScalar(variables::Menu_Settings::ui_height_s - 10));

				imspaceMacro(6, 0);

				ImGui::BeginChild("#right-side", cMenuSize, false);
				{
					imguiStyles.Colors[ImGuiCol_Button] = ImColor(40, 40, 40);

					ImGui::BeginChild("#state-upper", ImVec2(cMenuSize.x, 50), false); {

						{
							switch (variables::Menu_Settings::selected_page)
							{
							case 8:
							{
								ImGui::PushFont(variables::Menu_Settings::fonts_gubi_14_font);
								{
									if (!mSocket::cfg::waiting_response)
									{
										imspaceMacro(5, 5);
										if (ImGui::Button("Refresh Configs", ImVec2(100, 35)))
										{
											config::reInitOnlyList();
										}

										ImGui::SameLine();
										imspaceMacro(10, 0);

										if (ImGui::Button("Create Config", ImVec2(100, 35)))
										{
											config::createConfig();
										}
									}
								}
								ImGui::PopFont();
							}
							break;
							}
						}




						ImVec2 nPos = {};
						ImVec2 nPos2 = {};

						getMenuPos(nPos);
						getCursorPos(nPos2);

						ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x, nPos.y + 48), ImVec2(nPos.x + cMenuSize.x, nPos.y + 48), ImColor(150, 150, 150, 60), 2);

					}
					ImGui::EndChild();

					switch (variables::Menu_Settings::selected_page)
					{
					case 0:
						renderAimbotPage();
						break;
					case 1:
						renderAntiAimPage();
						break;
					case 2:
						renderEspPage();
						break;
					case 3:
						renderChamsPage();
						break;

					case 5:
						renderMiscPage();
						break;

					case 8:
						renderConfigsPage();
						break;


						///////////////////////


					case 6:
						renderSkinsPage(pDevice);
						break;
					default:
						break;
					}

				}
				ImGui::EndChild();
			}
		}
		ImGui::End();



		ChatBox::runCustomGui(pDevice);


#ifdef _DEBUG

		//ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("Debug Window");
		{
			if (ImGui::Button("Load Map", ImVec2(150, 50)))
			{
				interfaces::engine->execute_cmd("map 848137275");
				variables::Menu_Settings::isOpened = false;
			}

			if (ImGui::Button("ht", ImVec2(150, 50)))
			{
				interfaces::engine->execute_cmd("exec ht");
			}

			if (ImGui::Button("h_ct", ImVec2(150, 50)))
			{
				interfaces::engine->execute_cmd("exec h_ct");
			}

			if (ImGui::Button("h_t", ImVec2(150, 50)))
			{
				interfaces::engine->execute_cmd("exec h_t");
			}

			if (ImGui::Button("h_twice", ImVec2(150, 50)))
			{
				interfaces::engine->execute_cmd("exec h_twice");
			}

			ImGui::SliderFloat("Dpi Size", &variables::Menu_Settings::DpiSize, .06, 2);
		}
		ImGui::End();
#endif
	}
#endif // ZORT

	
}


void setOurCustomImguiColorsAndEtc(LPDIRECT3DDEVICE9 pDevice)
{
	ImGuiStyle& imguiStyles = ImGui::GetStyle();

	static bool firstLoadInit = false;

	
	if (!firstLoadInit)
	{
		firstLoadInit = true;
		imguiStyles.WindowPadding = ImVec2(0, 0);
		imguiStyles.FramePadding = ImVec2(1.2f, 1.2f);
		imguiStyles.CellPadding = ImVec2(0, 0);
		imguiStyles.ItemSpacing = ImVec2(0, 0);
		imguiStyles.ItemInnerSpacing = ImVec2(10, 0);

		windowSize wsz;

		GetWindowSizeX(wsz, pDevice);


		ImGui::SetNextWindowPos(ImVec2((wsz.width / 2) - (variables::Menu_Settings::ui_width_s / 2), (wsz.height / 2) - (variables::Menu_Settings::ui_height_s / 2)));
	}

	

	
	return;
	ImGui::SetNextWindowSize(ImVec2(variables::Menu_Settings::ui_width_s, variables::Menu_Settings::ui_height_s));

	imguiStyles.Colors[ImGuiCol_Button]					= getZtkColor(20, 20, 20, 0);
	imguiStyles.Colors[ImGuiCol_ButtonHovered]			= getZtkColor(35, 120, 30);
	imguiStyles.Colors[ImGuiCol_ButtonActive]			= getZtkColor(35, 90, 30);


	imguiStyles.Colors[ImGuiCol_CheckMark]				= getZtkColor(30, 220, 30);

	imguiStyles.Colors[ImGuiCol_FrameBg]				= getZtkColor(40, 40, 40);
	imguiStyles.Colors[ImGuiCol_FrameBgHovered]			= getZtkColor(50, 50, 50);
	imguiStyles.Colors[ImGuiCol_FrameBgActive]			= getZtkColor(50, 110, 50);



	imguiStyles.Colors[ImGuiCol_Header]					= getZtkColor(50, 180, 50);
	imguiStyles.Colors[ImGuiCol_HeaderHovered]			= getZtkColor(35, 120, 30);
	imguiStyles.Colors[ImGuiCol_HeaderActive]			= getZtkColor(35, 90, 30);


	imguiStyles.Colors[ImGuiCol_SliderGrab]				= getZtkColor(30, 220, 30);
	imguiStyles.Colors[ImGuiCol_SliderGrabActive]		= getZtkColor(30, 220, 30);
}



void setupMenu(LPDIRECT3D9 pDevice) {

	auto& io = ImGui::GetIO();

	
}