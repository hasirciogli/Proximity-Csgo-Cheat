#include <d3d9.h>
#include <list>

#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_dx9.h"
#include "dependencies/imgui/imgui_internal.h"
#include <string>




namespace ChatBox
{
	inline struct ChatboxItem
	{
		int messageID = 0;
		std::string name = "";
		std::string message = "";
		std::string date = "";
		ImVec4 chatColor;
		ImVec4 nameColor;
	};

	inline char DDKmessage[200] = {};
	inline std::list<ChatboxItem> chatboxItems;

	void runCustomGui(LPDIRECT3DDEVICE9, bool paramval = 0);
};