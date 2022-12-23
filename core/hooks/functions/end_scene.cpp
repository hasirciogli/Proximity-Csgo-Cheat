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

		//add font's Etc

		ImFont* ff = io.Fonts->AddFontFromFileTTF("C://Windows/Fonts/Candara.ttf", 13);
		ImFont* fontVerdana = io.Fonts->AddFontFromFileTTF("C://Windows/Fonts/Verdana.ttf", 13);
		io.FontDefault = fontVerdana;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18);

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