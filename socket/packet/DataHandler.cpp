#include "DataHandler.h"
using namespace nlohmann;

void CDataHandler::Handle()
{
	json json = json::parse(this->data);
	int packetID = (int)json["packet_id"];

	if (!packetID && packetID == NULL)
		return;

	switch ((Packets::NServerPackets::EFromServerToClient)packetID)
	{
		case Packets::NServerPackets::EFromServerToClient::USER_AUTH:
				CDataHandlerFuncs().UserAuth(this->data);
			break;

		default:
			break;
	}
}
