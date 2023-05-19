#include "dependencies/utilities/csgo.hpp"
#include "socket/msoket.h"
#include "socket/packet/Packet.h"

#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include <direct.h>

#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma comment(lib, "shell32.lib")

#include <iostream>
#include <vector>
#include <filesystem>






void initCfgItem_base(config::config_data_types cType, void* keyplus, void* defvalX, std::string way);
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
	initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::auto_wall, false, "aimbot.awal");

	initCfgItem(DATA_INT, &variables::Aimbot_Settings::aimbotKey, -2, "aimbot.aimbotKey");

	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_smoothing, 0.0f, "aimbot.aimbot_smoothing");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::min_damage, 0.0f, "aimbot.min_damage");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_fov, 0.0f, "aimbot.aimbot_fov");
	initCfgItem(DATA_FLOAT, &variables::Aimbot_Settings::aimbot_hitchance, 0.0f, "aimbot.aimbot_hitchance");

	for (size_t i = 0; i < 5; i++)
	{
		initCfgItem(DATA_BOOL, &variables::Aimbot_Settings::selected_hitboxes[i], false, std::string(std::string("aimbot.hitbox_") + std::to_string(i).c_str()));
	}

	initCfgItem(DATA_BOOL, &variables::Skin_Changer::isEnabledBasement, false, "sc.eb");
	initCfgItem(DATA_BOOL, &variables::Skin_Changer::skinlistSelectedWeaponID, false, "sc.swid");
	initCfgItem(DATA_INT, &variables::Skin_Changer::selectedKnifeNameID, 0, "sc.kid");
	initCfgItem(DATA_INT, &variables::Skin_Changer::selectedGloveNameID, 0, "sc.gid");

	for (size_t i = 0; i < 70; i++)
	{
		initCfgItem(DATA_BOOL, &variables::Skin_Changer::isEnabled[i], false, std::string(std::string("sc.e") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newPaintKit[i], 0, std::string(std::string("sc.p") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newSeed[i], 0, std::string(std::string("sc.s") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newStatTrak[i], 0, std::string(std::string("sc.st") + std::to_string(i).c_str()));
		initCfgItem(DATA_INT, &variables::Skin_Changer::newQuality[i], 0, std::string(std::string("sc.q") + std::to_string(i).c_str()));
		initCfgItem(DATA_FLOAT, &variables::Skin_Changer::newWear[i], 0, std::string(std::string("sc.w") + std::to_string(i).c_str()));
	}






	{


		initCfgItem(DATA_BOOL, &variables::Chams_Settings::weapon_chams, false, "chms.w_e"); // Enemy chams enabled
		initCfgItem(DATA_BOOL, &variables::Chams_Settings::weapon_wireframe, false, "chms.w_wfr"); // Enemy chams only visible
		initCfgItem(DATA_INT, &variables::Chams_Settings::weapon_chams_material, false, "chms.w_mat"); // Enemy chams material

		initCfgItem(DATA_BOOL, &variables::Chams_Settings::arm_chams, false, "chms.a_e"); // Enemy chams enabled
		initCfgItem(DATA_BOOL, &variables::Chams_Settings::arm_wireframe, false, "chms.a_wfr"); // Enemy chams only visible
		initCfgItem(DATA_INT, &variables::Chams_Settings::arm_chams_material, false, "chms.a_mat"); // Enemy chams material

		initCfgItem(DATA_BOOL, &variables::Chams_Settings::sleeve_chams, false, "chms.s_e"); // Sleeve enabled
		initCfgItem(DATA_BOOL, &variables::Chams_Settings::sleeve_wireframe, false, "chms.s_wfr"); // Sleeve wireframe
		initCfgItem(DATA_INT, &variables::Chams_Settings::sleeve_chams_material, false, "chms.s_mat"); // Sleeve chams material


		for (size_t i = 0; i <= 3; i++) // 3 dahil max 3 = 0, 1, 2, 3
		{
			initCfgItem(DATA_FLOAT, &variables::Chams_Settings::colors_weapon[i], i == 3 ? 1.0f : 0.0f, std::string("chms.wcol") + std::to_string(i)); // weapon color
			initCfgItem(DATA_FLOAT, &variables::Chams_Settings::colors_arm[i], i == 3 ? 1.0f : 0.0f, std::string("chms.acol") + std::to_string(i)); // arm color
			initCfgItem(DATA_FLOAT, &variables::Chams_Settings::colors_sleeve[i], i == 3 ? 1.0f : 0.0f, std::string("chms.scol") + std::to_string(i)); // sleeve color
		}

		for (size_t i = 0; i < 3; i++) // 3 dahil değil max 2 = 0, 1, 2
		{
			initCfgItem(DATA_BOOL, &variables::Chams_Settings::chams_enableds[i], false, std::string("chms.enblds-") + std::to_string(i)); // cheams enabled for teams
			initCfgItem(DATA_BOOL, &variables::Chams_Settings::chams_onlyvisibleds[i], false, std::string("chms.ovs-") + std::to_string(i)); // Chamses only visible enabled
			initCfgItem(DATA_BOOL, &variables::Chams_Settings::chams_wireframes[i], false, std::string("chms.wfrme-") + std::to_string(i)); // Chamses wireframe enabled
			initCfgItem(DATA_INT, &variables::Chams_Settings::chams_materialids[i], 0, std::string("chms.matids-") + std::to_string(i)); // Chamses wireframe enabled
		}

		initCfgItem(DATA_BOOL, &variables::Chams_Settings::draw_chams_on_top, false, "chms.dtop"); // Chams on top please is enabled
		

		for (size_t i = 0; i < 3; i++) // team for 0 = enemy, 1 = team, 2 = local;
		{
			for (size_t i2 = 0; i2 < 2; i2++) // visible or not for | 0 = vicibe, 1 = invicible;
			{
				for (size_t i3 = 0; i3 <= 3; i3++) // visible or not color for | 0 = r,     1 = g,     2 = b,    3 = a;
				{
					initCfgItem(DATA_FLOAT, &variables::Chams_Settings::player_colors_chams[i][i2][i3], i3 == 3 ? 1.0f : 0.0f, std::string(std::string("chms.col_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str() + std::string("_") + std::to_string(i3).c_str()));      // chams colors
				}
			}
		}
	}







	{ // ALL ESP SETTINGS DEFINED HERE
		initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledBase, false, std::string(std::string("esp.eba")));

	
		for (size_t i = 0; i < 3; i++)
		{
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledBox[i], false, std::string(std::string("esp.ebo") + std::to_string(i)));
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledGethers[i], false, std::string(std::string("esp.ei") + std::to_string(i)));
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledHealthesp[i], false, std::string(std::string("esp.ehe") + std::to_string(i)));
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledLine[i], false, std::string(std::string("esp.el") + std::to_string(i)));
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledNameesp[i], false, std::string(std::string("esp.ene") + std::to_string(i)));
			initCfgItem(DATA_BOOL, &variables::Esp_Settings::enabledSkeleton[i], false, std::string(std::string("esp.sk") + std::to_string(i)));

			for (size_t i2 = 0; i2 <= 3; i2++)
			{
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_box[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.bx_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str()));      // esp box colors
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_health[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.he_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str()));    // esp box colors
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_info[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.in_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str()));      // esp box colors
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_line[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.li_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str()));      // esp box colors
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_name[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.na_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str()));      // esp box colors
				initCfgItem(DATA_FLOAT, &variables::Esp_Settings::colors_skeleton[i][i2], i2 == 3 ? 1.0f : 0.0f, std::string(std::string("esp.ske_") + std::to_string(i).c_str() + std::string("_") + std::to_string(i2).c_str())); // esp box colors
			}
		}
	}



	initCfgItem(DATA_BOOL, &variables::misc::bhop, false, std::string(std::string("misc.bhop_enb")));
	initCfgItem(DATA_BOOL, &variables::misc::clean_screenshots, false, std::string(std::string("misc.scsht_clean")));
	initCfgItem(DATA_BOOL, &variables::misc::infinite_duck, false, std::string(std::string("misc.inf_dck"))); 
	initCfgItem(DATA_BOOL, &variables::misc::slowwalk, false, std::string(std::string("misc.slow_walk")));
	initCfgItem(DATA_BOOL, &variables::misc::reveal_ranks, false, std::string(std::string("misc.rev_ranks")));
	initCfgItem(DATA_INT, &variables::misc::autostrafe_target, 0, std::string(std::string("misc.as_opt")));


	initCfgItem(DATA_FLOAT, &variables::misc_visuals::noflash_alpha, 255.f, std::string(std::string("misc.flash_alpha")));




	initCfgItem(DATA_BOOL, &variables::misc::thirdperson, false, std::string(std::string("misc.tpeb")));



	c_config tfg = c_config();

	loadConfig(tfg, true);
	refreshConfigs();
}

void config::loadConfigFromServer(c_config config)
{
	std::string filename = config.configName;
	std::string directoryPath = "./proximity/";

	std::filesystem::path filePath = std::filesystem::path(directoryPath) / std::filesystem::path(filename);

	// Dosya okuma işlemi
	std::ifstream file(filePath);
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		console::log(std::string("Dosya okuma başarılı\n").c_str());
		file.close();
		try
		{
			config.data = json::parse(content);

			loadConfig(config, false);

			LoadedConfig = config;


			variables::Menu_Settings::addNotification(-1, "LOAD SUCCESS", "the config loaded");

		}
		catch (json::parse_error& e)
		{
			console::log("Load error");
		}
		


	}
	else {
		console::log("Dosya acilamadi.");
	}

	return;
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

void config::loadConfig(c_config config, bool blankLoad)
{
	try
	{
		json jObject;
		if (blankLoad)
		{
			c_config nCfg;
			nCfg.data = json::parse("{}");
			jObject = nCfg.data;
		}
		else
		{
			jObject = config.data;
		}
		

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
					else
					{
						*(int*)clitem.value = *(int*)clitem.dValue;
					}
					break;
				case DATA_BOOL:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_boolean())
					{
						*(bool*)clitem.value = jObject[clitem.getOneWay()][clitem.getSecondWay()];
					}
					else
					{
						*(bool*)clitem.value = *(bool*)clitem.dValue;
					}
					break;
				case DATA_FLOAT:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_number_float())
					{
						*(float*)clitem.value = jObject[clitem.getOneWay()][clitem.getSecondWay()];
					}
					else
					{
						*(float*)clitem.value = *(float*)clitem.dValue;
					}
					break;
				case DATA_STRING:
					if (!jObject[clitem.getOneWay()][clitem.getSecondWay()].is_null() && jObject[clitem.getOneWay()][clitem.getSecondWay()].is_string())
					{
						*(std::string*)clitem.value = std::string(jObject[clitem.getOneWay()][clitem.getSecondWay()]).c_str();
					}
					else
					{
						*(std::string*)clitem.value = *(std::string*)clitem.dValue;
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
		console::log("CFG LOAD ERROR OCCURRED");
		console::log(er.what());
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

		{
			{
				{
					std::string filename = config.configName;
					std::string content = jObject.dump();

					// Dosya yolu oluşturma
					std::filesystem::path filePath = std::filesystem::path(directoryPath) / std::filesystem::path(filename);

					// Dosya yazma işlemi
					std::ofstream file(filePath);
					if (file.is_open()) {
						file << content;
						file.close();
						console::log("Dosya yazildi.");

						variables::Menu_Settings::addNotification(-1, "SAVE SUCCESS", "the config saved");

						//if (hnf)
						//	hnf->addNotification(-1, "SAVE SUCCESS", std::string(filename + "configs has been saved").c_str());
					}
					else {
						console::log("Dosya acilamadi.");
					}
				}
			}
		}

		return;

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


void config::deleteConfig(const char* cfgName, bool notify) {

	std::cout << "3162: " << cfgName << std::endl;
	std::string filename = cfgName != "" ? cfgName : "cfgname";
	std::string directoryPath = "./proximity/";

	std::filesystem::path filePath = std::filesystem::path(directoryPath) / std::filesystem::path(filename);

	std::error_code errorCode;
	bool result = std::filesystem::remove(filePath, errorCode);

	if (result) {
		if (notify)
			variables::Menu_Settings::addNotification(-1, "Removed", "The config deleted :(");

		std::cout << "Dosya başarıyla silindi." << std::endl;
	}
	else {
		std::cout << "Dosya silinemedi. Hata kodu: " << errorCode.value() << filePath << std::endl;
	}
}

void config::createConfig(const char* configName)
{

	std::string filename = configName != "" ? std::string(std::string(configName) + std::string(".cfg")) : "cfgname";
	std::string directoryPath = "./proximity/";

	std::filesystem::path filePath = std::filesystem::path(directoryPath) / std::filesystem::path(filename);

	std::ifstream dosya(filePath);

	if (dosya.good())
	{
		std::cout << filename << " dosyası zaten var." << std::endl;
		dosya.close();
	}
	else
	{ 
		std::ofstream y_dosya(filePath);
		std::cout << filename << " dosyası oluşturuldu." << std::endl;


		if (y_dosya.is_open()) {
			y_dosya << "{}";
			y_dosya.close();
			console::log("Dosya yazildi.");

			//variables::Menu_Settings::addNotification(-1, "SAVE SUCCESS", "the config saved");

			//if (hnf)
			//	hnf->addNotification(-1, "SAVE SUCCESS", std::string(filename + "configs has been saved").c_str());
		}
		else {
			console::log("Dosya acilamadi.");
		}
	}

	return;
	json jdATAA;

	jdATAA.clear();

	jdATAA["who_i_am"] = "cheat";
	jdATAA["packet_id"] = (int)Packets::NClientPackets::CONFIG_CREATE;
	std::string sendLon = jdATAA.dump();

	variables::Menu_Settings::addNotification(-1, "CREATE SUCCESS", "the config created");
	//if (hnf)
	//	hnf->addNotification(-1, "Config created", "Config has been created");


	const char* sError = "";
	if (!mSocket::sendPacketToServer(sendLon.c_str(), &sError)) 
	{
		console::log(sError);
		// TODO: On error occurred
	}
}

void config::refreshConfigs(bool notify)
{
	namespace fs = std::filesystem;

	std::vector<std::string> filenames;
	std::string directoryPath = "proximity";

	fs::create_directory(directoryPath);

	// Iterate through the directory and find files with .cfg extension
	for (const auto& entry : fs::directory_iterator(directoryPath))
	{
		if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".cfg")
		{
			// Add the filename to the vector
			filenames.push_back(entry.path().filename().string());
		}
	}

	// Loop through the filenames vector and print each filename

	config::configsList.clear();

	int zbkId = 0;

	for (const auto& filename : filenames)
	{
		config::c_config _config = config::c_config();

		_config.configId = zbkId;
		_config.author = "You are";
		_config.configName = filename;
		_config.date = "today";

		config::configsList.push_front(_config);

		zbkId++;
	}

	if (notify)
		variables::Menu_Settings::addNotification(-1, "RELOAD SUCCESS", "Configs has been reloaded");


	return;
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
	else
	{
		
	}
}

void initCfgItem_base(config::config_data_types cType, void* keyPlus, void* defvalX, std::string way)
{
	config::c_config_item cItem;

	cItem.cDataType = cType;
	cItem.way = way;
	cItem.value = keyPlus;
	cItem.dValue = defvalX;

	config::configItems.push_front(cItem);
}

void initCfgItem(config::config_data_types cType, int* keyplus, int defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, &defValue, way);
	*keyplus = defValue;
}

void initCfgItem(config::config_data_types cType, bool* keyplus, bool defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, &defValue, way);
	*keyplus = defValue;
}

void initCfgItem(config::config_data_types cType, float* keyplus, float defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, &defValue, way);
	*keyplus = defValue;
}

void initCfgItem(config::config_data_types cType, std::string* keyplus, std::string defValue, std::string way)
{
	initCfgItem_base(cType, keyplus, &defValue, way);
	*keyplus = defValue;
}