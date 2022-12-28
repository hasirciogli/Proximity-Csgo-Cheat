#include "dependencies/utilities/csgo.hpp"
#include "socket/msoket.h"
#include "socket/packet/Packet.h"

#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include<direct.h>

#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma comment(lib, "shell32.lib")

void initCfgItem_base(config::config_data_types cType, void* keyplus, std::string way);
void initCfgItem(config::config_data_types cType, int* keyplus, int defValue, std::string way);
void initCfgItem(config::config_data_types cType, bool* keyplus, bool defValue, std::string way);
void initCfgItem(config::config_data_types cType, float* keyplus, float defValue, std::string way);
void initCfgItem(config::config_data_types cType, std::string* keyplus, std::string defValue, std::string way);

void initCfgItem(config::config_data_types cType, std::string way);

void config::init()
{

	c_config config;
	config.configId = 0;
	config.configName = "p-100 / alt+f4";
	config.author = "hasirciogli";
	config.date = "date idk :)";
	config.isLoaded = false;

	config.data = json::parse("{\"aimbot\": {\"enabled\": true}}");

	config::configsList.push_front(config);

	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::enabled, false, "aimbot.enabled");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::teamCheck, false, "aimbot.teamcheck");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::noScope, false, "aimbot.noscope");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::autofire, false, "aimbot.autofire");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::silentAim, false, "aimbot.silentAim");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::non_rifle_aimpunch, false, "aimbot.non_rifle_aimpunch");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::auto_revolver, false, "aimbot.auto_revolver");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::drawFov, false, "aimbot.drawFov");
	initCfgItem(config_data_types::DATA_BOOL, &variables::Aimbot_Settings::priorize_lethal_targets, false, "aimbot.priorize_lethal_targets");

	initCfgItem(config_data_types::DATA_INT, &variables::Aimbot_Settings::aimbotKey, -2, "aimbot.aimbotKey");

	initCfgItem(config_data_types::DATA_FLOAT, &variables::Aimbot_Settings::aimbot_smoothing, 0.0f, "aimbot.aimbot_smoothing");
	initCfgItem(config_data_types::DATA_FLOAT, &variables::Aimbot_Settings::min_damage, 0.0f, "aimbot.min_damage");
	initCfgItem(config_data_types::DATA_FLOAT, &variables::Aimbot_Settings::aimbot_fov, 0.0f, "aimbot.aimbot_fov");
	initCfgItem(config_data_types::DATA_FLOAT, &variables::Aimbot_Settings::aimbot_hitchance, 0.0f, "aimbot.aimbot_hitchance");

	refreshConfigs();
}

void config::loadConfig(c_config config)
{
	json jObject = config.data;

	try
	{
		for (auto clitem : configItems)
		{
			if (!jObject[clitem.getOneWay()].is_null() && jObject[clitem.getOneWay()].is_object())
			{
				switch (clitem.cDataType)
				{
				case DATA_INT:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_number_integer())
					{
						*(int*)clitem.value = atoi(std::string(jObject[clitem.getOneWay()][clitem.getSecondWay()]).c_str());
					}
					break;
				case DATA_BOOL:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_boolean())
					{
						*(bool*)clitem.value = jObject[clitem.getOneWay()][clitem.getSecondWay()];
					}
					break;
				case DATA_FLOAT:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_number_float())
					{
						*(float*)clitem.value = jObject[clitem.getOneWay()][clitem.getSecondWay()];
					}
					break;
				case DATA_STRING:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_string())
					{
						*(std::string*)clitem.value = std::string(jObject[clitem.getOneWay()][clitem.getSecondWay()]).c_str();
					}
					break;
				case DATA_COLOR:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_object())
					{

					}
					break;
				}
				
			}
			else
			{
				
			}
		}

		for (auto Tconfig : configsList)
			if (Tconfig.isLoaded)
				Tconfig.isLoaded = false;

		config.isLoaded = true;

	}
	catch (json::parse_error& er)
	{

	}
}

void config::saveConfig(c_config config, bool silentSave)
{

}

void config::refreshConfigs()
{
	return;

	const char* sError = "";
	if(mSocket::sendPacketToServer("123", &sError))
	{
		// TODO: On error occurred
	}
}





void initCfgItem_base(config::config_data_types cType, void* keyPlus, std::string way)
{
	config::c_config_item cItem;

	cItem.cDataType = cType;
	cItem.way = way;
	cItem.value = keyPlus;

	config::configItems.push_front(cItem);
}

void initCfgItem(config::config_data_types cType, int* keyplus, int defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, way);
	*keyplus = defValue;
}

void initCfgItem(config::config_data_types cType, bool* keyplus, bool defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, way);
	*keyplus = defValue;
}
void initCfgItem(config::config_data_types cType, float* keyplus, float defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, way);
	*keyplus = defValue;
}
void initCfgItem(config::config_data_types cType, std::string* keyplus, std::string defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, way);
	*keyplus = defValue;
}