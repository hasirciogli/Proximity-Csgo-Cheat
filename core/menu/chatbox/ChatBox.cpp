#include "socket/msoket.h"
#include <socket/procr/ProCr.h>
#include "socket/packet/Packet.h"
#include "core/menu/variables.hpp"

#include "ChatBox.h"

using namespace std;
using namespace ImGui;
using namespace nlohmann::json_abi_v3_11_2;

void CB_imspaceMacro(float x, float y) {
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + x, ImGui::GetCursorPos().y + y));
}

auto CB_getZtkColor = [](int red, int green, int blue, int alpha = 255)
{
	return ImVec4((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, (float)alpha / 255.0f);
};

void renderChatBoxItem(ChatBox::ChatboxItem item, std::string cName)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6);
	{
		ImGui::BeginChild(cName.c_str(), ImVec2(430, 55), false);
		{
			CB_imspaceMacro(0, 5);
			ImGui::BeginChild(string("#chatbox-item-title" + cName).c_str(), ImVec2(430, 20), false);
			{
				CB_imspaceMacro(10, 0);
				TextColored(item.nameColor, item.name.c_str());

				SameLine();

				PushFont(variables::Menu_Settings::fonts_gubi_14_font);
				{
					CB_imspaceMacro(5, 0);
					TextColored(ImVec4(255, 255, 255, 255), item.date.c_str());
				}
				PopFont();
			}
			ImGui::EndChild();

			CB_imspaceMacro(0, 5);
			ImGui::BeginChild(string("#chatbox-item-content" + cName).c_str(), ImVec2(430, 20), false);
			{
				CB_imspaceMacro(10, 0);
				TextColored(item.chatColor, item.message.c_str());
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
		Separator();
	}
	PopStyleVar();
}


void ChatBox::runCustomGui(LPDIRECT3DDEVICE9 pDevice, bool param, float m_alpha) {


	if (!mSocket::cfg::socketIsConnected)
		return;

	if (m_alpha <= 0.0001f)
		return;

	float ds = variables::Menu_Settings::DpiSize;


	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.013, .013, .013, 1));
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(50, 50, 50, 160));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.45f, 0.45f, 0.45f, 0.45f));

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6 * ds);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10 * ds, 5 * ds));

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.25f, 0.25f, .3));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.3f, 0.2f, .3));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.2f, 0.4f, 0.2f, .3));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, .3));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, .3));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.9f, 0.3f, .3));

		SetNextWindowSize(ImVec2(350, 480));
		ImGui::SetNextWindowBgAlpha(zbdckClamp(m_alpha, 0.f, .67f));
		ImGui::Begin("#name", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		{
			ImGui::BeginChild("#title", ImVec2(350, 30), false);
			{
				ImGui::BeginChild("#chatbox-title-left", ImVec2(150, 30));
				{
					std::string text = "Chat";
					ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
					CB_imspaceMacro(10, 30 / 2 - textSize.y / 2);
					ImGui::TextColored(CB_getZtkColor(200, 200, 200), text.c_str());
				}
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("#chatbox-title-right", ImVec2(200, 30));
				{
					/*std::string text = "N";
					ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
					CB_imspaceMacro(10, 30 / 2 - textSize.y / 2);
					ImGui::TextColored(CB_getZtkColor(20, 160, 20), text.c_str());

					ImGui::SameLine();*/

					std::string text = "N";
					ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
					CB_imspaceMacro(200 * ds - textSize.x - 10, 30 * ds / 2 - textSize.y / 2);
					ImGui::TextColored(CB_getZtkColor(variables::Menu_Settings::notifyOnMessage ? 20 : 160, variables::Menu_Settings::notifyOnMessage ? 160 : 20, 20), text.c_str());
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						variables::Menu_Settings::notifyOnMessage = !variables::Menu_Settings::notifyOnMessage;
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();

			ImGui::Separator();

			ImGui::BeginChild("#chatbox", ImVec2(350, 400), false);
			{
				CB_imspaceMacro(10, 10);
				ImGui::BeginChild("#chatbox_inside", ImVec2(330, 380), true);
				{
					int bbEka = 0;
					for (auto item : chatboxItems)
					{
						renderChatBoxItem(item, std::string(std::string("#message_item_") + std::to_string(bbEka)));
						bbEka++;
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();

			ImGui::BeginChild("#bottom-tab", ImVec2(350, 50), false);
			{
				ImGuiStyle& style = GetStyle();
				CB_imspaceMacro(10, 0);
				PushStyleVar(ImGuiStyleVar_ChildRounding, 3);
				{
					ImGui::BeginChild("#input-child", ImVec2(350 - 100 - 20, 40), false);
					{
						PushItemWidth(-1);
						PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 10));
						PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
						{
							string revSTR = std::string(DDKmessage);
							//std::string tC = vfuns::allowedCharsCheck(revSTR, revSTR.length());
							//strcpy(DDKmessage, tC.c_str());
							ImGui::InputText("#chatbox-inputtext", DDKmessage, IM_ARRAYSIZE(DDKmessage));
						}
						PopStyleVar();
						PopStyleVar();
						PopItemWidth();
					}
					ImGui::EndChild();
				}
				PopStyleVar();

				SameLine();
				CB_imspaceMacro(10, 0);

				if (!mSocket::cfg::waiting_response)
				{
					if (Button("Send", ImVec2(90, 40)))
					{
						string tMessage = DDKmessage;

						json jdATAA;

						jdATAA.clear();

						jdATAA["who_i_am"] = "cheat";
						jdATAA["packet_id"] = (int)Packets::NClientPackets::CHAT_MESSAGE_SENT;
						jdATAA["data"]["message_content"] = tMessage;

						string sendLon = jdATAA.dump();

						const char* eData = "";

						if (mSocket::sendPacketToServer(sendLon.c_str(), &eData))
						{
							printf("\n\npacket Sent -> %s\n\n", sendLon.c_str());
						}
						else
						{
							printf("\n\nsendPacketError -> %s\n\n", eData);
						}

						DDKmessage[0] = {};
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();



		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
	ImGui::PopStyleVar();
}


/*

void ChatBox::AddChatItem(ChatboxItem item)
{
	ChatBox::chatboxItems.push_back(item);
};

void ChatBox::clearItems()
{
	ChatBox::chatboxItems.clear();
};

void ChatBox::removeItem(int messageID)
{
	ChatboxItem rItem;
	for (auto item : ChatBox::chatboxItems)
	{
		if (item.messageID == messageID)
		{
			rItem = item;
		}
	}
	ChatBox::chatboxItems.remove(rItem);
};

*/