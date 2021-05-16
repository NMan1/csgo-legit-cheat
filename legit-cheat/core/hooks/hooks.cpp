#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../../dependencies/utilities/settings.h"
#include "../menu/menu.h"

hooks::create_move::fn		create_move_original		= nullptr;
hooks::paint_traverse::fn	paint_traverse_original		= nullptr;
hooks::get_view_model::fn	get_view_model_original		= nullptr;
hooks::fsn::fn				fsn_original				= nullptr;

HWND						hooks::hCSGOWindow			= nullptr; 

unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }

bool hooks::initialize() 
{
	while (!(hooks::hCSGOWindow = FindWindowA("Valve001", nullptr)))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto get_view_model_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto fsn_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));

	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(fsn_target, &fsn::hook, reinterpret_cast<void**>(&fsn_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize fsn. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(get_view_model_target, &get_view_model::hook, reinterpret_cast<void**>(&get_view_model_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize get_view_model. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}	

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	interfaces::engine->get_screen_size(menu::screen_x, menu::screen_y);
	interfaces::console->get_convar("sv_cheats")->set_value(1);
	interfaces::console->get_convar("sv_pure_allow_loose_file_loads")->set_value(0);

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() 
{
	interfaces::input->m_fCameraInThirdPerson = false;
	Sleep(100);

	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) 
{
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!csgo::local_player)
		return create_move_original(input_sample_frametime, cmd);

	if (!csgo::local_player->is_alive())
		return create_move_original(input_sample_frametime, cmd);

	if (menu::config::rank_revealer && (cmd->buttons & in_score) != 0)
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);

	if (menu::config::bhop)
		features::misc::bunny_hop(cmd);

	if (menu::config::backtrack)
		features::aimbot::legit::save_ticks(csgo::local_player, cmd);

	if (menu::config::aimbot && GetAsyncKeyState(VK_LBUTTON))
		features::aimbot::legit::run(cmd);

	return false;
}

void __stdcall hooks::fsn::hook(int frame_stage)
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		fsn_original(interfaces::client, frame_stage);

	if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			features::skins::run();

	fsn_original(interfaces::client, frame_stage);
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) 
{
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) 
	{
	case fnv::hash("MatSystemTopPanel"):
		render::draw_text_string(1795, 5, render::fonts::watermark_font, "overflow", false, { 255, 50, 0 });

		if (interfaces::engine->is_connected() && interfaces::engine->is_in_game())
		{
			if (menu::config::visuals)
			{
				if (!csgo::local_player)
					break;

				if (menu::config::visuals)
				{
					features::visuals::render_visuals();
				}

				interfaces::console->get_convar("mat_postprocess_enable")->set_value(0);
			}
		}

		menu::render_menu();

		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

float __stdcall hooks::get_view_model::hook()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return 68.f;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player || !local_player->is_alive())
		return 68.f;

	if (!local_player->is_scoped())
	{
		if (menu::config::weapon_fov > 0)
			return menu::config::weapon_fov;
	}
	return 68.f;		
}
