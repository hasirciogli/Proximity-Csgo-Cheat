#include "ProCr.h"

#include <iostream>
#include <random>

#include "dependencies/utilities/console/console.hpp"


int ProCr::getRnd()
{
	return 10 + (rand() % 90);
}

std::string ProCr::RandomString(int ch)
{
	srand((unsigned)time(NULL));
	char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
						  'h', 'i', 'j', 'k', 'l', 'm', 'n',
						  'o', 'p', 'q', 'r', 's', 't', 'u',
						  'v', 'w', 'x', 'y', 'z' };
	std::string result = "";
	for (int i = 0; i < ch; i++)
		result += alpha[rand() % 26];

	return result;
}

void ProCr::GenerateKey(std::string iData, std::string& oData, bool ncr)
{
	std::string tiData = iData;
	std::string datasize = "0";
	if (iData.length() >= 1000)
		datasize = iData.length();
	else if (iData.length() >= 100)
		datasize = "0" + std::to_string(iData.length());
	else if (iData.length() >= 10)
		datasize = "00" + std::to_string(iData.length());
	else if (iData.length() >= 0)
		datasize = "000" + std::to_string(iData.length());

	oData = "";
	oData += datasize;
	oData += tiData;
	this->verifyDat = this->RandomString(10);
	oData += this->verifyDat;

	if (ncr)
		this->Crypt(oData, oData);
}

void ProCr::Crypt(std::string iData, std::string& oData)
{
	std::string tiData = iData;
	oData = std::to_string(this->getRnd());

	for (int i = 0; i < tiData.size(); i++)
	{
		for (int i2 = 0; i2 < this->tFiles.size(); i2++)
		{
			if (this->tFiles.at(i2).source[0] == tiData[i])
			{
				oData += this->tFiles.at(i2).dest;
				oData += std::to_string(this->getRnd());
			}
		}
	}
}

void ProCr::DeCrypt(std::string iData, std::string& oData)
{
	oData = "";
	std::string tiData = iData;

	for (int i = 0; i < iData.length(); i++)
	{
		std::string cStr = "";

		if ((i % 5 == 2))
		{
			cStr += iData[i];
			cStr += iData[i + 1];
			cStr += iData[i + 2];

			for (int i2 = 0; i2 < this->tFiles.size(); i2++)
			{
				if (this->tFiles.at(i2).dest == cStr)
				{
					oData += this->tFiles.at(i2).source[0];
				}
			}
		}
	}
}

std::string ProCr::GetData()
{
	std::string getData = "andjg2hwken8sndensqe3sjmd7fdhwjewmaoe6jakafs9df4ghjklglm0bvnmxndskjf";


	std::string oData = getData;
	this->GenerateKey(getData, oData);

	this->Crypt(oData, oData);

	this->Crypt(oData, oData);

	return oData;
}


bool ProCr::Validate(std::string socketData)
{
	std::string getData = "andjg2hwken8sndensqe3sjmd7fdhwjewmaoe6jakafs9df4ghjklglm0bvnmxndskjf";

	std::string validTdata = socketData;

	std::string oData = "";

	this->DeCrypt(validTdata, oData);

	

	this->DeCrypt(oData, oData);


	if (oData == ("ok" + this->verifyDat + getData))
		return true;
	else
		return false;
}