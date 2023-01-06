#undef UTF8_WINAPI
#define UTF8_WINAPI

#include <steam/isteamfriends.h>

#include "DataHandler.h"
#include "Packet.h"
#include "../msoket.h"
#include "core/menu/chatbox/ChatBox.h"
#include "dependencies/imgui/imgui.h"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"


#include "steam/isteamuser.h"

//#include "../../core/menu/chatbox/ChatBox.h"
//#include "dependencies/imgui/imgui.h"

using namespace nlohmann;


void CDataHandlerFuncs::FunOk(std::string fullData)
	{
		json j = json();

		try
		{
			j = json::parse(fullData);


			if (mSocket::cfg::waiting_response)
				mSocket::cfg::waiting_response = false;

		}
		catch (json::parse_error& err)
		{
			mSocket::cleanup(true);
			return;
		}
	}


void CDataHandlerFuncs::NeedUserAuth(std::string fullData)
	{
		json j = json();

		try
		{
			j = json::parse(fullData);


			std::string errstr = "";
			std::string resstr = "";

			if (!mSocket::getHWID(&errstr, &resstr))
			{
				mSocket::cleanup(true);
				return;
			}

			j["who_i_am"] = "cheat";
			j["cver"] = 1.0f;
			j["packet_id"] = Packets::NClientPackets::USER_AUTH;
			j["data"]["hwid"] = resstr; // Your hwid data
			j["data"]["steam_id"] = SteamUser()->GetSteamID().ConvertToUint64();
			j["data"]["steam_name"] = SteamFriends()->GetPersonaName();

			const char* sError = "";
			mSocket::sendPacketToServer(j.dump().c_str(), &sError);
		}
		catch (json::parse_error& err)
		{
			mSocket::cleanup(true);
			return;
		}
	}

void CDataHandlerFuncs::UserAuth(std::string fullData)
	{
		json faj;
		try
		{
			faj = json::parse(fullData);


			int packetID = faj["packet_id"];//faj["packet_id"];
			std::string dataSTR = faj["data"].dump();
			bool isSuccess = faj["data"]["isSuccess"];
			std::string token = faj["data"]["token"];
			std::string username = faj["data"]["username"];
			std::string subTill = faj["data"]["subs_till"];

			if (isSuccess)
			{
				if (token != "")
				{
					config::reInitOnlyList();
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
		catch (json::parse_error& err)
		{
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}

void CDataHandlerFuncs::ChatMessageSent(std::string fullData)
	{
		try
		{
			json faj;

			try
			{
				faj = json::parse(fullData);
			}
			catch (json::parse_error& err)
			{
				variables::NetworkUser::fuckThisCheat = true;
				mSocket::cfg::authed = false;
				mSocket::cfg::socketIsConnected = false;
				mSocket::cfg::grabbedToken = "";
				return;
			}

			int packetID = faj["packet_id"]; //faj["packet_id"];
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

			//MessageBoxA(0, message_content.c_str(), "", 0);



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
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}






void CDataHandlerFuncs::ConfigCreate(std::string fullData)
	{
		json faj = json();

		try
		{
			faj = json::parse(fullData);


			config::c_config _config = config::c_config();

			_config.configId = faj["data"]["config_id"];
			_config.author = faj["data"]["config_author"];
			_config.configName = faj["data"]["config_name"];
			_config.date = faj["data"]["config_date"];

			config::configsList.push_front(_config);
		}
		catch (json::parse_error& err)
		{
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}

void CDataHandlerFuncs::ConfigRefresh(std::string fullData)
	{
		json faj = json();

		try
		{
			faj = json::parse(fullData);


			config::c_config _config = config::c_config();

			_config.configId = faj["data"]["config_id"];
			_config.author = faj["data"]["config_author"];
			_config.configName = faj["data"]["config_name"];
			_config.date = faj["data"]["config_date"];

			bool needPush = true;

			for (auto& item : config::configsList)
			{
				if (item.configId == _config.configId)
				{
					needPush = false;
					item = _config;
				}

				if (!needPush)
					break;
			}

			if (needPush)
			{
				config::configsList.push_front(_config);
			}

		}
		catch (json::parse_error& err)
		{
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}

void CDataHandlerFuncs::ConfigLoad(std::string fullData)
	{
		json faj = json();

		try
		{
			faj = json::parse(fullData);


			config::c_config _config = config::c_config();

			_config.data = faj["data"]["config_data"];
			_config.configId = faj["data"]["config_id"];

			loadConfig(_config);

			for (auto item : config::configsList)
			{
				if (item.isLoaded)
				{
					item.isLoaded = false;
				}

				if (item.configId == _config.configId)
				{
					item.isLoaded = true;
				}
			}

			interfaces::clientstate->full_update();

		}
		catch (json::parse_error& err)
		{
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}

void CDataHandlerFuncs::VersionResponse(std::string fullData)
	{
		json faj = json();

		try
		{
			faj = json::parse(fullData);

			config::c_config _config = config::c_config();

			bool is_valid = faj["data"]["valid"];

			if (!is_valid)
			{
				ShellExecute(0, 0, "https://www.google.com/search?q=You+need+to+update+a+loader+%26+Cheat", 0, 0, SW_SHOW);
				mSocket::cfg::socketReconnect = false;
				mSocket::cfg::closingTO = true;
				mSocket::cfg::authed = false;
				mSocket::cleanup();
				exit(1);
			}
		}
		catch (json::parse_error& err)
		{
			variables::NetworkUser::fuckThisCheat = true;
			mSocket::cfg::authed = false;
			mSocket::cfg::socketIsConnected = false;
			mSocket::cfg::grabbedToken = "";
			return;
		}
	}
