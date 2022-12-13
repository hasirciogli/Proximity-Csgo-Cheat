#include "Packet.h"
#include <string>

#include <nlohmann/json.hpp>

class CDataHandler
{
public:
	std::string data = "";

	CDataHandler(std::string tData)
	{
		this->data = tData;
	}

	void Handle();
};

class CDataHandlerFuncs
{
public:
	void FirstAuth(std::string fullData);
	void UserAuth(std::string fullData);
};