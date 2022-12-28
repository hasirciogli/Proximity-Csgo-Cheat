#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/menu/menu.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"




HRESULT __stdcall hooks::end_scene::hook(LPDIRECT3DDEVICE9 pDevice) {
	static bool isSetupped = false;
	 
	if (!isSetupped)
	{
		isSetupped = true;

		ImGui::CreateContext();

		D3DDEVICE_CREATION_PARAMETERS creationParameters = { };
		pDevice->GetCreationParameters(&creationParameters);

		ImGui_ImplWin32_Init(creationParameters.hFocusWindow);
		ImGui_ImplDX9_Init(pDevice);


		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();
		static const ImWchar icons_ranges_Turkish[] = {
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x00c7, 0x00c7, // Ç
			0x00e7, 0x00e7, // ç
			0x011e, 0x011e, // Ð
			0x011f, 0x011f, // ð
			0x0130, 0x0130,// Ý
			0x0131, 0x0131, // ý
			0x00d6, 0x00d6, // Ö
			0x00f6, 0x00f6, // ö
			0x015e, 0x015e, // Þ
			0x015f, 0x015f, // þ
			0x00dc, 0x00dc, // Ü
			0x00fc, 0x00fc, // ü

			0, }; 
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18, 0, icons_ranges_Turkish);

		variables::Menu_Settings::fonts_GUIBIGFONT = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 150);
		variables::Menu_Settings::fonts_GUIBIGFONT2 = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 50);

	}

	if (GetAsyncKeyState(VK_XBUTTON2) & 1)
		variables::Menu_Settings::isOpened = !variables::Menu_Settings::isOpened;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	iXmenu::renderImguiBasedMenu(pDevice, variables::Menu_Settings::isOpened);


	ImGui::EndFrame();
	ImGui::Render();

	// render draw lists from draw data
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return hooks::end_scene::original(pDevice);
}