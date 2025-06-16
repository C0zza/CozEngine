#pragma once

#include <filesystem>
#include <fstream>

#include "Misc/Logging.h"
#include "json.hpp"

class LClass;
class LResource;

class LSavable
{
public:
	static void SaveAssetToDisk(const std::string& Asset, LClass* Class, void* Address);
	static void LoadAssetFromDisk(const std::string& Asset, LClass* Class, void* Address);

	static bool DoesAssetExist(const std::string& Asset);

	template<typename T>
	static void SaveAssetToDisk(const std::string& Asset, const T& Object)
	{
		CreateDirectoryForAsset(Asset);

		std::fstream AssetFile(Asset, std::ios::out | std::ios::trunc);

		if (AssetFile.is_open())
		{
			nlohmann::json Data = Object;
			AssetFile << Data.dump(2);
			AssetFile.close();
		}
		else
		{
			Log(LLogLevel::ERROR, "LSavable::SaveAssetToDisk - Failed to save " + Asset + ".");
		}
	}

	template<typename T>
	static void LoadAssetFromDisk(const std::string& Asset, T& Object)
	{
		std::ifstream AssetFile(Asset);
		if (AssetFile.is_open())
		{
			nlohmann::json Data;
			AssetFile >> Data;
			Object = Data.get<T>();
		}
		else
		{
			Log(LLogLevel::ERROR, "LSavable::LoadAssetFromDisk - Failed to load " + Asset + ".");
		}
	}

	static void CreateDirectoryForAsset(const std::string& AssetPath)
	{
		std::size_t EndOfDirectory = AssetPath.find_last_of("/\\");
		if (EndOfDirectory != std::string::npos)
		{
			std::string Directory = AssetPath.substr(0, EndOfDirectory);
			try
			{
				std::filesystem::create_directories(Directory);
			}
			catch(std::filesystem::filesystem_error e)
			{
				Log(LLogLevel::ERROR, "LSavable::CreateDirectoryForAsset - " + std::string(e.what()));
			}
		}
		else
		{
			Log(LLogLevel::WARNING, "LSavable::CreateDirectoryForAsset - Invalid AssetPath provided: " + AssetPath + ". Directory was not created.");
		}
	}
};
