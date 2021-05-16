#pragma once
#include "../../dependencies/common_includes.hpp"

namespace menu {
	extern int screen_x, screen_y;

	namespace config {
		extern bool aimbot;
		extern int fov;
		extern int smoothing;
		extern bool backtrack;
		extern bool rank_revealer;
		extern bool bhop;
		extern bool visuals;
		extern bool show_backtrack;
		extern int weapon_fov;
	}

	extern bool menu_open;

	void render_menu();

	void handle_controls();
}