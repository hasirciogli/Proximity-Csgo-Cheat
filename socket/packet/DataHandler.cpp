#include "DataHandler.h"
#include "Packet.h"

using namespace nlohmann::json_abi_v3_11_2;

void CDataHandler::Handle()
{
	std::cout << std::endl << "Before err data = " << this->data << std::endl;

	json jbbk;
	try
	{
		jbbk = json::parse(this->data);
	

		int packetID = jbbk["packet_id"];

		if (!packetID && packetID == NULL)
			return;

		CDataHandlerFuncs cdhf = CDataHandlerFuncs();

		switch ((Packets::NServerPackets::EFromServerToClient)packetID)
		{
			case Packets::NServerPackets::EFromServerToClient::USER_AUTH:
				cdhf.UserAuth(this->data);
				break;
			case Packets::NServerPackets::EFromServerToClient::CHAT_MESSAGE_SENT:
				cdhf.ChatMessageSent(this->data);
				break;

			default:
				break;
		}
	}
	catch (json::parse_error& ex)
	{
		printf("json parse error = %i\n\n", ex.byte);
	}
}
