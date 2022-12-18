#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <winsock.h>
#include <socketapi.h>
#include <WinSock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")




#define DEFAULT_BUFLEN 8192
#define DEFAULT_PORT "6655"
#define DEFAULT_IP "127.0.0.1"

namespace mSocket
{

	namespace cfg
	{
		inline bool socketIsConnected = false;
		inline bool closingTO = false;
		inline bool socketReconnect = true;
		inline bool socketInited = false;


		inline bool authed = false;
		inline std::string grabbedToken = "";

		inline HANDLE socketThreadHandle;
		inline SOCKET ConnectSocket;
		inline WSADATA wsaData;
		inline addrinfo* result;
		inline addrinfo* ptr;
		inline addrinfo hints;

		inline char recvbuf[8192] = "";
		inline int iResult;
		inline int recvbuflen = 8192;
	};


	bool initSoket(const char** errStr);
	bool cleanup();
	int socketThread(HMODULE hModule);
	bool sendPacketToServer(const char* data, const char** iError, bool = false);
};