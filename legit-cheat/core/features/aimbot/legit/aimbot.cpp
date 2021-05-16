#include "../../../../dependencies/common_includes.hpp"
#include "../../features.hpp"
#include "../../../menu/menu.h"

int get_nearest_bone(player_t* entity, c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return false;

	float best_dist = 360.f;
	int aimbone;

	matrix_t matrix[MAXSTUDIOBONES];

	if (!entity->setup_bones(matrix, 128, BONE_USED_BY_HITBOX, 0.0f))
		return -1;

	studio_hdr_t* studio_model = interfaces::model_info->get_studio_model(entity->model());
	if (!studio_model)
		return -1;

	studio_hitbox_set_t* set = studio_model->hitbox_set(entity->hitbox_set());
	if (!set)
		return -1;

	for (int i = 0; i < set->hitbox_count; i++) {
		if (i >= hitbox_max)
			continue;

		studio_box_t* hitbox = set->hitbox(i);

		if (!hitbox)
			continue;

		auto angle = math::calculate_angle(local_player->get_eye_pos(), vec3_t(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3]), user_cmd->viewangles);
		auto this_dist = std::hypotf(angle.x, angle.y);

		if (best_dist > this_dist) {
			best_dist = this_dist;
			aimbone = hitbox->bone;
			continue;
		}
	}
	return aimbone;
}

player_t* closest_to_crosshair(c_usercmd* user_cmd)
{
	player_t* best_entity = nullptr;
	float delta;
	float best_delta = FLT_MAX;
	int best_index = -1;

	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto view_angles = user_cmd->viewangles;
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player)
			continue;

		if (entity->has_gun_game_immunity() || entity->dormant() || !entity->is_alive() || !entity->is_player() || entity->health() <= 0)
			continue;

		if (entity->team() == csgo::local_player->team())
			continue;

		if (!entity->is_visible(entity))
			continue;

		delta = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_eye_pos(), view_angles).length();
		if (delta < best_delta && delta < menu::config::fov)
		{
			best_entity = entity;
			best_delta = delta;
			best_index = i;
		}
	}

	if (best_index != -1 && menu::config::backtrack)
		features::aimbot::legit::set_backtrack_tick_entity(best_index, user_cmd);

	return best_entity;
}

void features::aimbot::legit::run(c_usercmd* user_cmd) 
{
	if (!csgo::local_player->is_alive())
		return;

	const auto weapon_type = csgo::local_player->active_weapon()->get_weapon_data()->m_iWeaponType;
	if (weapon_type == WEAPONTYPE_GRENADE || weapon_type == WEAPONTYPE_C4 || weapon_type == WEAPONTYPE_KNIFE)
		return;

	player_t* entity = closest_to_crosshair(user_cmd); 
	if (!entity)
		return;

	int bone = get_nearest_bone(entity, user_cmd);
	auto angle = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_bone_position(bone), user_cmd->viewangles);

	if (csgo::local_player->shots_fired() > 1 && weapon_type != WEAPONTYPE_PISTOL)
		angle -= csgo::local_player->aim_punch_angle() * 2;

	angle = angle.clamp();

	if (menu::config::smoothing != 0)
		angle /= (menu::config::smoothing * 5);

	angle = math::normalize(angle);

	user_cmd->viewangles += angle;

	interfaces::engine->set_view_angles(user_cmd->viewangles);
}