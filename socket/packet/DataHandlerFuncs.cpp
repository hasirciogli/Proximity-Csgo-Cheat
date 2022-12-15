#include "DataHandler.h"
#include "../msoket.h"
#include "core/menu/chatbox/ChatBox.h"
#include "dependencies/imgui/imgui.h"
//#include "../../core/menu/chatbox/ChatBox.h"
//#include "dependencies/imgui/imgui.h"

using namespace nlohmann;


void CDataHandlerFuncs::FirstAuth(std::string fullData)
{
	
}

void CDataHandlerFuncs::UserAuth(std::string fullData)
{
	json faj = json::parse(fullData);

	int packetID = (int)faj["packet_id"];//faj["packet_id"];
	std::string dataSTR = faj["data"].dump();
	bool isSuccess = faj["data"]["isSuccess"];
	std::string token = faj["data"]["token"];

	std::cout << std::endl << dataSTR << std::endl;

	if (isSuccess)
	{
		if (token != "")
		{
			mSocket::cfg::tokenGrabbed = true;
			mSocket::cfg::grabbedToken = token;
		}
		else
		{
			mSocket::cfg::tokenGrabbed = false;
			mSocket::cfg::grabbedToken = "";
		}
	}
	else
	{
		mSocket::cfg::tokenGrabbed = false;
		mSocket::cfg::grabbedToken = "";
	}

	if (!mSocket::cfg::tokenGrabbed)
		mSocket::cleanup();
}

void CDataHandlerFuncs::ChatMessageSent(std::string fullData)
{
	
	json faj = json::parse(fullData);

	int packetID = (int)faj["packet_id"];//faj["packet_id"];
	std::string dataSTR					= faj["data"].dump();
	int message_id					= faj["data"]["message_id"];
	std::string message_uthor		= faj["data"]["message_author"];
	std::string message_content		= faj["data"]["message_content"];
	std::string message_date		= faj["data"]["message_date"];


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


	ImVec4 rgbColorAuthor;
	rgbColorAuthor.x = ((message_author_color >> 16) & 0xFF) / 255.0f;  // Extract the RR byte
	rgbColorAuthor.y = ((message_author_color >> 8) & 0xFF) / 255.0f;   // Extract the GG byte
	rgbColorAuthor.z = ((message_author_color) & 0xFF) / 255.0f;        // Extract the BB byte
	rgbColorAuthor.w = 1.0f;

	ImVec4 rgbColorContent;
	rgbColorContent.x = ((message_content_color >> 16) & 0xFF) / 255.0f;  // Extract the RR byte
	rgbColorContent.y = ((message_content_color >> 8) & 0xFF) / 255.0f;   // Extract the GG byte
	rgbColorContent.z = ((message_content_color) & 0xFF) / 255.0f;        // Extract the BB by te
	rgbColorContent.w = 1.0f;

	*/

	
	
	 
	
	ChatBox::ChatboxItem cbItem;

	cbItem.messageID		= message_id;
	cbItem.name				= message_uthor;
	cbItem.message			= message_content;
	cbItem.date				= message_date;

	cbItem.chatColor		= ImColor(255, 255, 255, 255);
	cbItem.nameColor		= ImColor( 255, 50, 50, 255);

	ChatBox::chatboxItems.push_back(cbItem);
}
