#include "../dependencies/common_includes.hpp"
#include "features/features.hpp"
#include "features/skins/parser.h"

DWORD WINAPI initialize(void* instance) {
	//console::initialize("Overflow Console");

	try {
		interfaces::initialize();
		kit_parser::setup();
		features::skins::load();
		hooks::initialize();
		render::initialize();
	}
	catch (const std::runtime_error & error) {
		MessageBoxA(NULL, error.what(), "Overflow - Error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	hooks::release();

	Sleep(1000);
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 1);
}

BOOL APIENTRY DllMain(HMODULE instance, uintptr_t reason, void* reserved)  {
	switch (reason) 
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(instance);
		if (auto h = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(h);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return true;
}
