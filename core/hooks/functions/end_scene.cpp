#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/menu/menu.hpp"
#include "core/hooks/hooks.hpp"
#include "bytes/LeftBarIcons.h"
#include "bytes/CheatIcons.hpp"
#include "bytes/FontBytes.hpp"


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

			0,
		};
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18, 0, icons_ranges_Turkish);

		variables::Menu_Settings::fonts_gubi_8_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 8, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_10_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 10, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_12_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 12, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_14_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 14, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_16_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 16, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_18_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 18, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_20_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 20, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_22_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 22, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_24_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 24, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_26_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 26, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_28_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 28, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_30_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 30, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_32_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 32, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_34_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 34, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_gubi_36_font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 36, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_GUIBIGFONT = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 150, 0, icons_ranges_Turkish);
		variables::Menu_Settings::fonts_GUIBIGFONT2 = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 40, 0, icons_ranges_Turkish);


		variables::Menu_Settings::fonts_kollektifb_10_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 10, 10);
		variables::Menu_Settings::fonts_kollektifb_15_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 15, 15);
		variables::Menu_Settings::fonts_kollektifb_20_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 20, 20);
		variables::Menu_Settings::fonts_kollektifb_25_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 25, 25);
		variables::Menu_Settings::fonts_kollektifb_30_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 30, 30);
		variables::Menu_Settings::fonts_kollektifb_60_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 60, 32);


		variables::Menu_Settings::fonts_kollektifn_10_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 10, 10);
		variables::Menu_Settings::fonts_kollektifn_15_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 15, 15);
		variables::Menu_Settings::fonts_kollektifn_20_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 20, 20);
		variables::Menu_Settings::fonts_kollektifn_25_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 25, 25);
		variables::Menu_Settings::fonts_kollektifn_30_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 30, 30);
		variables::Menu_Settings::fonts_kollektifn_60_font = io.Fonts->AddFontFromMemoryTTF(&fontKollektifbByte, 60, 60);
		
		io.IniFilename = NULL;
		io.LogFilename = NULL;


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



		D3DXCreateTextureFromFileInMemory(pDevice, &dark_settingsIconImageByte, IM_ARRAYSIZE(dark_settingsIconImageByte), &variables::Menu_Settings::dark_settingsIconImageByte);
    
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