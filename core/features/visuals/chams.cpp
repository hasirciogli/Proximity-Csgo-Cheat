#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/features/misc/backtrack.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

using namespace ens;

std::vector<const char*> materials = {
	"vgui/screens/transparent",														// "Transparent"
	"debug/debugambientcube",														// "Textured"
	"debug/debugdrawflat",															// "Flat"
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",			// "Ghost"
	"models/inventory_items/trophy_majors/gold",									// "Gold 1"
	"models/inventory_items/wildfire_gold/wildfire_gold_detail",					// "Gold 2"
	"models/inventory_items/trophy_majors/crystal_blue",							// "Amethyst"
	"models/inventory_items/trophy_majors/crystal_clear",							// "Silver"
	"models/player/ct_fbi/ct_fbi_glass",											// "Steel"
	"models/gibs/glass/glass",														// "Dark steel"
	"models/inventory_items/trophy_majors/gloss",									// "Plastic"
	"dev/glow_rim3d",																// "Red glow"
	"models/inventory_items/dreamhack_trophies/dreamhack_star_blur",				// "Effect 1"
	"models/inventory_items/dreamhack_trophies/dreamhack_pickem_glow_gold",			// "Effect 2"
	"models/inventory_items/dogtags/dogtags_lightray",								// "Effect 3"
	"models/inventory_items/dogtags/dogtags_outline",								// "Animated blink"
	"models/inventory_items/music_kit/darude_01/mp3_detail"							// "Animated stripes"
};

void override_material(bool ignorez, bool wireframe, const color& rgba, const char* mat_name) {
	auto material = interfaces::material_system->find_material(mat_name, TEXTURE_GROUP_MODEL);

	interfaces::render_view->set_blend(1.f);
	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);
	material->alpha_modulate(rgba.a / 255.f);
	material->color_modulate(rgba.r / 255.f, rgba.g / 255.f, rgba.b / 255.f);
	interfaces::model_render->override_material(material);
}

// Used in draw_model_execute
void visuals::draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!csgo::local_player) return;

	const auto mdl = info.model;
	if (!mdl) return;

	#pragma region PLAYER
	if (strstr(mdl->name, "models/player") && (variables::Chams_Settings::chams_enableds[0] || variables::Chams_Settings::chams_enableds[1] || variables::Chams_Settings::chams_enableds[2] || variables::Chams_Settings::backtrack_chams)) {
		const char* player_material = (variables::Chams_Settings::chams_materialids[0] < materials.size()) ? materials.at(variables::Chams_Settings::chams_materialids[0]) : materials.at(materials.size() - 1);

		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant()) return;

		if (player->has_gun_game_immunity()) {
			if (variables::Chams_Settings::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, false, color(255, 255, 255, 100), player_material);
		} else {
			// Backtrack Chams_Settings
			if (csgo::local_player == player)
			{
				
			}
			else if (variables::misc::backtrack && backtrack::records[player->index()].size() > 0 && variables::Chams_Settings::backtrack_chams[0] && (helpers::is_enemy(player) || variables::Chams_Settings::backtrack_chams[0] || variables::Chams_Settings::backtrack_chams[1] || variables::Chams_Settings::backtrack_chams[2])) {
				if (!variables::Chams_Settings::chams_enableds[0] || !player->is_moving())
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);	// Draw original player before backtrack if normal player Chams_Settings are disabled. Probably a bad way of doing it

				// Colors, like Chams_Settings, depend on team and not on is_enemy()
				const color chams_col = (player->team() == csgo::local_player->team()) ? vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[ENEMY][VICIBLE]) : vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[ENEMY][VICIBLE]);
				//const color chams_col_fade = (player->team() == csgo::local_player->team()) ? vfuns::getcolorofimcolor(variables::colors::bt_chams_friend_fade) : vfuns::getcolorofimcolor(variables::colors::bt_chams_enemy_fade);
				for (uint32_t i = 0; i < backtrack::records[player->index()].size(); i++) {
					if (!backtrack::valid_tick(backtrack::records[player->index()][i].simulation_time)
						|| backtrack::records[player->index()][i].matrix == nullptr)
						continue;

					//override_material(false, false, color::interpolate(chams_col, chams_col_fade, std::clamp(1 * (i) / (64.f), 0.f, 1.f) * 5), materials[1]);
					override_material(false, false, chams_col, materials[1]);
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, backtrack::records[player->index()][i].matrix);		// Use backtrack's matrix
				}
				interfaces::model_render->override_material(nullptr);		// Reset material to avoid replacing the player material itself
			}

			// For thirdperson
			if (player == csgo::local_player && variables::misc::thirdperson && variables::Chams_Settings::chams_enableds[2]) {
				if (variables::Chams_Settings::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				const float localplayer_col_a = (csgo::local_player->is_scoped()) ? 30 : variables::Chams_Settings::player_colors_chams[LOCAL][VICIBLE][3];
				//colXMAN.a = localplayer_col_a;
				override_material(false, variables::Chams_Settings::chams_wireframes[2], vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[LOCAL][VICIBLE]), materials[variables::Chams_Settings::chams_materialids[2]]);
			// Enemies (color depends on team, not is_enemy())
			} else if (variables::Chams_Settings::chams_enableds[0] && player->team() != csgo::local_player->team()) {
				player_material = (variables::Chams_Settings::chams_materialids[0] < materials.size()) ? materials.at(variables::Chams_Settings::chams_materialids[0]) : materials.at(materials.size() - 1);
				if (variables::Chams_Settings::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				if (!variables::Chams_Settings::chams_onlyvisibleds[0]) {
					const color invisible_chams_col = vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[ENEMY][INVICIBLE]);
					// So it uses the same alpha as normal col
					override_material(true, variables::Chams_Settings::chams_wireframes[0], invisible_chams_col, player_material);						// Not visible - Enemy
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}

				override_material(false, variables::Chams_Settings::chams_wireframes[0], vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[ENEMY][VICIBLE]), player_material);			// Visible - Enemy
			// Friends
			} else if (variables::Chams_Settings::chams_enableds[1] && player != csgo::local_player) {
				player_material = (variables::Chams_Settings::chams_materialids[1] < materials.size()) ? materials.at(variables::Chams_Settings::chams_materialids[1]) : materials.at(materials.size() - 1);

				if (variables::Chams_Settings::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				if (!variables::Chams_Settings::chams_onlyvisibleds[1]) {
					const color invisible_chams_col = vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[TEAM][INVICIBLE]);	// So it uses the same alpha as normal col
					override_material(true, variables::Chams_Settings::chams_wireframes[1], invisible_chams_col, player_material);						// Not visible - Friendly
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}
				override_material(false, variables::Chams_Settings::chams_wireframes[1], vfuns::getcolorofimcolorF(variables::Chams_Settings::player_colors_chams[TEAM][VICIBLE]), player_material);		// Visible - Friendly
			}
		}
	}
	#pragma endregion

	#pragma region VIEWMODEL
	// Sleeve
	if (strstr(mdl->name, "sleeve")) {
		if (variables::Chams_Settings::sleeve_chams) {
			if (variables::Chams_Settings::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::Chams_Settings::sleeve_wireframe, vfuns::getcolorofimcolorF(variables::Chams_Settings::colors_sleeve), materials.at(variables::Chams_Settings::sleeve_chams_material));
		}
	// Arms
	} else if (strstr(mdl->name + 17, "arms")) {
		// Remove normal arms if we have a custom model and alive
		if (csgo::local_player->is_alive() && skins::custom_models.find(ARMS) != skins::custom_models.end() && strstr(csgo::local_player->arms_model(), skins::custom_models.at(ARMS).worldmodel.c_str())) {
			override_material(false, variables::Chams_Settings::arm_wireframe, color{ 0,0,0,0 }, materials.at(1));
		// Normal arms
		} else if (variables::Chams_Settings::arm_chams) {
			if (variables::Chams_Settings::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::Chams_Settings::arm_wireframe, vfuns::getcolorofimcolorF(variables::Chams_Settings::colors_arm), materials.at(variables::Chams_Settings::arm_chams_material));
		}
	// Viewmodel weapon
	} else if (strstr(mdl->name, "models/weapons/v")) {
		if (variables::Chams_Settings::weapon_chams && !csgo::local_player->is_scoped()) {
			if (variables::Chams_Settings::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::Chams_Settings::weapon_wireframe, vfuns::getcolorofimcolorF(variables::Chams_Settings::colors_weapon), materials.at(variables::Chams_Settings::weapon_chams_material));
		}
	}
	#pragma endregion

	hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
}
