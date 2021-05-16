#pragma once
#include "../../dependencies/common_includes.hpp"

namespace features 
{
	namespace aimbot 
	{
		namespace legit 
		{
			void run(c_usercmd* user_cmd);
			void save_ticks(player_t* local_player, c_usercmd* cmd);
			void save_tick_entity(int entity_index, c_usercmd* cmd);
			void set_backtrack_tick(player_t* local_player, c_usercmd* cmd);
            void set_backtrack_tick_entity(int entity_index, c_usercmd* cmd);

			struct saved_ticks {
				saved_ticks(int tick_number, vec3_t head_positon) {
					this->tick_number = tick_number;
					this->head_position = head_positon;
				}

				int tick_number = -1;
				vec3_t head_position = { 0, 0, 0 };
			};
			extern std::vector<saved_ticks> player_ticks[64];
		};
	};

	namespace misc 
	{
		void bunny_hop(c_usercmd* cmd);
	};

	namespace visuals 
	{
		void render_visuals();
	};

	namespace skins 
	{
		void run();
		void load();
		extern std::map <int, std::vector<std::tuple<std::string, std::string, int>>> weapon_skin_quality_list;
		void get_weapon_skins();
	}
}