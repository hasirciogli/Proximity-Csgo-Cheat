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
	void NeedUserAuth(std::string fullData);
	void UserAuth(std::string fullData);
	void ChatMessageSent(std::string fullData);
};