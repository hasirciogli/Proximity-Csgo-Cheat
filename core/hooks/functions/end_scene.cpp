#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/menu/menu.hpp"
#include "core/hooks/hooks.hpp"
#include "bytes/LeftBarIcons.h"


#pragma comment(lib, "d3d9.lib")


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
		static const ImWchar icons_ranges_Turkish[] = 
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x00c7, 0x00c7, // �
			0x00e7, 0x00e7, // �
			0x011e, 0x011e, // �
			0x011f, 0x011f, // �
			0x0130, 0x0130,// �
			0x0131, 0x0131, // �
			0x00d6, 0x00d6, // �
			0x00f6, 0x00f6, // �
			0x015e, 0x015e, // �
			0x015f, 0x015f, // �
			0x00dc, 0x00dc, // �
			0x00fc, 0x00fc, // �

			0,
		};
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18, 0, icons_ranges_Turkish);

		variables::Menu_Settings::fonts_gubi_14_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 14, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_16_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 16, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_18_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_20_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 20, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_GUIBIGFONT = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 150, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_GUIBIGFONT2 = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 40, 0, icons_ranges_Turkish);


        D3DXCreateTextureFromFileInMemory(pDevice, &ragebotImageByte, IM_ARRAYSIZE(ragebotImageByte), &variables::Menu_Settings::ragebotImage);
        D3DXCreateTextureFromFileInMemory(pDevice, &antiAimImageByte, IM_ARRAYSIZE(antiAimImageByte), &variables::Menu_Settings::antiaimImage);

        D3DXCreateTextureFromFileInMemory(pDevice, &espImageByte, IM_ARRAYSIZE(espImageByte), &variables::Menu_Settings::espImage);
        D3DXCreateTextureFromFileInMemory(pDevice, &chamsImageByte, IM_ARRAYSIZE(chamsImageByte), &variables::Menu_Settings::chamsImage);
        D3DXCreateTextureFromFileInMemory(pDevice, &worldImageByte, IM_ARRAYSIZE(worldImageByte), &variables::Menu_Settings::worldImage);

        D3DXCreateTextureFromFileInMemory(pDevice, &miscImageByte, IM_ARRAYSIZE(miscImageByte), &variables::Menu_Settings::miscImage);
        D3DXCreateTextureFromFileInMemory(pDevice, &skinsImageByte, IM_ARRAYSIZE(skinsImageByte), &variables::Menu_Settings::skinsImage);

        D3DXCreateTextureFromFileInMemory(pDevice, &scriptsImageByte, IM_ARRAYSIZE(scriptsImageByte), &variables::Menu_Settings::scriptsImage);
        D3DXCreateTextureFromFileInMemory(pDevice, &settingsImageByte, IM_ARRAYSIZE(settingsImageByte), &variables::Menu_Settings::settingsImage);


        D3DXCreateTextureFromFileInMemory(pDevice, &networkUserImageByte, IM_ARRAYSIZE(networkUserImageByte), &variables::Menu_Settings::networkUserImage);
    }

	/*if (GetAsyncKeyState(VK_INSERT) & 1 || GetAsyncKeyState(VK_DELETE) & 1 || GetAsyncKeyState(VK_XBUTTON2) & 1)
		variables::Menu_Settings::isOpened = !variables::Menu_Settings::isOpened;*/

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