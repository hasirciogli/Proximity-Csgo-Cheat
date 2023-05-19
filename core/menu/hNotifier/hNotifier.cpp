#include "hNotifier.hpp"



void drawNotification(int x, int y, int width, int height, void* imguiIO, void* nObject, void* drawList, float a);

void HNOTIF::HNotifier::runBasement(LPDIRECT3DDEVICE9* pDevice, float m_alpha) {

	//auto drawList = ImGui::GetForegroundDrawList();
	auto drawList = ImGui::GetBackgroundDrawList();
	auto& io = ImGui::GetIO();

	int lastPlusPadding = 10;

	std::list<variables::Menu_Settings::HNFAnimObject*> pDeleteMe;

	if (variables::Menu_Settings::h_Notifications.size() > 0)
	{
		for (auto tNotify : variables::Menu_Settings::h_Notifications)
		{
			if (tNotify->pleaseDeleteMe)
			{
				pDeleteMe.push_back(tNotify);
				continue;
			}
#ifdef _DEBUG
			{
				//tNotify->masterWidth = 300 * variables::Menu_Settings::DpiSize;
			//tNotify->masterHeight = 60 * variables::Menu_Settings::DpiSize;

				ImGui::Text(std::to_string(tNotify->Alpha).c_str());
				ImGui::Text(std::to_string(tNotify->tAlpha).c_str());
				ImGui::Text(std::to_string(tNotify->baseTime).c_str());


				ImGui::Text(std::to_string(tNotify->width).c_str());
				ImGui::Text(std::to_string(tNotify->height).c_str());
			}
#endif

			tNotify->animationRunner(io.DeltaTime);
			drawNotification(10, lastPlusPadding, tNotify->width * variables::Menu_Settings::DpiSize, tNotify->height * variables::Menu_Settings::DpiSize, (void*)&io, (void*)tNotify, (void*)drawList, tNotify->Alpha);


			lastPlusPadding += (tNotify->height * variables::Menu_Settings::DpiSize) + (10 * variables::Menu_Settings::DpiSize);
		}
	}

	for (auto tNotify : pDeleteMe)
	{
		variables::Menu_Settings::h_Notifications.remove(tNotify);
	}

	pDeleteMe.clear();


	return;
}

void drawNotification(int x, int y, int width, int height, void* imguiIO, void* nObject, void* drawList, float a) {

	ImDrawList* drawlist = (ImDrawList*)drawList;
	variables::Menu_Settings::HNFAnimObject* notify = (variables::Menu_Settings::HNFAnimObject*)nObject;
	ImGuiIO& io = (ImGuiIO&)imguiIO;

	width = std::clamp(width, 0, 10500);
	height = std::clamp(height, 0, 10500);

	drawlist->AddRectFilled(ImVec2(x, y), ImVec2(x + 6, y + height), ImColor(20, 140, 20, (int)a * .76), 4.f, ImDrawCornerFlags_Left);

	drawlist->AddRectFilled(ImVec2(x + 6, y), ImVec2(x + 6 + width, y + height), ImColor(20, 20, 20, a * .65), 4.f, ImDrawCornerFlags_Right);

	drawlist->AddText(nullptr, 16 * variables::Menu_Settings::DpiSize, ImVec2(x + 8 + 6, y + (10 * variables::Menu_Settings::DpiSize)), ImColor(255, 255, 255, (int)notify->tAlpha), notify->title.c_str());
	drawlist->AddText(nullptr, 14 * variables::Menu_Settings::DpiSize, ImVec2(x + 8 + 6, y + (ImGui::CalcTextSize(notify->title.c_str()).y * variables::Menu_Settings::DpiSize) + (14 * variables::Menu_Settings::DpiSize)), ImColor(255, 255, 255, (int)notify->tAlpha), notify->message.c_str());
}