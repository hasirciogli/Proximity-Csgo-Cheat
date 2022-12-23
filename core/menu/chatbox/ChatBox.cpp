#include "socket/msoket.h"
#include "socket/packet/Packet.h"

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


void ChatBox::runCustomGui(LPDIRECT3DDEVICE9 pDevice, bool param) {


	if (!mSocket::cfg::socketIsConnected)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, CB_getZtkColor(20,20,20));
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, CB_getZtkColor(20, 160, 20)); 
	ImGui::PushStyleColor(ImGuiCol_Border, CB_getZtkColor(35, 160, 35));


	SetNextWindowSize(ImVec2(450, 600));
	ImGui::Begin("#name", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
	{
		ImGui::BeginChild("#title", ImVec2(450, 40), false);
		{
			std::string text = "Proximity Chat";
			ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
			CB_imspaceMacro(10, 40 / 2 - textSize.y / 2);
			ImGui::TextColored(CB_getZtkColor(20, 160, 20), text.c_str());
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("#chatbox", ImVec2(450, 500), false);
		{
			CB_imspaceMacro(10, 10);
			ImGui::BeginChild("#chatbox_inside", ImVec2(430, 480), true);
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

		ImGui::BeginChild("#bottom-tab", ImVec2(450, 60), false);
		{
			ImGuiStyle& style = GetStyle();
			CB_imspaceMacro(10, 0);
			PushStyleVar(ImGuiStyleVar_ChildRounding, 3);
			{
				ImGui::BeginChild("#input-child", ImVec2(430 - 100 - 20, 40), true);
				{
					PushItemWidth(-1);
					PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 10));
					PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
					{
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

			if (Button("Send", ImVec2(100, 40)))
			{
				string tMessage = DDKmessage;
				string username = "admin";

				json jdATAA;

				jdATAA.clear();
				 
				jdATAA["who_i_am"] = "cheat";
				jdATAA["packet_id"] = (int)Packets::NClientPackets::CHAT_MESSAGE_SENT;
				jdATAA["data"]["message_author"]		= username;
				jdATAA["data"]["message_content"]		= tMessage;

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
		ImGui::EndChild();
	}
	ImGui::End();



	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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