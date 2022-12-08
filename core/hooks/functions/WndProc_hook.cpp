#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hooks::WndProc_hook::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// Get inputs from user and store them. Same as we did in paint_traverse
	input::global_input.WndProcUpdate(msg, wparam, lparam);

	if (true && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	return CallWindowProcW(original, hwnd, msg, wparam, lparam);
}