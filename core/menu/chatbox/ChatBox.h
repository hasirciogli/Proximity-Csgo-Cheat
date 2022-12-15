#include "dependencies/imgui/imgui.h"

#include <d3d9.h>
#include <list>
#include <string>
#include <nlohmann/json.hpp>

namespace ChatBox
{
	struct ChatboxItem
	{
		int messageID = 0;
		std::string name = "";
		std::string message = "";
		std::string date = "";
		ImVec4 chatColor;
		ImVec4	nameColor;
	};

	inline char DDKmessage[200] = {};
	inline std::list<ChatboxItem> chatboxItems;

	void runCustomGui(LPDIRECT3DDEVICE9, bool paramval = 0);
};