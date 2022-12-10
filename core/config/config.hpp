#pragma once
#include "core/menu/global_input.hpp"
#include "dependencies/utilities/csgo.hpp"
#include "dependencies/rapidjson/document.h"

namespace config {
	void init();

	inline std::string nullhooks_config_folder;
	void get_nullhooks_folder();

	inline const int max_configs = 24;
	inline int selected_config = -1;
	inline std::vector<std::string> config_names;
	void refresh_list();

	inline textbox_t new_config_name("");
	bool create_new_config(std::string filename);

	bool load_config(std::string filename);
	void load_selected_config();
	
	bool save_config(std::string filename);
	void save_selected_config();

	namespace load {
	}

	namespace save {
	}
}