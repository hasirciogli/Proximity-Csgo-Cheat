#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/features/misc/backtrack.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

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
	if (!variables::chams::general_chams_enabled) return;

	const auto mdl = info.model;
	if (!mdl) return;

	#pragma region PLAYER
	if (strstr(mdl->name, "models/player") && (variables::chams::general_chams_enabled || variables::chams::backtrack_chams)) {
		const char* player_material = (variables::chams::enemy_chams_material < materials.size()) ? materials.at(variables::chams::enemy_chams_material) : materials.at(materials.size() - 1);

		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant()) return;

		if (player->has_gun_game_immunity()) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, false, color(255, 255, 255, 100), player_material);
		} else {
			// Backtrack chams
			if (variables::misc::backtrack && backtrack::records[player->index()].size() > 0 && variables::chams::backtrack_chams && (helpers::is_enemy(player) || variables::misc::backtrack_team)) {
				if (!variables::chams::enemy_chams || !player->is_moving())
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);	// Draw original player before backtrack if normal player chams are disabled. Probably a bad way of doing it

				// Colors, like chams, depend on team and not on is_enemy()
				const color chams_col = (player->team() == csgo::local_player->team()) ? vfuns::getcolorofimcolorF(variables::chams::chams_team_visible) : vfuns::getcolorofimcolorF(variables::chams::chams_enemy_visible);
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
			if (player == csgo::local_player && variables::misc::thirdperson && variables::chams::local_chams) {
				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				const float localplayer_col_a = (csgo::local_player->is_scoped()) ? 30 : variables::colors::chams_localplayer.Value.x;
				//colXMAN.a = localplayer_col_a;
				override_material(false, variables::chams::local_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_local_visible), materials[variables::chams::local_chams_material]);
			// Enemies (color depends on team, not is_enemy())
			} else if (variables::chams::enemy_chams && player->team() != csgo::local_player->team()) {
				player_material = (variables::chams::enemy_chams_material < materials.size()) ? materials.at(variables::chams::enemy_chams_material) : materials.at(materials.size() - 1);
				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				if (!variables::chams::enemy_only_visible) {
					const color invisible_chams_col = vfuns::getcolorofimcolorF(variables::chams::chams_enemy_invisible);	// So it uses the same alpha as normal col
					override_material(true, variables::chams::enemy_wireframe, invisible_chams_col, player_material);						// Not visible - Enemy
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}

				override_material(false, variables::chams::enemy_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_enemy_visible), player_material);			// Visible - Enemy
			// Friends
			} else if (variables::chams::team_chams && player != csgo::local_player) {
				player_material = (variables::chams::team_chams_material < materials.size()) ? materials.at(variables::chams::team_chams_material) : materials.at(materials.size() - 1);

				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);

				if (!variables::chams::team_only_visible) {
					const color invisible_chams_col = vfuns::getcolorofimcolorF(variables::chams::chams_team_invisible);	// So it uses the same alpha as normal col
					override_material(true, variables::chams::team_wireframe, invisible_chams_col, player_material);						// Not visible - Friendly
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}
				override_material(false, variables::chams::team_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_team_visible), player_material);		// Visible - Friendly
			}
		}
	}
	#pragma endregion

	#pragma region VIEWMODEL
	// Sleeve
	if (strstr(mdl->name, "sleeve")) {
		if (variables::chams::sleeve_chams) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::sleeve_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_sleeve_visible), materials.at(variables::chams::sleeve_chams_material));
		}
	// Arms
	} else if (strstr(mdl->name + 17, "arms")) {
		// Remove normal arms if we have a custom model and alive
		if (csgo::local_player->is_alive() && skins::custom_models.find(ARMS) != skins::custom_models.end() && strstr(csgo::local_player->arms_model(), skins::custom_models.at(ARMS).worldmodel.c_str())) {
			override_material(false, variables::chams::arm_wireframe, color{ 0,0,0,0 }, materials.at(1));
		// Normal arms
		} else if (variables::chams::arm_chams) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::arm_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_arm_visible), materials.at(variables::chams::arm_chams_material));
		}
	// Viewmodel weapon
	} else if (strstr(mdl->name, "models/weapons/v")) {
		if (variables::chams::weapon_chams && !csgo::local_player->is_scoped()) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::weapon_wireframe, vfuns::getcolorofimcolorF(variables::chams::chams_weapon_visible), materials.at(variables::chams::weapon_chams_material));
		}
	}
	#pragma endregion

	hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
}
