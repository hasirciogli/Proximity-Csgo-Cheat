#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

class CDataHandler
{
public:
	std::string data = "";

	void Handle();
};

class CDataHandlerFuncs
{
public:
	void FunOk(std::string fullData);
	void NeedUserAuth(std::string fullData);
	void UserAuth(std::string fullData);
	void ChatMessageSent(std::string fullData);
	void ConfigCreate(std::string fullData);
	void ConfigLoad(std::string fullData);
	void ConfigRefresh(std::string fullData);
};