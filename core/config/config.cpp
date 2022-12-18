#include "dependencies/utilities/csgo.hpp"
#include "socket/msoket.h"
#include "socket/packet/Packet.h"

#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include<direct.h>		// _mkdir
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma comment(lib, "shell32.lib")




void config::init()
{
	refreshConfigs();
}

void config::loadConfig(c_config_item_t config)
{
	
}

void config::refreshConfigs()
{
	configsList.clear();







	const char* sError = "";
	if(mSocket::sendPacketToServer("123", &sError))
	{
		// TODO: On error occurred
	}
}