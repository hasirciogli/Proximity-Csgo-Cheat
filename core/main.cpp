#define WIN32_LEAN_AND_MEAN
#define no_server_is_debug_mode_fuck true
#include "socket/msoket.h"

#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "dependencies/utilities/renderer/renderer.hpp"

#include <nlohmann/json.hpp>


using namespace nlohmann::json_abi_v3_11_2;

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(150);

#ifdef _DEBUG
	console::initialize("Rogsoftware console");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		hooks::initialize();
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "Rogsoftware error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	// VK_END to unhook. We use IsHeld() because don't need to check the "first frame"
	while (!input::global_input.IsHeld(VK_END) && !variables::cheat::forceCloseCheat)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	//close menu so input is restored to user in the hooks::paint_traverse::hook hook.
	variables::Menu_Settings::isOpened = false;
	variables::Menu_Settings::isInitialized = true;


	mSocket::cfg::closingTO = true;
	mSocket::cfg::socketReconnect = false;
	mSocket::cfg::socketIsConnected = false;

	mSocket::cleanup();

	//wait for paint_traverse::hook to be called and restore input.
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();
#ifdef _DEBUG
	console::release();
#endif
	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	//int fuck_you = strlen((const char*)fuck_skids);	// Need to call var so shit ass compiler does not ignore it
	
	DisableThreadLibraryCalls(instance);

	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
				CloseHandle(handle);

#if no_server_is_debug_mode_fuck == true

			mSocket::cfg::socketIsConnected = true;
			mSocket::cfg::authed = true;

			variables::NetworkUser::username = "admin";

#else
			if (auto handle = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)mSocket::socketThread, instance, NULL, nullptr))
				CloseHandle(handle);
#endif




			
			break;
		}

		case DLL_PROCESS_DETACH: {
			release();
			break;
		}
	}

	return true;
}
