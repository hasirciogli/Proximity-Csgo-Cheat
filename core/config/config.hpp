#pragma once
#include "core/menu/global_input.hpp"
#include "dependencies/utilities/csgo.hpp"
#include "dependencies/rapidjson/document.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

namespace config {
	enum config_data_types
	{
		DATA_INT,
		DATA_BOOL,
		DATA_FLOAT,
		DATA_STRING,
		DATA_COLOR,
	};

	struct c_config
	{
		int configId;
		std::string configName;
		std::string date;
		std::string author;
		json data;
		bool isLoaded;
	};

	struct c_config_item
	{
		std::string way;

		config_data_types cDataType;

		void* value;

		std::string getOneWay()
		{
			return way.substr(0, way.find("."));
		}

		std::string getSecondWay()
		{
			return way.substr(way.find(".") + 1, (int)strlen(way.c_str()));
		}
	};

	inline std::list<c_config> configsList;
	inline std::list<c_config_item> configItems;

	inline c_config* LoadedConfig;

	void loadConfig(c_config config, bool blankLoad = false);
	void loadConfigFromServer(c_config config);
	void saveConfig(c_config config, bool silentSave = false);
	void refreshConfigs();
	void createConfig();
	void init();
	void reInitOnlyList();
}