#include "DataHandler.h"
#include "Packet.h"
#include "../msoket.h"
#include "core/menu/chatbox/ChatBox.h"
#include "dependencies/imgui/imgui.h"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
//#include "../../core/menu/chatbox/ChatBox.h"
//#include "dependencies/imgui/imgui.h"

using namespace nlohmann;


void CDataHandlerFuncs::NeedUserAuth(std::string fullData)
{
	json j = json();

	j["who_i_am"] = "cheat";
	j["packet_id"] = Packets::NClientPackets::USER_AUTH;
	j["data"]["hwid"] = "9963"; // Your hwid data

	const char* sError = "";
	mSocket::sendPacketToServer(j.dump().c_str(), &sError);
}

void CDataHandlerFuncs::UserAuth(std::string fullData)
{
	json faj;
	try
	{
		faj = json::parse(fullData);
	}
	catch (json::parse_error& err)
	{
		mSocket::cleanup(true);
		return;
	}

	


	std::cout << "My data: " << fullData << std::endl;

	
	int packetID = faj["packet_id"];//faj["packet_id"];
	std::string dataSTR = faj["data"].dump();
	bool isSuccess = faj["data"]["isSuccess"];
	std::string token = faj["data"]["token"];
	std::string username = faj["data"]["username"];
	std::string subTill = faj["data"]["subs_till"];

	std::cout << "My data: " << dataSTR << std::endl;

	std::cout << "success: " << isSuccess << std::endl;
	std::cout << "token: " << token << std::endl;

	if (isSuccess)
	{
		if (token != "")
		{
			variables::NetworkUser::username = username;
			mSocket::cfg::authed = true;
			mSocket::cfg::grabbedToken = token;
		}
		else
		{
			mSocket::cfg::authed = false;
			mSocket::cfg::grabbedToken = "";
		}
	}
	else
	{
		mSocket::cfg::authed = false;
		mSocket::cfg::grabbedToken = "";
	}

	if (!mSocket::cfg::authed)
		mSocket::cleanup();
}

void CDataHandlerFuncs::ChatMessageSent(std::string fullData)
{
	try
	{
		json faj = json::parse(fullData);

		int packetID = faj["packet_id"];//faj["packet_id"];
		std::string dataSTR = faj["data"].dump();
		int message_id = faj["data"]["message_id"];
		std::string message_author = faj["data"]["message_author"];
		std::string message_content = faj["data"]["message_content"];
		std::string message_date = faj["data"]["message_date"];


		float message_color_x = faj["data"]["message_content_color"]["x"];
		float message_color_y = faj["data"]["message_content_color"]["y"];
		float message_color_z = faj["data"]["message_content_color"]["z"];

		float author_color_x = faj["data"]["message_author_color"]["x"];
		float author_color_y = faj["data"]["message_author_color"]["y"];
		float author_color_z = faj["data"]["message_author_color"]["z"];



		faj = NULL;


		/*
		unsigned int message_content_color;
		unsigned int message_author_color;

		std::stringstream ss;
		ss << std::hex << bscc;
		ss >> message_content_color;

		std::stringstream ss2;
		ss2 << std::hex << bsac;
		ss2 >> message_author_color;
		*/

		ImVec4 rgbColorAuthor;
		rgbColorAuthor.x = author_color_x / 255.f;
		rgbColorAuthor.y = author_color_y / 255.f;
		rgbColorAuthor.z = author_color_z / 255.f;
		rgbColorAuthor.w = 1.0f;

		ImVec4 rgbColorContent;
		rgbColorContent.x = message_color_x / 255.f;
		rgbColorContent.y = message_color_y / 255.f;
		rgbColorContent.z = message_color_z / 255.f;
		rgbColorContent.w = 1.0f;







		ChatBox::ChatboxItem cbItem;

		cbItem.messageID = message_id;
		cbItem.name = message_author;
		cbItem.message = message_content;
		cbItem.date = message_date;

		cbItem.chatColor = rgbColorContent;
		cbItem.nameColor = rgbColorAuthor;

		ChatBox::chatboxItems.push_back(cbItem);
	}
	catch (json::parse_error& ex)
	{
		std::cout << "func chat err" << ex.what() << std::endl;
	}
}
