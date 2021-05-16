#include "menu.h"
#include "..\features\features.hpp"
#include <vector>

namespace menu {
	int screen_x = 1920, screen_y = 1080;

	namespace config {
		bool aimbot = true;
		int fov = 4;
		int smoothing = 1;
		bool backtrack = true;
		bool rank_revealer = true;
		bool bhop = false;
		bool visuals = true;
		bool show_backtrack = true;
		int weapon_fov = 70;
	}

	enum item_type {
		SECTION, SWITCH, SLIDER
	};
	
	struct menu_item {
		menu_item(std::string title, void* value = 0, item_type type = SWITCH, int min_value = 0, int max_value = 1) {
			this->title = title;
			this->value = value;
			this->type = type;
			this->min_value = min_value;
			this->max_value = max_value;
		}

		std::string title;
		void* value;
		item_type type;
		int min_value;
		int max_value;
	};

	std::vector<menu_item> items{};
	int x, y, w, h;
	int current_selection = 0;
	bool menu_open = false;
	

	void add_items() {
		static bool section_one = true, section_two = true, section_three = true, section_four = true;

		items.push_back({ "legit", &section_one, SECTION });
		if (section_one) {
			items.push_back({ "aimbot", &menu::config::aimbot });
			items.push_back({ "fov", &menu::config::fov, SLIDER, 1, 90 });
			items.push_back({ "smoothing", &menu::config::smoothing, SLIDER, 0, 10 });
			items.push_back({ "backtrack", &menu::config::backtrack });
		}
		
		items.push_back({ "visuals", &section_two, SECTION });
		if (section_two) {
			items.push_back({ "esp", &menu::config::visuals });
			items.push_back({ "show backtrack", &menu::config::show_backtrack });
		}
			
		items.push_back({ "misc", &section_three, SECTION });
		if (section_three) {
			items.push_back({ "bhop", &menu::config::bhop });
			items.push_back({ "rank revealer", &menu::config::rank_revealer });
			items.push_back({ "weapon fov", &menu::config::weapon_fov, SLIDER, 0, 120 });
		}
	}

	void render_items() {
		for (int i = 0; i < items.size(); i++) {
			auto& item = items[i];

			if (item.type == SECTION)
				render::draw_text_string(x + 3, y + 15 + (i * 15), render::fonts::menu_font, item.title, false, { 255, 255, 255 });
			else if (item.type == SWITCH) {
				render::draw_text_string(x + 8, y + 15 + (i * 15), render::fonts::menu_font, item.title, false, { 170, 170, 170 });
				render::draw_text_string_menu(x + 8 + 189, y + 15 + (i * 15), render::fonts::menu_font, *(bool*)item.value ? "on" : "off", true, *(bool*)item.value ? color{ 10, 220, 10 } : color{ 220, 10, 10 });
			}
			else if (item.type == SLIDER) {
				render::draw_text_string(x + 8, y + 15 + (i * 15), render::fonts::menu_font, item.title, false, { 170, 170, 170 });
				render::draw_text_string_menu(x + 8 + 189, y + 15 + (i * 15), render::fonts::menu_font, std::to_string(*(int*)item.value), true, { 10, 220, 10 });
			}
		}
	}

	void handle_controls() {
		if (GetAsyncKeyState(VK_UP) & 1) {
			if (current_selection == 0)
				current_selection = items.size();

			current_selection--;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 1) {
			if (current_selection == items.size() - 1)
				current_selection = -1;

			current_selection++;
		}
		else if (GetAsyncKeyState(VK_LEFT) & 1) {
			auto& item = items.at(current_selection);
			if (item.type == SECTION || item.type == SWITCH)
				*(bool*)item.value = !*(bool*)item.value;
			else if (item.type == SLIDER) {
				if (*(int*)item.value != item.min_value)
					*(int*)item.value -= 1;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 1) {
			auto& item = items.at(current_selection);
			if (item.type == SECTION || item.type == SWITCH)
				*(bool*)item.value = !*(bool*)item.value;
			else if (item.type == SLIDER) {
				if (*(int*)item.value != item.max_value)
					*(int*)item.value += 1;
			}
		}

	}

	void render_menu() {
		if (GetAsyncKeyState(VK_INSERT) & 1 || GetAsyncKeyState(VK_RMENU) & 1)
			menu_open = !menu_open;

		if (!menu_open)
			return;

		w = 200, h = 15;
		x = screen_x / 2 - (w / 2), y = screen_y / 3;

		add_items();
		h += items.size() * 15;

		render::draw_filled_rect(x, y, w, h, { 30, 30, 30 });
		render::draw_outline(x, y, w, h, { 0, 0, 0 });

		render::draw_filled_rect(x, y, w, 15, { 25, 25, 25 });
		render::draw_outline(x, y, w, 15, { 0, 0, 0 });
		render::draw_text_string(x + (w / 2), y + 1, render::fonts::menu_font, "overflow", true, { 255, 50, 0 });

		render::draw_filled_rect(x + 1, y + 15 + (current_selection * 15), w - 2, 15 - 1, { 50, 50, 50, 127 });

		render_items();
		handle_controls();
		items.clear();
	}
}