#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "json.hpp"

typedef nlohmann::json LSerializedData;

class LSerializable
{
public:
	LSerializedData GetData(const std::string& Asset);
};

// For classes that will be saved and loaded from their own file.
class LSavable : public LSerializable
{
public:
	template<typename T>
	void SaveAssetToDisk(const std::string& Asset, const T& Object)
	{
		// std::fstream::trunc should clear the file
		std::fstream AssetFile(Asset, std::fstream::out, std::fstream::trunc);
		
		if (AssetFile.is_open())
		{
			nlohmann::json Data = Object;
			AssetFile << Data.dump(2);
			AssetFile.close();
		}
	}

	template<typename T>
	void LoadAssetFromDisk(const std::string& Asset, T& Object)
	{
		std::ifstream AssetFile(Asset);
		if (AssetFile.is_open())
		{
			nlohmann::json Data;
			AssetFile >> Data;
			Object = Data.get<T>();
		}
	}
};
