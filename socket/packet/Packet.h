

namespace Packets
{
	namespace NServerPackets
	{
		enum EFromServerToClient
		{
			FUN_OK,
			USER_AUTH_RESPONSE,
			NEED_USER_AUTH,
			CONFIG_CREATE_RESPONSE,
			CONFIG_LOAD_RESPONSE,
			CONFIG_REFRESH_RESPONSE,
			CONFIG_REMOVE_RESPONSE,
			CONFIG_SAVE_RESPONSE,
			CHAT_MESSAGE_SENT,
			VERSION_RESP,
		};
	};

	namespace NClientPackets
	{
		enum EFromClientToServer
		{
			FUN_OK,
			USER_AUTH,
			CONFIG_CREATE,
			CONFIG_LOAD,
			CONFIG_REFRESH,
			CONFIG_REMOVE,
			CONFIG_SAVE,
			CHAT_MESSAGE_SENT,
		};
	};
};