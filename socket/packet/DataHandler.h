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
	void FirstAuth(std::string fullData);
	void UserAuth(std::string fullData);
	void ChatMessageSent(std::string fullData);
};