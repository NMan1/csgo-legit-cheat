#pragma once
#include "../../dependencies/common_includes.hpp"

class c_view_setup;
class i_net_message;

namespace hooks 
{
	bool initialize();
	void release();

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}	

	namespace get_view_model {
		using fn = float(__thiscall*)();
		float __stdcall hook();
	}

	namespace fsn {
		using fn = void(__thiscall*)(void* _this, int);
		void __stdcall hook(int frame_stage);
	}	

	extern HWND                           hCSGOWindow; // CSGO window handle
	extern bool							  initialized_drawManager;
}