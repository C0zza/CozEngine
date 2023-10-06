#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>

#include "ComponentSystem.h"
#include "ECSDefinitions.h"

class LSystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* TypeName = typeid(T).name();

		assert(Systems.find(TypeName) == Systems.end());

		auto System = std::make_shared<T>();
		Systems.insert({ TypeName, System });
		return System;
	}

	template<typename T>
	void SetSignature(LSignature Signature)
	{
		const char* TypeName = typeid(T).name();

		assert(Systems.find(TypeName) != Systems.end());
		
		Signatures.insert({ TypeName, Signature });
	}

	void EntityDestroyed(LEntity Entity)
	{
		for (auto const& Pair : Systems)
		{
			auto const& System = Pair.second;
			System->Entities.erase(Entity);
		}
	}

	void EntitySignatureChanged(LEntity Entity, LSignature EntitySignature)
	{
		for (auto const& Pair : Systems)
		{
			auto const& Type = Pair.first;
			auto const& System = Pair.second;
			auto const& SystemSignature = Signatures[Type];

			if ((EntitySignature & SystemSignature) == SystemSignature)
			{
				System->Entities.insert(Entity);
			}
			else
			{
				System->Entities.erase(Entity);
			}
		}
	}

private:
	std::unordered_map<const char*, LSignature> Signatures;
	std::unordered_map<const char*, std::shared_ptr<LComponentSystem>> Systems;
};

