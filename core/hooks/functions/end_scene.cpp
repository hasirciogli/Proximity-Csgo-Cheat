#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/menu/menu.hpp"
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
	}


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (variables::ui::menu::opened)
	{
		menu::renderImguiBasedMenu();
	}


	ImGui::EndFrame();
	ImGui::Render();

	// render draw lists from draw data
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return hooks::end_scene::original(pDevice);
}