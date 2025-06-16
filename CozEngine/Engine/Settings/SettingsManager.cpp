#include "stdafx.h"

#include "SettingsManager.h"

#include "Globes.h"
#include "ResourceManagement/Serializable.h"
#include "Reflection/Class.h"

namespace
{
	const char* SettingsPath = "Engine/Settings/Saved/";
}

void LSettingsManager::Initialize()
{
	LClass* SettingsClass = LSettings::StaticClass();
	assert(SettingsClass);

	for (LClass* Class : SettingsClass->GetChildClasses())
	{
		if (!Class)
		{
			continue;
		}

		assert(!SettingsClasses.contains(Class));

		LSettings* SettingsObject = Class->CreateObject<LSettings>();
		assert(SettingsObject);

		SettingsClasses.try_emplace(Class, SettingsObject);

		LoadSettings(Class);
	}
}

void LSettingsManager::ForEachSettingsClass(std::function<void(LClass*, LSettings*)> Func)
{
	for (auto& Pair : SettingsClasses)
	{
		Func(Pair.first, Pair.second.get());
	}
}

void LSettingsManager::SaveSettings(LClass* SettingsClass)
{
	assert(SettingsClass);
	LSavable::SaveAssetToDisk(GetSavedSettingsPath(SettingsClass), SettingsClass, SettingsClasses.at(SettingsClass).get());
}

void LSettingsManager::LoadSettings(LClass* SettingsClass)
{
	assert(SettingsClass);

	const std::string SettingsPath = GetSavedSettingsPath(SettingsClass);
	if (LSavable::DoesAssetExist(SettingsPath))
	{
		LSavable::LoadAssetFromDisk(SettingsPath, SettingsClass, SettingsClasses.at(SettingsClass).get());
	}
}

std::string LSettingsManager::GetSavedSettingsPath(LClass* SettingsClass)
{
	assert(SettingsClass);
	assert(SettingsClasses.contains(SettingsClass));

	const std::string TypeName = SettingsClass->GetTypeName().data();
	return SettingsPath + TypeName + ".txt";
}
