#include "../features.hpp"
#include "../../../source-sdk/math/vector3d.hpp"
#include "../../menu/menu.h"

void features::visuals::render_visuals()
{
	if (!csgo::local_player)
		return;

	for (int i = 1; i < interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player)
			continue;

		if (!entity->is_player() || !entity->is_alive() || entity->team() == csgo::local_player->team() || entity->dormant())
			continue;

		vec3_t screen_origin, origin = entity->origin();
		if (!math::world_to_screen(origin, screen_origin))
			continue;

		vec3_t screen_head, head = entity->flags() & fl_ducking ? origin + vec3_t{ 0, 0, 54 } : origin + vec3_t{ 0, 0, 72 };
		if (!math::world_to_screen(head, screen_head))
			continue;

		int x = screen_head.x;
		int y = screen_head.y;
		int h = screen_origin.y - y;
		int w = h / 1.5;

		render::draw_corner_box(x - (w / 2), y, w, h, { 255, 192, 203 });

		if (menu::config::show_backtrack && menu::config::backtrack) {
			vec3_t last_pos = {};
			bool loop = false;
			for (auto& tick : features::aimbot::legit::player_ticks[i]) {
				vec3_t screen_pos = {};
				if (!math::world_to_screen(tick.head_position, screen_pos))
					continue;

				if (loop) {
					render::draw_line(last_pos.x, last_pos.y, screen_pos.x, screen_pos.y, { 255, 255, 0 });
				}

				last_pos = screen_pos;
				loop = true;
			}
		}
	}
}