#include "../../../../dependencies/common_includes.hpp"
#include "../../features.hpp"
#include "../../../menu/menu.h"

std::vector<features::aimbot::legit::saved_ticks> features::aimbot::legit::player_ticks[64];

void features::aimbot::legit::save_ticks(player_t* local_player, c_usercmd* cmd) {
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = (player_t*)interfaces::entity_list->get_client_entity(i);
		if (!entity || entity->team() == local_player->team()) {
			continue;
		}

		if (!entity->is_alive()) {
			player_ticks[i].clear();
			return;
		}

		player_ticks[i].insert(player_ticks[i].begin(), { cmd->tick_count, entity->get_hitbox_position(hitboxes::hitbox_head) });
		if (player_ticks[i].size() > 12) {
			player_ticks[i].pop_back();
		}
	}
}

void features::aimbot::legit::save_tick_entity(int entity_index, c_usercmd* cmd) {
	auto entity = (player_t*)interfaces::entity_list->get_client_entity(entity_index);
	if (!entity) {
		return;
	}

	if (!entity->is_alive()) {
		player_ticks[entity_index].clear();
		return;
	}

	player_ticks[entity_index].insert(player_ticks[entity_index].begin(), { cmd->tick_count, entity->get_hitbox_position(hitboxes::hitbox_head) });
	if (player_ticks[entity_index].size() > 12) {
		player_ticks[entity_index].pop_back();
	}
}

void features::aimbot::legit::set_backtrack_tick(player_t* local_player, c_usercmd* cmd) {
	auto max_delta = FLT_MAX;
	auto best_index = -1;
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = (player_t*)interfaces::entity_list->get_client_entity(i);
		if (!entity || !entity->health() || entity->team() == local_player->team()) {
			continue;
		}

		float delta = (math::calculate_angle_alternative(local_player->get_eye_pos(), entity->get_eye_pos()) - cmd->viewangles).length();
		if (delta < max_delta) {
			best_index = i;
			max_delta = delta;
		}
	}

	if (best_index == -1) {
		return;
	}

	int best_tick = -1;
	max_delta = FLT_MAX;
	for (int i = 0; i < player_ticks[best_index].size(); i++) {
		float delta = (math::calculate_angle_alternative(local_player->get_eye_pos(), player_ticks[best_index].at(i).head_position) - cmd->viewangles).length();
		if (delta < max_delta) {
			max_delta = delta;
			best_tick = i;
		}
	}

	if (best_tick != -1 && cmd->buttons & in_attack) {
		cmd->tick_count = player_ticks[best_index].at(best_tick).tick_number;
	}
}

void features::aimbot::legit::set_backtrack_tick_entity(int entity_index, c_usercmd* cmd) {
	int best_tick = -1;
	auto max_delta = FLT_MAX;
	for (int i = 0; i < player_ticks[entity_index].size(); i++) {
		float delta = (math::calculate_angle_alternative(csgo::local_player->get_eye_pos(), player_ticks[entity_index].at(i).head_position) - cmd->viewangles).length();
		if (delta < max_delta) {
			max_delta = delta;
			best_tick = i;
		}
	}

	if (best_tick != -1 && cmd->buttons & in_attack) {
		cmd->tick_count = player_ticks[entity_index].at(best_tick).tick_number;
	}
}
