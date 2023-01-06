#include "DataHandler.h"
#include "Packet.h"
#include "core/menu/variables.hpp"

using namespace nlohmann::json_abi_v3_11_2;

void CDataHandler::Handle()
{
	if (variables::NetworkUser::fuckThisCheat)
		return;

	try
	{
		json jbbk;
		jbbk = json::parse(this->data);
	

		int packetID = jbbk["packet_id"];

		if (!packetID == 0 && packetID == NULL)
			return;

		CDataHandlerFuncs cdhf = CDataHandlerFuncs();

		switch ((Packets::NServerPackets::EFromServerToClient)packetID)
		{
		case Packets::NServerPackets::EFromServerToClient::FUN_OK:
			cdhf.FunOk(this->data);
			break;
		case Packets::NServerPackets::EFromServerToClient::NEED_USER_AUTH:
			cdhf.NeedUserAuth(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::USER_AUTH_RESPONSE:
			cdhf.UserAuth(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::CHAT_MESSAGE_SENT:
			cdhf.ChatMessageSent(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::CONFIG_CREATE_RESPONSE:
			cdhf.ConfigCreate(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::CONFIG_LOAD_RESPONSE:
			cdhf.ConfigLoad(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::CONFIG_REFRESH_RESPONSE:
			cdhf.ConfigRefresh(this->data);
			break;

		case Packets::NServerPackets::EFromServerToClient::VERSION_RESP:
			cdhf.VersionResponse(this->data);
			break;

			default:
				break;
		}
	}
	catch (json::parse_error& ex)
	{
		console::log("json parse error =");
		console::log(this->data.c_str());
		console::log("\n");
	}
}
