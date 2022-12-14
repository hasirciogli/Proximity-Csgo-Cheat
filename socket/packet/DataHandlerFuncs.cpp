#include "DataHandler.h"
#include "../msoket.h"
#include "../../core/menu/chatbox/ChatBox.h"

using namespace nlohmann;
using namespace std;


void CDataHandlerFuncs::FirstAuth(std::string fullData)
{
	
}

void CDataHandlerFuncs::UserAuth(std::string fullData)
{
	json faj = json::parse(fullData);

	int packetID = (int)faj["packet_id"];//faj["packet_id"];
	std::string dataSTR = faj["data"].dump();
	bool isSuccess = faj["data"]["isSuccess"];
	string token = faj["data"]["token"];

	cout << dataSTR << endl;

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
	string dataSTR				= faj["data"].dump();
	int message_id			= faj["data"]["message_id"];
	string message_uthor	= faj["data"]["message_uthor"];
	string message_content	= faj["data"]["message_content"];
	string message_date		= faj["data"]["message_date"];

	ChatBox::ChatboxItem cbItem;

	cbItem.messageID	= message_id;
	cbItem.name			= message_uthor;
	cbItem.message		= message_content;
	cbItem.date			= message_date;

	ChatBox::chatboxItems.push_back(cbItem);
}
