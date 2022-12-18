#pragma once
#include "core/menu/global_input.hpp"
#include "dependencies/utilities/csgo.hpp"
#include "dependencies/rapidjson/document.h"

namespace config {
	struct c_config_item_t
	{
		std::string configName;
		std::string date;
		std::string author;
		std::string data;

		bool isLoaded;
	};

	inline std::list<c_config_item_t> configsList;

	void loadConfig(c_config_item_t config);
	void refreshConfigs();
	void init();
}