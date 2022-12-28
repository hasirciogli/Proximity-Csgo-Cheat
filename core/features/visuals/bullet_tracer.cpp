#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "dependencies/interfaces/interfaces.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/235892-bullet-tracer.html

void visuals::bullet_tracer(i_game_event* event) {
	auto index  = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(index));
	if (!entity || !entity->is_player()) return;
	if (!csgo::local_player) return;

	bool isLocalPlayer = false;
	bool isEnemy = false;
	bool isTeamMate = false;
	

	if (variables::misc_visuals::bulletracer_team_target[0] && entity == csgo::local_player)
		isLocalPlayer = true;
	else if (variables::misc_visuals::bulletracer_team_target[1] && entity->team() != csgo::local_player->team())
		isEnemy = true;
	else if (variables::misc_visuals::bulletracer_team_target[2])
		isTeamMate = true;
	else
		return;

	vec3_t hit = { event->get_float("x"), event->get_float("y"), event->get_float("z") };
	auto eyes = entity->get_eye_pos();

	ray_t ray;
	ray.initialize(eyes, hit);

	trace_filter filter;
	filter.skip = entity;
	
	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);		// Trace a ray

	constexpr auto time = 5.0f;
	//auto box_col = /*(tr.entity && tr.entity->is_player()) ? color(255, 0, 0, 100) :*/ line_col.col.get_custom_alpha(100);		// Removed because hit detection is buggy

	if (variables::misc_visuals::bulletracer_type == 0)
		interfaces::debug_overlay->add_line_overlay(eyes, hit, vfuns::getcolorofimcolor(variables::colors::bulletBeamColor[0]), false, time);

	//if (variables::misc_visuals::bulletracer_type == 0)
		
	interfaces::debug_overlay->add_box_overlay(hit, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), vec3_t(0, 0, 0), color::white(255), time);
}