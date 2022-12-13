

namespace Packets
{
	namespace NServerPackets
	{
		enum EFromServerToClient
		{
			FIRST_AUTH,
			USER_AUTH,
			CONFIG_CREATE,
			CONFIG_LOAD,
			CONFIG_REFRESH,
			CONFIG_REMOVE,
		};
	};

	namespace NClientPackets
	{
		enum EFromClientToServer
		{
			FIRST_AUTH,
			USER_AUTH,
			CONFIG_CREATE,
			CONFIG_LOAD,
			CONFIG_REFRESH,
			CONFIG_REMOVE,
		};
	};
};