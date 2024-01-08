#include "Serializable.h"

#include <cassert>
#include <iostream>
#include <fstream>

LSerializedData LSerializable::GetData(const std::string& Asset)
{
	std::ifstream File;
	File.open(Asset);

	LSerializedData Data;
	if (File.is_open())
	{
		Data = LSerializedData::parse(File);
	}

	return Data;
}