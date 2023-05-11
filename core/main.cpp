#define WIN32_LEAN_AND_MEAN

#define no_server_is_debug_mode_fuck false

#include "socket/msoket.h"

#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "dependencies/utilities/renderer/renderer.hpp"

#include <nlohmann/json.hpp>
#include <steam/steam_api.h>
#include <steam/isteamuser.h>
#include <steam/isteamfriends.h>

#pragma comment(lib, "steam_api64.lib")
#pragma comment(lib, "steam_api.lib")


#include <clocale>
#include <TlHelp32.h>
#include <bcrypt.h>

using namespace nlohmann::json_abi_v3_11_2;

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll") && !GetModuleHandleA("steam_api.dll")) 
		Sleep(150);
	setlocale(LC_ALL, "Turkish");
	SteamAPI_Init();

#ifdef _DEBUG
	console::initialize("Proximity console");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		hooks::initialize();
	}
	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "Proximity error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}


#if _DEBUG
	// VK_END to unhook. We use IsHeld() because don't need to check the "first frame"
	while (!input::global_input.IsHeld(VK_END) && !variables::cheat::forceCloseCheat)
	{
		if (false)
		{
			player_info_t lpInfo_t;
			if (csgo::local_player)
			{
				interfaces::engine->get_player_info(csgo::local_player->index(), &lpInfo_t);

				console::log(std::to_string(lpInfo_t.xuid).c_str());
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
#else
	// VK_END to unhook. We use IsHeld() because don't need to check the "first frame"
	while (!variables::cheat::forceCloseCheat)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif

	
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
#ifndef _DEBUG
	SteamAPI_Shutdown();
#endif
	return TRUE;
}

/*
__forceinline std::uintptr_t ntapihide()
{
	using NtSetInformationThreadFn = NTSTATUS(NTAPI*)
		(
			HANDLE ThreadHandle,
			ULONG  ThreadInformationClass,
			PVOID  ThreadInformation,
			ULONG  ThreadInformationLength
			);

	const ULONG hide_thread_from_debugger = 0x11;

	NtSetInformationThreadFn NtSetInformationThread = (NtSetInformationThreadFn)GetProcAddress
	(
		GetModuleHandleA("ntdll.dll"),
		"NtSetInformationThread"
	);

	NTSTATUS status = NtSetInformationThread
	(
		(HANDLE)0xFFFFFFFE,
		hide_thread_from_debugger,
		NULL,
		NULL
	);
	//status suc (starshinee Mar 16, 2021)
	if (status != 0x0)
	{
		std::printf("NtSetInformationThread | 0x0\n\n");
		std::printf("Starting execute syscall\n");

		const auto Wow64AllocMemory = VirtualAlloc
		(
			0x0,
			0x1000,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE
		);
		//asm xmm0 xmword ptr ds : [edx] //burayi kapat bro
		__asm
		{
			mov edx, dword ptr fs : [0xC0]
			movups xmm0, xmmword ptr ds : [edx]
			mov eax, dword ptr ds : [Wow64AllocMemory]
			mov dword ptr ss : [ebp - 0x8] , eax
			movups xmmword ptr ds : [eax] , xmm0
		}

		__asm
		{
			push 0x0
			push 0x0
			push 0x11
			push 0xFFFFFFFE

			call $ + 5

			mov eax, 0xD

			call dword ptr ds : [Wow64AllocMemory]
		}

		return status;
	}

	std::printf("NtSetInformationThread | 0x1\n");

	return status;
}


void crash()
{
	__asm
	{
		rdtsc
		XOR edx, eax
		add eax, edx
		mov esp, eax
		XOR ebp, edx
		mov ebx, ebp
		mov ecx, esp
		XOR esi, ebx
		XOR edi, esp
		jmp eax
	}
}

__forceinline std::uintptr_t k0x85()
{
	using NtQueryInformationProcessFn = NTSTATUS(NTAPI*)
		(
			HANDLE           ProcessHandle,
			UINT ProcessInformationClass,
			PVOID            ProcessInformation,
			ULONG            ProcessInformationLength,
			PULONG           ReturnLength
			);

	const UINT debug_flags = 0x7;
	DWORD is_dbg = 0x0;

	NtQueryInformationProcessFn NtQueryInformationProcess = (NtQueryInformationProcessFn)GetProcAddress
	(
		GetModuleHandleA("ntdll.dll"),
		"NtQueryInformationProcess"
	);

	NTSTATUS status = NtQueryInformationProcess
	(
		(HANDLE)0xFFFFFFFF,
		debug_flags,
		&is_dbg,
		sizeof(DWORD),
		NULL
	);

	if (status == 0x0 && is_dbg != 0x0)
	{
		std::printf("NtQueryInformationProcess | 0x1\n");
		return status;
	}

	std::printf("NtQueryInformationProcess | 0x0\n\n");
	std::printf("Starting execute syscall\n\n");

	const auto Wow64AllocMemory = VirtualAlloc
	(
		0x0,
		0x1000,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	__asm
	{
		mov edx, dword ptr fs : [0xC0]
		movups xmm0, xmmword ptr ds : [edx]
		mov eax, dword ptr ds : [Wow64AllocMemory]
		mov dword ptr ss : [esi + 0x8] , eax
		movups xmmword ptr ds : [eax] , xmm0
	}

	__asm
	{
		push 0x0
		push 0x4
		lea ecx, dword ptr ss : [is_dbg]
		push ecx
		push 0x7
		push 0xFFFFFFFF

		call $ + 5

		mov eax, 0x19

		call dword ptr ds : [Wow64AllocMemory]
	}

	if (is_dbg != 0x0)  //succes IDA x64 , x64dbg , x32dbg checked (strshn)
	{
		MessageBox(NULL, "DRIVERERR", "WND_NAME", MB_OKCANCEL);
		//globals.whileloop = false;
		crash();
	}
	else
	{
		std::printf("NtQueryInformationProcess Syscall | 0x0\n");
	}

	return status;
}

*/

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	//int fuck_you = strlen((const char*)fuck_skids);	// Need to call var so shit ass compiler does not ignore it
	
	DisableThreadLibraryCalls(instance);

	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			//ntapihide();
			setlocale(LC_ALL, "Turkish");
			if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
				CloseHandle(handle);

#if no_server_is_debug_mode_fuck == true

			/*mSocket::cfg::socketIsConnected = true;
			mSocket::cfg::authed = true;

			variables::NetworkUser::username = "admin";*/

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
