

namespace Packets
{
	namespace NServerPackets
	{
		enum EFromServerToClient
		{
			USER_AUTH_RESPONSE,
			NEED_USER_AUTH,
			CONFIG_CREATE,
			CONFIG_LOAD,
			CONFIG_REFRESH,
			CONFIG_REMOVE,
			CHAT_MESSAGE_SENT,
		};
	};

	namespace NClientPackets
	{
		enum EFromClientToServer
		{
			USER_AUTH,
			CONFIG_CREATE,
			CONFIG_LOAD,
			CONFIG_REFRESH,
			CONFIG_REMOVE,
			CHAT_MESSAGE_SENT,
		};
	};
};