#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "json.hpp"

class LSavable
{
public:
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
	}

private:

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
				std::cout << e.what() << "\n";
			}
		}
	}
};
