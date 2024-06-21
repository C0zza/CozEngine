#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "ECS/ECSDefinitions.h"
#include "Misc/Logging.h"
#include "Misc/TypeIdGenerator.h"
#include "Subsystem.h"

class LSubsystem;

class LSubsystemCollection
{
public:
	~LSubsystemCollection();

	template<typename T>
	T* AddSubsystem()
	{
		LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<T>();
		if (Subsystems.contains(TypeID))
		{
			Log(LLogLevel::INFO, "LSubsystemCollection::AddSubsystem - Subsystem, " + std::string(typeid(T).name()) + ", already exists.");
			return dynamic_cast<T*>(Subsystems.at(TypeID).get());
		}

		T* Subsystem = new T();
		Subsystems.insert({ TypeID, std::unique_ptr<T>(Subsystem) });

#if defined(_DEBUG)
		if (!Subsystems[TypeID]->HasRequiredSubsystems())
		{
			Log(LLogLevel::ERROR, "LSubsystemCollection::AddSubsystem - Unable to add " + std::string(typeid(T).name()) + " as subsystem dependencies have not been added.");
			return nullptr;
		}
#endif

		Subsystems.at(TypeID)->Initialize();
		return Subsystem;
	}

	template<typename T>
	T* GetSubsystem(const bool bAddIfNotFound = false)
	{
		LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<T>();
		if (Subsystems.contains(TypeID))
		{
			if (T* Subsystem = dynamic_cast<T*>(Subsystems.at(TypeID).get()))
			{
				return Subsystem;
			}
			else
			{
				Log(LLogLevel::INFO, "LSubsystemCollection::GetSubsystem<T> - Found subsystem type in collection but failed to cast.");
			}
		}
		else if(bAddIfNotFound)
		{
			T* Subsystem = new T();
			if (!Subsystem->HasRequiredSubsystems())
			{
				delete Subsystem;
				Subsystem = nullptr;
				return nullptr;
			}
			else
			{
				Subsystems.insert({ TypeID, std::unique_ptr<T>(Subsystem) });
				Subsystems.at(TypeID)->Initialize();
				return Subsystem;
			}
		}

		return nullptr;
	}

	void ForEachSubsystem(std::function<void(LSubsystem*)>&& Func);

private:
	std::unordered_map<LIDType, std::unique_ptr<LSubsystem>> Subsystems;
};

