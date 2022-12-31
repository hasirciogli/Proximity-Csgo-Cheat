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
void initCfgItem(config::config_data_types cType, float* keyplus, color defValue, std::string way);
void initCfgItem(config::config_data_types cType, std::string* keyplus, std::string defValue, std::string way);

void initCfgItem(config::config_data_types cType, std::string way);


void config::reInitOnlyList()
{
	config::configsList.clear();
	config::refreshConfigs();
}


void config::init()
{
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::enabled, false, "aimbot.enabled");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::teamCheck, false, "aimbot.teamcheck");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::noScope, false, "aimbot.noscope");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::autofire, false, "aimbot.autofire");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::silentAim, false, "aimbot.silentAim");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::non_rifle_aimpunch, false, "aimbot.non_rifle_aimpunch");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::auto_revolver, false, "aimbot.auto_revolver");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::drawFov, false, "aimbot.drawFov");
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::priorize_lethal_targets, false, "aimbot.priorize_lethal_targets");

	initCfgItem(DATA_INT, &variables::Aimbot_Settings::aimbotKey, -2, "aimbot.aimbotKey");

	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_smoothing, 0.0f, "aimbot.aimbot_smoothing");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::min_damage, 0.0f, "aimbot.min_damage");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_fov, 0.0f, "aimbot.aimbot_fov");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_hitchance, 0.0f, "aimbot.aimbot_hitchance");



	initCfgItem(DATA_BOOL, &variables::chams::enemy_chams, false, "chams.enemy_chams");
	initCfgItem(DATA_BOOL, &variables::chams::enemy_only_visible, false, "chams.enemy_chams_onlyvisible");
	initCfgItem(DATA_BOOL, &variables::chams::enemy_wireframe, false, "chams.enemy_chams_wireframe");
	initCfgItem(DATA_INT, &variables::chams::enemy_chams_material, 0, "chams.enemy_chams_material");

	//initCfgItem(DATA_COLOR, &variables::chams::chams_enemy_visible, color(255,255,255,255), "chams.enemy_chams_visible");

	//initCfgItem(DATA_COLOR, &variables::chams::chams_enemy_invisible, color(255, 255, 255, 255), "chams.enemy_chams_invisible");

	initCfgItem(DATA_BOOL, &variables::Skin_Changer::isEnabledBasement, false, "sc.eb");
	initCfgItem(DATA_INT, &variables::Skin_Changer::newKnifeID, false, "sc.kid");
	initCfgItem(DATA_INT, &variables::Skin_Changer::newGloveID, false, "sc.gid");

	for (size_t i = 0; i < 70; i++)
	{
		initCfgItem(DATA_BOOL, &variables::Skin_Changer::isEnabled[i], false, std::string(std::string("sc.e") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newPaintKit[i], 0, std::string(std::string("sc.p") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newSeed[i], 0, std::string(std::string("sc.s") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newStatTrak[i], 0, std::string(std::string("sc.st") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newQuality[i], 0, std::string(std::string("sc.q") + std::to_string(i).c_str()));
		initCfgItem(DATA_FLOAT, &variables::Skin_Changer::newWear[i], 0, std::string(std::string("sc.w") + std::to_string(i).c_str()));
	}

	refreshConfigs();
}
void config::loadConfigFromServer(c_config config)
{
	json loadJson = json();

	loadJson["who_i_am"] = std::string("cheat");
	loadJson["packet_id"] = (int)Packets::NClientPackets::CONFIG_LOAD;
	loadJson["data"]["config_id"] = (int)config.configId;

	std::string sendLoadDATA = std::string(loadJson.dump());

	const char* iErr__ = "";
	if (!mSocket::sendPacketToServer(sendLoadDATA.c_str(), &iErr__))
	{
		// TODO: Any Error;
	}
}

void config::loadConfig(c_config config)
{	
	try
	{
		json jObject = config.data;

		for (auto clitem : configItems)
		{
			if (!jObject[clitem.getOneWay()].is_null() && jObject[clitem.getOneWay()].is_object())
			{
				switch (clitem.cDataType)
				{
				case DATA_INT:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_number_integer())
					{
						*(int*)clitem.value = jObject[clitem.getOneWay()][clitem.getSecondWay()];
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
						if (jObject[clitem.getOneWay()][clitem.getSecondWay()]["r"])
						{
							(*(float**)clitem.value)[0] = jObject[clitem.getOneWay()][clitem.getSecondWay()];
						}
						if (jObject[clitem.getOneWay()][clitem.getSecondWay()]["g"])
						{
							(*(float**)clitem.value)[1] = jObject[clitem.getOneWay()][clitem.getSecondWay()];
						}
						if (jObject[clitem.getOneWay()][clitem.getSecondWay()]["b"])
						{
							(*(float**)clitem.value)[2] = jObject[clitem.getOneWay()][clitem.getSecondWay()];
						}
						if (jObject[clitem.getOneWay()][clitem.getSecondWay()]["a"])
						{
							(*(float**)clitem.value)[3] = jObject[clitem.getOneWay()][clitem.getSecondWay()];
						}
					}
					break;
				}
			}
		}
	}
	catch (json::parse_error& er)
	{

	}
}

void config::saveConfig(c_config config, bool silentSave)
{
	try
	{
		json jObject = json();

		for (auto clitem : configItems)
		{
			switch (clitem.cDataType)
			{
			case DATA_INT:
				jObject[clitem.getOneWay()][clitem.getSecondWay()] = *(int*)clitem.value;
				break;
			case DATA_BOOL:
				jObject[clitem.getOneWay()][clitem.getSecondWay()] = *(bool*)clitem.value;
				break;
			case DATA_FLOAT:
				jObject[clitem.getOneWay()][clitem.getSecondWay()] = *(float*)clitem.value;
				break;
			case DATA_STRING:
				jObject[clitem.getOneWay()][clitem.getSecondWay()] = *(std::string*)clitem.value;
				break;
			case DATA_COLOR:
				jObject[clitem.getOneWay()][clitem.getSecondWay()]["r"] = (*(float**)clitem.value)[0];
				jObject[clitem.getOneWay()][clitem.getSecondWay()]["g"] = (*(float**)clitem.value)[1];
				jObject[clitem.getOneWay()][clitem.getSecondWay()]["g"] = (*(float**)clitem.value)[2];
				jObject[clitem.getOneWay()][clitem.getSecondWay()]["a"] = (*(float**)clitem.value)[3];
				break;
			}
		}

		json sO = json();
		sO["who_i_am"] = "cheat";
		sO["packet_id"] = Packets::NClientPackets::CONFIG_SAVE;
		sO["data"]["config_id"] = (int)config.configId;
		sO["data"]["config_data"] = jObject;
		std::string sendLon = sO.dump();

		const char* iStr = "";
		if (mSocket::sendPacketToServer(sendLon.c_str(), &iStr))
		{
			// TODO: Any Socket Sent Error...
		}
		
	}
	catch (json::parse_error& e)
	{

	}
}

void config::createConfig()
{
	json jdATAA;

	jdATAA.clear();

	jdATAA["who_i_am"] = "cheat";
	jdATAA["packet_id"] = (int)Packets::NClientPackets::CONFIG_CREATE;
	std::string sendLon = jdATAA.dump();


	const char* sError = "";
	if (!mSocket::sendPacketToServer(sendLon.c_str(), &sError))
	{
		console::log(sError);
		// TODO: On error occurred
	}
}

void config::refreshConfigs()
{
	json jdATAA;

	jdATAA.clear();

	jdATAA["who_i_am"] = "cheat";
	jdATAA["packet_id"] = (int)Packets::NClientPackets::CONFIG_REFRESH;
	std::string sendLon = jdATAA.dump();


	const char* sError = "";
	if(!mSocket::sendPacketToServer(sendLon.c_str(), &sError))
	{
		console::log(sError);
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
//void initCfgItem(config::config_data_types cType, float* keyplus, color defValue, std::string way)
//{
//	float defVal_[] = {(float)defValue.r, (float)defValue.g, (float)defValue.b, (float)defValue.a};
//	initCfgItem_base(cType, keyplus, way);
//	*keyplus = *defVal_;
//}
void initCfgItem(config::config_data_types cType, std::string* keyplus, std::string defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, way);
	*keyplus = defValue;
}