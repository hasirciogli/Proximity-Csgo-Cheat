#include "msoket.h"

#include <nlohmann/json.hpp>

#include "packet/DataHandler.h"
#include <core/menu/variables.hpp>

int bebeka = 0;

using nlohmann::json;

int mSocket::socketThread(HMODULE hModule) 
{
#ifdef _DEBUG
	static const char* bErr = "";
	if (!mSocket::initSoket(&bErr))
	{
		variables::cheat::logboxLists.push_front(bErr);
		std::cout << "" << bErr << std::endl;
		mSocket::cleanup();
	}
	
#else

	static const char* bErr = "";
	if (!mSocket::initSoket(&bErr))
	{
		variables::cheat::openLink("Server connection eror");
		mSocket::cleanup();
	}

#endif 



	while (!cfg::closingTO)
	{
		if (mSocket::cfg::socketInited && !cfg::closingTO && !variables::NetworkUser::fuckThisCheat)
		{ 
			if (mSocket::cfg::socketIsConnected && !cfg::closingTO)
			{
				mSocket::cfg::iResult = recv(mSocket::cfg::ConnectSocket, mSocket::cfg::recvbuf, mSocket::cfg::recvbuflen, 0);
				if (mSocket::cfg::iResult > 0)
				{
					std::string test = "";
					for (size_t i = 0; i < mSocket::cfg::iResult; i++) 
					{
						test += mSocket::cfg::recvbuf[i];
					}

#ifdef _DEBUG 
					std::cout << "DR ["<< mSocket::cfg::iResult << "] - " << test.c_str() << std::endl; 
#endif

					CDataHandler cdh = CDataHandler();
					cdh.data = test;
					cdh.Handle();

					test = "";
					mSocket::cfg::recvbuf[0] = {};
				}
				else if (mSocket::cfg::iResult == 0)
				{
					mSocket::cfg::socketIsConnected = false;
					mSocket::cfg::authed = false;
					mSocket::cfg::grabbedToken = "";
#ifdef _DEBUG 
					printf("Connection closed\n\n");
#endif	
					
				}
				else
				{
					mSocket::cfg::authed = false;
					mSocket::cfg::grabbedToken = "";
					mSocket::cfg::socketReconnect		= true;
					mSocket::cfg::socketIsConnected		= false;
#ifdef _DEBUG
					printf("recv failed with error: %d\n\n", WSAGetLastError());
#endif

				}
			}
			else if (mSocket::cfg::socketReconnect && !cfg::closingTO)
			{
				mSocket::cfg::authed = false;
				mSocket::cfg::grabbedToken = "";
#ifdef _DEBUG
				variables::cheat::logboxLists.push_front("Socket connection failed | reconnecting...");
#endif
				bebeka++;
				// Resolve the server address and port
				mSocket::cfg::iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &mSocket::cfg::hints, &mSocket::cfg::result);
				if (mSocket::cfg::iResult != 0) {
#ifdef _DEBUG
					printf("getaddrinfo failed with error: %d\n\n", mSocket::cfg::iResult);
#endif
					WSACleanup();
					continue;
				}

				cfg::ptr = cfg::result;
				// Create a SOCKET for connecting to server
				mSocket::cfg::ConnectSocket = socket(mSocket::cfg::ptr->ai_family, mSocket::cfg::ptr->ai_socktype,
					mSocket::cfg::ptr->ai_protocol);

				if (mSocket::cfg::ConnectSocket == INVALID_SOCKET) {
#ifdef _DEBUG
					printf("socket failed with error: %ld\n\n", WSAGetLastError());
#endif
					WSACleanup();
					continue;
				}

				// Connect to server.
				mSocket::cfg::iResult = connect(mSocket::cfg::ConnectSocket, mSocket::cfg::ptr->ai_addr, (int)mSocket::cfg::ptr->ai_addrlen);
				if (mSocket::cfg::iResult == SOCKET_ERROR) {
					closesocket(mSocket::cfg::ConnectSocket);
					mSocket::cfg::ConnectSocket = INVALID_SOCKET;
					continue;
				}

				freeaddrinfo(mSocket::cfg::result);

				if (mSocket::cfg::ConnectSocket == INVALID_SOCKET) {
#ifdef _DEBUG
					printf("Unable to connect to server!\n\n");
#endif
					WSACleanup();
					mSocket::cfg::socketIsConnected = false;
					continue;
				}

				variables::cheat::logboxLists.push_front("Connected!");
				mSocket::cfg::socketIsConnected = true;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

bool mSocket::sendPacketToServer(const char* data, const char** iError, bool force_send)
{

#ifdef _DEBUG
	std::cout << "TY TO SENT = " << data << std::endl;
#endif

	if (!mSocket::cfg::socketIsConnected)
	{
		*iError = "Socket isn't connected";
		return false;
	}

	mSocket::cfg::iResult = send(mSocket::cfg::ConnectSocket, data, (int)strlen(data), 0);
	if (mSocket::cfg::iResult == SOCKET_ERROR) {

		mSocket::cfg::socketIsConnected				= false;
		//mSocket::cfg::socketReconnect				= true;
		mSocket::cfg::socketReconnect				= false;
		closesocket(mSocket::cfg::ConnectSocket);

		*iError = "mSocket::cfg::iResult == SOCKET_ERROR";

		//WSACleanup();
		return false;
	}

	return true;
}



bool mSocket::initSoket(const char** errStr)
{
	// Initialize Winsock
	if (mSocket::cfg::socketInited)
	{
		*errStr = "Error -> Socket allready initialized";
		return false;
	}

	mSocket::cfg::iResult = WSAStartup(MAKEWORD(2, 2), &mSocket::cfg::wsaData);
	if (mSocket::cfg::iResult != 0) { 
		*errStr = "Error -> Socket init failed" + mSocket::cfg::iResult;
		return false;
	}

	ZeroMemory(&mSocket::cfg::hints, sizeof(mSocket::cfg::hints));
	mSocket::cfg::hints.ai_family = AF_INET;
	mSocket::cfg::hints.ai_socktype = SOCK_STREAM;
	mSocket::cfg::hints.ai_protocol = IPPROTO_TCP;

	mSocket::cfg::socketInited = true;
	//mSocket::cfg::socketThreadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mSocket::socketThread, 0, 0, 0);
	return true;
}


bool mSocket::cleanup(bool fuck)
{
	mSocket::cfg::closingTO				= true;
	mSocket::cfg::socketReconnect		= false;
	mSocket::cfg::socketIsConnected		= false;

	//printf("Cleanup called\n\n");

	// Cleanup
	closesocket(mSocket::cfg::ConnectSocket);
	WSACleanup();

	mSocket::cfg::ConnectSocket = INVALID_SOCKET;

	if (fuck)
	{
		variables::NetworkUser::fuckThisCheat = true;
		mSocket::cfg::authed = false;
		mSocket::cfg::socketIsConnected = false;
		mSocket::cfg::grabbedToken = "";
	}

	//if (mSocket::cfg::socketThreadHandle)
	//	CloseHandle(mSocket::cfg::socketThreadHandle);

	return true;
}



bool mSocket::getHWID(std::string* iError, std::string* resultHWID)
{
	/*UCHAR szFileSys[255], szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;

	//request information of Volume C, using GetVolumeInformatioA winapi function
	bool fail = GetVolumeInformationA("C:\\", (LPSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPSTR)szFileSys, 255);
	if (!fail) {
		*iError = "Error : Not elevated (please run this with admin rights)";
		return false;
	}

	std::stringstream hwidstream;
	hwidstream << std::hex << dwSerial; // convert volume serial to hex

	std::string HWID = hwidstream.str();*/


	unsigned long s1 = 0;
	unsigned long s2 = 0;
	unsigned long s3 = 0;
	unsigned long s4 = 0;
	__asm
	{
		mov eax, 00h
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	__asm
	{
		mov eax, 01h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s3, edx
		mov s4, ecx
	}

	static char buf[100];
	sprintf_s(buf, "%08X%08X%08X%08X", s1, s2, s3, s4);

	*resultHWID = buf;

	return true;
}