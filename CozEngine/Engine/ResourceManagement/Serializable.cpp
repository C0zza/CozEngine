#include "stdafx.h"

#include "Serializable.h"

#include "Reflection/Class.h"
#include "ResourceManagement/Resource.h"

void LSavable::SaveAssetToDisk(const std::string& Asset, LClass* Class, LResource* Address)
{
	CreateDirectoryForAsset(Asset);

	std::fstream AssetFile(Asset, std::ios::out | std::ios::trunc);

	if (AssetFile.is_open())
	{
		nlohmann::json Data;

		Class->SerializeAddress(reinterpret_cast<uint8_t*>(Address), Data);

		AssetFile << Data.dump(2);
		AssetFile.close();
	}
	else
	{
		Log(LLogLevel::ERROR, "LSavable::SaveAssetToDisk - Failed to save " + Asset + ".");
	}
}

void LSavable::LoadAssetFromDisk(const std::string& Asset, LClass* Class, LResource* Address)
{
	std::ifstream AssetFile(Asset);
	if (AssetFile.is_open())
	{
		nlohmann::json Data;
		AssetFile >> Data;

		Class->DeserializeAddress(reinterpret_cast<uint8_t*>(Address), Data);
	}
	else
	{
		Log(LLogLevel::ERROR, "LSavable::LoadAssetFromDisk - Failed to load " + Asset + ".");
	}
}
