#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#pragma region BOUNDING BOX FUNCTION
// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Bounding_ESP_Boxes
// Can be used for any collideable entity, not just players.
bool bbox(entity_t *entity, int &x, int &y, int &w, int &h) {
	auto collideable = entity->collideable();
	auto obb_mins    = collideable->obb_mins();
	auto obb_maxs    = collideable->obb_maxs();
	auto &trans	     = entity->coordinate_frame();

	vec3_t points[] = {
		{obb_mins.x, obb_mins.y, obb_mins.z},
		{obb_mins.x, obb_maxs.y, obb_mins.z},
		{obb_maxs.x, obb_maxs.y, obb_mins.z},
		{obb_maxs.x, obb_mins.y, obb_mins.z},
		{obb_maxs.x, obb_maxs.y, obb_maxs.z},
		{obb_mins.x, obb_maxs.y, obb_maxs.z},
		{obb_mins.x, obb_mins.y, obb_maxs.z},
		{obb_maxs.x, obb_mins.y, obb_maxs.z}
	};

	for (int i = 0; i < 8; i++) {
		vec3_t t, s;
		math::transform_vector(points[i], trans, t);
		if (!math::world_to_screen(t, s))
			return false;
		points[i] = s;
	}

	auto left   = points[0].x;
	auto bottom = points[0].y;
	auto right  = points[0].x;
	auto top    = points[0].y;

	for (int i = 0; i < 8; i++) {
		if (left   > points[i].x) left   = points[i].x;
		if (bottom < points[i].y) bottom = points[i].y;
		if (right  < points[i].x) right  = points[i].x;
		if (top    > points[i].y) top    = points[i].y;
	}

	x = static_cast<int>(left);
	y = static_cast<int>(top);
	w = static_cast<int>(right - left);
	h = static_cast<int>(bottom - top);

	return true;
}
#pragma endregion

void visuals::playeresp() {
	// Check if there is at least one item enabled
	bool player_info_enabled = false;
	for (variables::player_visuals::multicombo_opt_t item : variables::player_visuals::playerinfo_options) {
		if (item.value) {
			player_info_enabled = true;
			break;
		}
	}

	if (!player_info_enabled && !variables::Esp_Settings::enabledBase) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	// Will ignore ESP if the player being spectated
	player_t* local_player_ent = helpers::local_or_spectated();

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player) continue;
		if (player == local_player_ent)
			if (!variables::misc::thirdperson)
				continue;

		if (player->dormant()) continue;
		if (!(player->is_alive() && player->health() > 0)) continue;

		vec3_t chest = player->get_hitbox_position(hitbox_upper_chest);

		const auto player_model = player->model();
		if (!player_model) continue;
		auto hdr = interfaces::model_info->get_studio_model(player_model);
		if (!hdr) continue;
		static matrix_t bones[MAXSTUDIOBONES];
		if (!player->setup_bones(bones, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0)) continue;

		int x, y, w, h;
		if (!bbox(player, x, y, w, h)) continue;

		#pragma region SKELETON ESP
		if (variables::Esp_Settings::enabledSkeleton[0] || variables::Esp_Settings::enabledSkeleton[1] || variables::Esp_Settings::enabledSkeleton[2]) {
			for (int i = 0; i < hdr->bones_count; i++) {
				studio_bone_t* bone = hdr->bone(i);
				if (!bone || !(bone->flags & 256) || bone->parent == -1) continue;

				auto child  = vec3_t(bones[i][0][3], bones[i][1][3], bones[i][2][3]);
				auto parent = vec3_t(bones[bone->parent][0][3], bones[bone->parent][1][3], bones[bone->parent][2][3]);

				// https://www.unknowncheats.me/forum/counterstrike-global-offensive/261581-appealing-bone-esp.html
				// if (point is close enough to chest) use chest instead;
				if ((child  - chest).length_sqr() < 25) child  = chest;
				if ((parent - chest).length_sqr() < 25) parent = chest;

				vec3_t s_child, s_parent;
				if (!math::world_to_screen(child,  s_child))  continue;
				if (!math::world_to_screen(parent, s_parent)) continue;

				if (player == csgo::local_player && variables::Esp_Settings::enabledSkeleton[2])
					render::draw_line(s_child.x, s_child.y, s_parent.x, s_parent.y, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_skeleton[2]));
				if (player->team() == csgo::local_player->team() && variables::Esp_Settings::enabledSkeleton[1])
					render::draw_line(s_child.x, s_child.y, s_parent.x, s_parent.y, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_skeleton[1]));
				else if (player->team() != csgo::local_player->team() && variables::Esp_Settings::enabledSkeleton[0])
					render::draw_line(s_child.x, s_child.y, s_parent.x, s_parent.y, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_skeleton[0]));
			}
		}
		#pragma endregion

		#pragma region BOX ESP
		if (player == csgo::local_player && variables::Esp_Settings::enabledBox[2]) {
			render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(vfuns::getcolorofimcolor(variables::colors::enemy_color).a));
			render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black(vfuns::getcolorofimcolor(variables::colors::enemy_color).a));
			render::draw_rect(x, y, w, h, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_box[2]));
		}

		else if (player->team() == csgo::local_player->team() && variables::Esp_Settings::enabledBox[1]) {
			render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(vfuns::getcolorofimcolor(variables::colors::friendly_color).a));		// Outer box outline (Use inner color's opacity)
			render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black(vfuns::getcolorofimcolor(variables::colors::friendly_color).a));		// Inner box outline
			render::draw_rect(x, y, w, h, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_box[1]));	// Color box line
		}

		else if (player->team() != csgo::local_player->team() && variables::Esp_Settings::enabledBox[0]) {
			render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(vfuns::getcolorofimcolor(variables::colors::enemy_color).a));
			render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black(vfuns::getcolorofimcolor(variables::colors::enemy_color).a));
			render::draw_rect(x, y, w, h, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_box[0]));
		}

		#pragma endregion

		#pragma region LINE ESP
		if (variables::Esp_Settings::enabledLine[0] || variables::Esp_Settings::enabledLine[1] || variables::Esp_Settings::enabledLine[2]) {
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			
			// Draw from crosshair
			if (player == csgo::local_player)
			{}
			else if (player->team() == csgo::local_player->team() && variables::Esp_Settings::enabledLine[1])
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_line[1]));
			else if (player->team() != csgo::local_player->team() && variables::Esp_Settings::enabledLine[0])
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_line[0]));
		}
		#pragma endregion
		
		#pragma region NAME ESP
		if (variables::Esp_Settings::enabledNameesp[0] || variables::Esp_Settings::enabledNameesp[1] || variables::Esp_Settings::enabledNameesp[2]) {
			player_info_t playerinfo;
			interfaces::engine->get_player_info(i, &playerinfo);
			wchar_t w_player_name[128];
			if (MultiByteToWideChar(CP_UTF8, 0, playerinfo.name, -1, w_player_name, 128) < 0) continue;
			if (player == csgo::local_player && variables::Esp_Settings::enabledNameesp[2])
				render::draw_text_wchar(x + w / 2, y + h + 2, render::fonts::watermark_font, w_player_name, true, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_name[2]));
			else if (player->team() == csgo::local_player->team() && variables::Esp_Settings::enabledNameesp[1])
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_name[1]));
			else if (player->team() != csgo::local_player->team() && variables::Esp_Settings::enabledNameesp[0])
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, vfuns::getcolorofimcolorF(variables::Esp_Settings::colors_name[0]));
		}
		#pragma endregion

		#pragma region INFO ESP
		if (player_info_enabled && false) {
			// Friends and enemies
			if (player->team() != csgo::local_player->team() || variables::player_visuals::showteamesp) {
				if (variables::player_visuals::playerinfo_options[0].value && player->armor() > 0) {
					int armor_x = (variables::player_visuals::healthesp) ? 6 : 0;
					render::draw_text_string(x - 10 - armor_x, y + 1, render::fonts::watermark_font, "A", false, vfuns::getcolorofimcolor(variables::colors::friendly_color_softer));
				}

				// Has bomb
				bool has_bomb = false;
				if (variables::player_visuals::playerinfo_options[4].value) {
					const auto weapons = player->get_weapons();
					if (!weapons) return;
					for (int n = 0; weapons[n]; n++) {		// Iterate list of weapon handles
						weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapons[n]);
						if (weapon && weapon->is_bomb()) {
							has_bomb = true;
							break;
						}
					}
				}

				int item_num = 0;
				if (variables::player_visuals::playerinfo_options[3].value && player->is_defusing()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, color::blue(255));
					item_num++;
				} else if (variables::player_visuals::playerinfo_options[3].value && player->has_defuser()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, vfuns::getcolorofimcolor(variables::colors::friendly_color_softer));
					item_num++;
				} else if (variables::player_visuals::playerinfo_options[4].value && has_bomb) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "B", true, color(210, 110, 0, 255));
					item_num++;
				}

				if (variables::player_visuals::playerinfo_options[5].value && player->is_scoped()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "S", true, (player->is_defusing()) ? color::blue(255) : vfuns::getcolorofimcolor(variables::colors::friendly_color_softer));
					item_num++;
				}
				if (variables::player_visuals::playerinfo_options[6].value && player->is_flashed()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "F", true, color(255, 255, 0));
					item_num++;
				}
				if (variables::player_visuals::playerinfo_options[7].value && !aim::can_fire(player)) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "X", true, color(230, 210, 0, 255));
					item_num++;
				}

				// Weapon name/icon
				if (variables::player_visuals::playerinfo_options[0].value || variables::player_visuals::playerinfo_options[1].value) {
					auto current_weapon = player->active_weapon();
					if (!current_weapon) continue;

					int y_weapon = (variables::player_visuals::nameesp) ? 12 : 0;

					// Weapon icon
					if (variables::player_visuals::playerinfo_options[1].value) {
						const int idx = current_weapon->item_definition_index();
						render::draw_text_string(x + w / 2, y + h + y_weapon - 2, render::fonts::weapon_icon_font, helpers::idx2icon(idx), true, color::white());
					// We ignore text if we have icon, price of having a multicombobox for all the items
					} else if (variables::player_visuals::playerinfo_options[3].value) {
						auto weapon_data = current_weapon->get_weapon_data();
						if (!weapon_data) continue;
						std::string s_weapon_name = weapon_data->weapon_name;

						if (strstr(s_weapon_name.c_str(), "weapon_")) s_weapon_name.erase(s_weapon_name.begin(), s_weapon_name.begin() + 7);	// Remove "weapon_"

						const color weapon_name_col = (player->team() == csgo::local_player->team()) ? vfuns::getcolorofimcolor(variables::colors::friendly_color_softer) : vfuns::getcolorofimcolor(variables::colors::enemy_color_softer);
						render::draw_text_string(x + w / 2, y + h + 2 + y_weapon, render::fonts::watermark_font, s_weapon_name, true, weapon_name_col);
					}
				}
			}
		}
		#pragma endregion

		#pragma region HEALTH ESP
		if (variables::Esp_Settings::enabledHealthesp[0] || variables::Esp_Settings::enabledHealthesp[1] || variables::Esp_Settings::enabledHealthesp[2]) {
			int health = player->health();
			const int health_h = (h * health) / 100;
			const int health_w = 4;
			const int health_y = y + (h - health_h);
			const int health_x = x - 6;
			//render::draw_text_string(10, 20, render::fonts::watermark_font, std::to_string(h), true, color::red());
			if (player == csgo::local_player && variables::Esp_Settings::enabledHealthesp[2]) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			} else if (player->team() == csgo::local_player->team() && variables::Esp_Settings::enabledHealthesp[1]) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			} else if (player->team() != csgo::local_player->team() && variables::Esp_Settings::enabledHealthesp[0]) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			}
		}
		#pragma endregion

	}
}
