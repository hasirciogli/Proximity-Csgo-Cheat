#include "DataHandler.h"
#include "../msoket.h"
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
