#pragma once
#include "core/menu/global_input.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include "source-sdk/misc/color.hpp"
#include <shellapi.h>

namespace vfuns
{
	inline color getcolorofimcolor(ImColor item) { return color(255, 255, 255, 255); }
}

namespace variables {
	namespace NetworkUser
	{
		inline std::string username = "null";
		inline bool fuckThisCheat = false;
	}

	namespace cheat
	{
		inline bool forceCloseCheat = false;
		inline std::list<std::string> logboxLists;
		inline std::string socketError = "";

		inline void openLink(std::string pData)
		{
			std::string eData = std::string(std::string("https://rogsoftware.com/error?exmp=") + std::string(pData));
			ShellExecuteW(0, 0, (LPCWSTR)eData.c_str(), 0, 0, SW_SHOW);
		}
	};

	namespace aim {
		inline bool triggerbot = false;
		inline hotkey_t triggerbot_key(VK_PRIOR);
		inline float triggerbot_delay = 0.f;

		inline bool aimbot = false;
		inline bool silent = false;
		inline bool autofire = false;
		inline hotkey_t aimbot_key(VK_NEXT);

		inline bool bodyaim_if_lethal = false;
		inline bool priorize_lethal_targets = false;		// If we can kill a someone inside our fov, go for it even if it's not the closest one

		inline std::vector<std::string> autowall_settings = {
			"Only visible",
			"Autowall",
			"Ignore walls"
		};


		inline float aimbot_fov = 20.f;
		inline bool draw_fov = false;

		inline float aimbot_smoothing = 0.f;
		inline bool target_friends = false;
		inline bool non_rifle_aimpunch = false;
		inline bool aimbot_noscope = false;
		inline bool autorevolver = false;

		inline float min_damage = 60.f;

		struct hitboxLamp
		{
			
		};

		inline bool selected_hitboxes[] = {
			false,
			false,
			false,
			false,
			false
		};
		//inline multicombobox_toggle_t hitboxes(hitboxes_options);
	}

	namespace antiaim {
		inline bool antiaim = false;
		inline float yaw = 0.f;
		inline float pitch = 0.f;
		inline bool spinbot = false;
		inline float spinbot_speed = 0.f;
		inline bool peek_aa = false;
		inline hotkey_t peek_aa_toggle_key(VK_XBUTTON1);
	}

	namespace	player_visuals {
		inline bool showteamesp = false;
		inline bool playerglow = false;
		inline bool boxesp = false;
		inline bool lineesp = false;
		inline bool skeletonesp = false;
		inline bool nameesp = false;

		inline struct multicombo_opt_t
		{
			const char* text = "";
			bool value = false;
		};

		inline std::vector<multicombo_opt_t> playerinfo_options = {
			{ "Weapon name",	false },
			{ "Weapon icon",	false },
			{ "Armor",			false },
			{ "Defuser",		false },
			{ "Bomb",			false },
			{ "Scoped",		false },
			{ "Flashed",		false },
			{ "Can fire",		false }
		};

		inline bool healthesp = false;
	}

	namespace entity_visuals {
		inline std::vector<std::string> entity_esp_settings = {
			"None",
			"Text",
			"Icon"
		};
		inline int entity_esp = 0;

		inline bool entityglow = false;
		inline bool bombtimer = true;
		inline bool nade_esp = false;
	}

	namespace chams {
		inline std::vector<std::string> materials = {
			"Transparent",
			"Textured",
			"Flat",
			"Ghost",
			"Gold 1",
			"Gold 2",
			"Amethyst",
			"Silver",
			"Steel",
			"Dark steel",
			"Plastic",
			"Red glow",
			"Effect 1",
			"Effect 2",
			"Effect 3",
			"Animated blink",
			"Animated stripes"
		};

		// Enabled options
		inline bool only_visible_chams = false;
		inline bool draw_chams_on_top = false;
		inline bool wireframe_chams = false;
		inline bool player_chams = false;
		inline bool localplayer_chams = false;
		inline bool vm_weapon_chams = false;
		inline bool vm_arm_chams = false;
		inline bool vm_sleeve_chams = false;

		// Selected material for each setting
		inline int player_chams_mat_id = 0;
		inline int localplayer_chams_mat_id = 0;
		inline int weapon_chams_mat_id = 0;
		inline int arm_chams_mat_id = 0;
		inline int sleeve_chams_mat_id = 0;

		// Backtrack
		inline bool backtrack_chams = false;
	}

	namespace misc_visuals {
		inline float noflash_alpha = 0.f;
		inline bool wireframe_smoke = false;
		inline bool nade_predict = false;
		inline bool noscope = false;
		inline bool crosshair = false;
		inline bool recoil_crosshair = false;
		inline bool worldcolor = false;

		/*inline std::vector<multicombo_opt_t> bulletracer_draw_options = {
			{ "Line",			false },
			{ "Impacts",		false }
			//{ "Hits",			false }		// Will just draw a different color for hits
		};
		inline std::vector<multicombo_opt_t> bulletracer_team_options = {
			{ "Localplayer",	true },
			{ "Friendly",		false },
			{ "Enemy",			false }
		};
		inline multicombobox_toggle_t bulletracer_draw_target(bulletracer_draw_options);
		inline multicombobox_toggle_t bulletracer_team_target(bulletracer_team_options);*/

		inline bool bulletracer_team_target[] = { false, false, false };
		inline int bulletracer_type = 0;

		inline bool chickenpride = false;

		inline float custom_fov_slider = 90.f;
		inline float custom_vmfov_slider = 1.f;
	}

	namespace misc {
		// Movement
		inline bool infinite_duck = false;
		inline bool bhop = false;

		inline std::vector<std::string> autostrafe_options = {
			"None",
			"Legit",
			"Forward rage",
			"Multidir rage"
		};

		inline int autostrafe_target = 0;

		inline bool edgejump = false;
		inline hotkey_t ej_key('C');	// Mind the ' ' and the capital letter
		inline bool edgebug = false;
		inline hotkey_t eb_key(VK_XBUTTON1);
		inline bool jumpbug = false;
		inline hotkey_t jb_key(VK_MENU);		// Alt key
		inline bool slowwalk = false;
		inline hotkey_t slowwalk_key(VK_MENU);	// Alt key

		/*inline std::vector<multicombo_opt_t> speedgraph_options = {
			{ "Line",		false },
			{ "Color",		false },
			{ "Speed",		true },
			{ "Jump",		true }
		};
		inline multicombobox_toggle_t speedgraph_target(speedgraph_options);*/

		inline bool speedGraph[] = { false, false, false };
		inline float speedgraph_h = 40.f;
		inline float speedgraph_pos = 92.5;

		// Network
		inline bool backtrack = false;
		inline bool backtrack_team = false;
		inline float backtrack_ticks = 200.f;

		// Thirdperson
		inline bool thirdperson = false;
		inline int thirdperson_key = KEY_V;
		inline float thirdperson_dist = 100.f;

		// UI
		/*inline std::vector<multicombo_opt_t> watermark_options = {
			{ "User",		true },
			{ "Cheat",		true },
			{ "Fps",		true },
			{ "Ping",		true }
		};
		inline multicombobox_toggle_t watermark(watermark_options);*/
		inline bool clean_screenshots = false;
		inline bool reveal_ranks = false;
	}

	namespace ui {
		namespace spectators {
			inline bool spectator_list = false;
			inline int x = 10, y = 560;
			inline int w = 100, h = 10;
		}

		namespace menux {
			inline bool opened = false;
			inline int x = 300, y = 200;
			inline int w = 500, h = 450;
		}

		namespace watermark {
			inline int y = 10;
			inline int x = 15;
			inline int w = 0;
			inline int h = 17;
		}
	}

	namespace crosshair {
		const inline bool only_engine_crosshair = false;		// Edit here
		inline bool using_cs_crosshair = false;

		inline float crosshair_len = 3;		// 2-20
		inline float crosshair_w = 2;		// 2-15
		inline float crosshair_gap = 0;		// 0-10

		inline bool rainbow_crosshair = false;
	}

	// Despite the indentation, we are calling constructor with color as argument
	namespace colors {
		// Esp and glow

		inline ImColor friendly_color = ImColor(1, 1, 1, 1);
		inline ImColor friendly_color_soft = ImColor(250, 250, 250, 250);
		inline ImColor friendly_color_softer = ImColor(250, 250, 250, 250);
		inline ImColor enemy_color = ImColor(1, 1, 1, 1);
		inline ImColor enemy_color_soft = ImColor(250, 250, 250, 250);
		inline ImColor enemy_color_softer = ImColor(250, 250, 250, 250);

		inline ImColor friendly_glow_c = ImColor(250, 250, 250, 250);
		inline ImColor enemy_glow_c = ImColor(250, 250, 250, 250);
		inline ImColor entity_glow_c = ImColor(250, 250, 250, 250);

		// Chams
		inline ImColor chams_localplayer = ImColor(250, 250, 250, 250);
		inline ImColor chams_vis_enemy_c = ImColor(250, 250, 250, 250);
		inline ImColor chams_inv_enemy_c = ImColor(250, 250, 250, 250);
		inline ImColor chams_vis_friend_c = ImColor(250, 250, 250, 250);
		inline ImColor chams_inv_friend_c = ImColor(250, 250, 250, 250);

		inline ImColor chams_weapon_c = ImColor(250, 250, 250, 250);
		inline ImColor chams_arms_c = ImColor(250, 250, 250, 250);
		inline ImColor chams_sleeve_c = ImColor(250, 250, 250, 250);

		inline ImColor bt_chams_enemy = ImColor(250, 250, 250, 250);
		inline ImColor bt_chams_friend = ImColor(250, 250, 250, 250);
		inline ImColor bt_chams_enemy_fade = ImColor(250, 250, 250, 250);
		inline ImColor bt_chams_friend_fade = ImColor(250, 250, 250, 250);

		// Misc
		inline ImColor crosshair_c = ImColor(250, 250, 250, 250);
		inline ImColor recoil_crosshair_c = ImColor(250, 250, 250, 250);
		inline ImColor aimbot_fov_c = ImColor(250, 250, 250, 250);
		inline ImColor worldcolor_c = ImColor(250, 250, 250, 250);






		inline ImColor bulletBeamColor[] = { ImColor(255), ImColor(255), ImColor(255) };
	};

	inline struct MotionBlur {
		bool enabled{ false };
		bool forwardEnabled{ false };
		float fallingMin{ 10.0f };
		float fallingMax{ 20.0f };
		float fallingIntensity{ 1.0f };
		float rotationIntensity{ 1.0f };
		float strength{ 4.0f };
	} motion_blur;




	namespace Menu_Settings {
		inline bool isOpened = true;
		inline bool isInitialized = false;
		inline int ui_width = 780;
		inline int ui_height = 590;

		inline int ui_width_s = 0;
		inline int ui_height_s = 0;

		inline int ui_scalar = 100;

		inline int selected_page = 0;

		inline ImFont* fonts_GUIBIGFONT;
		inline ImFont* fonts_GUIBIGFONT2;

		inline int uiSelectedScalarID = 4;
		inline float uiSelectedDPI = 1;
		inline const char* uiSelectedScalarName[10] = { "0", "25", "50", "75", "100", "125", "150", "175", "200", "250" };

		inline void updateMenuScalar(int selectedItem) {
			switch (selectedItem)
			{
			case 0:
				variables::Menu_Settings::ui_scalar = 0;
				break;
			case 1:
				variables::Menu_Settings::ui_scalar = 25;
				break;
			case 2:
				variables::Menu_Settings::ui_scalar = 50;
				break;
			case 3:
				variables::Menu_Settings::ui_scalar = 75;
				break;
			case 4:
				variables::Menu_Settings::ui_scalar = 100;
				break;
			case 5:
				variables::Menu_Settings::ui_scalar = 125;
				break;
			case 6:
				variables::Menu_Settings::ui_scalar = 150;
				break;
			case 7:
				variables::Menu_Settings::ui_scalar = 175;
				break;
			case 8:
				variables::Menu_Settings::ui_scalar = 200;
				break;
			case 9:
				variables::Menu_Settings::ui_scalar = 250;
				break;
			default:
				variables::Menu_Settings::ui_scalar = 100;
				break;
			}
		}
	}

	namespace Aimbot_Settings {
		inline bool enabled = false;
		inline bool teamCheck = false;
		inline bool noScope = false;
		inline bool autofire = false;
		inline bool silentAim = false;
		inline bool non_rifle_aimpunch = false;
		inline bool auto_revolver = false;
		inline bool drawFov = false;
		inline bool priorize_lethal_targets = false;
		inline bool auto_wall = false;

		inline int aimbotKey = -2;

		inline float aimbot_smoothing = 0.f;


		inline float min_damage = 0.f;
		inline float aimbot_fov = 0.f;
		inline float aimbot_hitchance = 0.f;

		inline bool selected_hitboxes[] = {
			false,
			false,
			false,
			false,
			false
		};

		inline const char* selected_hitboxes_names[] = {
			"head",
			"neck",
			"chest",
			"arms",
			"legs"
		};
	};

	namespace Esp_Settings {
		inline int espTeamState = 0;
		inline int selected_team = 0;  
		inline bool test = false;

		inline bool enabledBase = false;
		inline bool enabledNameesp[] = { false, false, false };
		inline bool enabledHealthesp[] = { false, false, false };
		inline bool enabledBox[] = { false, false, false };
		inline bool enabledLine[] = { false, false, false };
		inline bool enabledSkeleton[] = { false, false, false };
		inline bool enabledGethers[] = { false, false, false };
	};


	namespace Skin_Changer
	{
		inline bool isEnabledBasement = false;


		inline int skinlistSelectedWeaponID = -1;

		inline bool isEnabled[70];
		inline int newKnifeID = 0;
		inline int newGloveID = 0;

		inline int newPaintKit[70];
		inline int newSeed[70];
		inline int newStatTrak[70];
		inline int newQuality[70];
		inline float newWear[70];

		inline const char* newCustomName[70];



		inline std::vector<const char*> menuSkinsItemDefins = 
		{
			"Knife",
			"Glove",
			"AK-47",
			"AUG",
			"AWP",
			"CZ75 Auto",
			"Desert Eagle",
			"Dual Berettas",
			"FAMAS",
			"Five-SeveN",
			"G3SG1",
			"Galil AR",
			"Glock-18",
			"M249",
			"M4A1-S",
			"M4A4",
			"MAC-10",
			"MAG-7",
			"MP5-SD",
			"MP7",
			"MP9",
			"Negev",
			"Nova",
			"P2000",
			"P250",
			"P90",
			"PP-Bizon",
			"R8 Revolver",
			"Sawed-Off",
			"SCAR-20",
			"SSG 08",
			"SG 553",
			"Tec-9",
			"UMP-45",
			"USP-S",
			"XM1014",
		};

		inline struct SkinSetSt
		{
			bool isEnabled = false;
			bool isKnife = false;
			bool isGlove = false;
			int newKnifeID = NULL;
			int newGloveID = NULL;

			int MenuID = 0;
			int GameID = 0;

			int PaintKit = 0;
			int Seed = 0;
			int StatTrack = 0;
			int Quality = 0;
			float Wear = 0;
			std::string CustomName = "";
		};

		inline std::vector<const char*> knifeNames = 
		{
			"-",
			"BAYONET",
			"M9_BAYONET",
			"KARAMBIT",
			"SURVIVAL_BOWIE",
			"BUTTERFLY",
			"FALCHION",
			"FLIP",
			"GUT",
			"TACTICAL",   // Huntsman
			"PUSH",
			"GYPSY_JACKKNIFE",
			"STILETTO",
			"WIDOWMAKER",      // Talon
			"SKELETON",
			"URSUS",
			"CSS"
		};

		inline int selectedKnifeNameID = 0;

		inline int getKnifeNewID()
		{
			switch (selectedKnifeNameID)
			{
			case 0:
				return 42;
				break;
			case 1:
				return WEAPON_BAYONET;
				break;
			case 2:
				return WEAPON_KNIFE_M9_BAYONET;
				break;
			case 3:
				return WEAPON_KNIFE_KARAMBIT;
				break;
			case 4:
				return WEAPON_KNIFE_SURVIVAL_BOWIE;
				break;
			case 5:
				return WEAPON_KNIFE_BUTTERFLY;
				break;
			case 6:
				return WEAPON_KNIFE_FALCHION;
				break;
			case 7:
				return WEAPON_KNIFE_FLIP;
				break;
			case 8:
				return WEAPON_KNIFE_GUT;
				break;
			case 9:
				return WEAPON_KNIFE_TACTICAL;
				break;
			case 10:
				return WEAPON_KNIFE_PUSH;
				break;
			case 11:
				return WEAPON_KNIFE_GYPSY_JACKKNIFE;
				break;
			case 12:
				return WEAPON_KNIFE_STILETTO;
				break;
			case 13:
				return WEAPON_KNIFE_WIDOWMAKER;
				break;
			case 14:
				return WEAPON_KNIFE_SKELETON;
				break;
			case 15:
				return WEAPON_KNIFE_URSUS;
				break;
			case 16:
				return WEAPON_KNIFE_CSS;
				break;
			default:
				return 0;
				break;
			}
		}

		inline SkinSetSt getValueFromList(int skinWeaponID)
		{
			int bBar = -1;

			skins::_st::wSkins_Weplst bbeka;

			for(auto item : skins::wItemDefs)
			{
				if (item.weaponGameID == skinWeaponID)
				{
					bBar = item.weaponMenuID;
					bbeka = item;
					break;
				}
			}

			SkinSetSt rbeka;

			if (bBar == -1)
			{
				rbeka.isEnabled = false;
				rbeka.isKnife = false;
				rbeka.isGlove = false;
				rbeka.newKnifeID = 0;
				rbeka.newGloveID = 0;
				rbeka.PaintKit = 0;
				rbeka.Seed = 0;
				rbeka.StatTrack = 0;
				rbeka.Quality = 0;
				rbeka.Wear = 0;
				rbeka.CustomName = "";
			}
			else
			{
				rbeka.isEnabled = variables::Skin_Changer::isEnabled[bBar];
				rbeka.isKnife = bBar == 0 ? true : false;
				rbeka.isGlove = bBar == 1 ? true : false;
				rbeka.newKnifeID = bBar == 0 ? getKnifeNewID() : 0;
				rbeka.newGloveID = variables::Skin_Changer::newGloveID;
				rbeka.PaintKit = variables::Skin_Changer::newPaintKit[bBar];
				rbeka.Seed = variables::Skin_Changer::newSeed[bBar];	
				rbeka.StatTrack = variables::Skin_Changer::newStatTrak[bBar];
				rbeka.Quality = variables::Skin_Changer::newQuality[bBar];
				rbeka.Wear = variables::Skin_Changer::newWear[bBar];
				//rbeka.CustomName = variables::Skin_Changer::newCustomName[bBar];
				rbeka.GameID = bbeka.weaponGameID;
				rbeka.MenuID = bbeka.weaponMenuID;
			}



			return rbeka;
		}
	};


};