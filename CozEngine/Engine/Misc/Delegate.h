#pragma once

#include <functional>
#include <set>

#include "Misc/Logging.h"

#define MAX_BINDINGS 10

class LDelegateBase;

class LDelegateEventHandle
{
	friend LDelegateBase;
public:
	~LDelegateEventHandle();

	void Reset();

private:
	LDelegateBase* BoundDelegate = nullptr;
};

class LDelegateBase
{
	friend LDelegateEventHandle;
protected:
	void InitDelegateEventHandle(LDelegateEventHandle& DelegateEventHandle) { DelegateEventHandle.BoundDelegate = this; }

	virtual void RemoveEvent(LDelegateEventHandle* DelegateEventHandle) = 0;

	std::map<LDelegateEventHandle*, int> HandleToIndexMap;
};

template<typename... Args>
class LDelegate : public LDelegateBase
{
public:
	LDelegate();
	~LDelegate();

	void Broadcast(Args...);
	
	template<typename TCallerType, typename TFuncRef>
	void AddEvent(TCallerType* Caller, TFuncRef&& FuncRef, LDelegateEventHandle& DelegateEventHandle);

	virtual void RemoveEvent(LDelegateEventHandle* DelegateEventHandle) final;

private:
	std::vector<std::function<void(Args...)>> Events;
	std::map<void*, LDelegateEventHandle*> FuncToHandleMap;
};

template<typename ...Args>
inline LDelegate<Args...>::LDelegate()
{
	Events.reserve(MAX_BINDINGS);
}

template<typename ...Args>
inline LDelegate<Args...>::~LDelegate()
{
	for (auto& Pair : HandleToIndexMap)
	{
		if (Pair.first)
		{
			Pair.first->Reset();
		}
	}
}

template<typename ...Args>
inline void LDelegate<Args...>::Broadcast(Args... args)
{
	for (std::function<void(Args...)>& Event : Events)
	{
		Event(args...);
	}
}

template<typename ...Args>
inline void LDelegate<Args...>::RemoveEvent(LDelegateEventHandle* DelegateEventHandle)
{
	if (!HandleToIndexMap.contains(DelegateEventHandle))
	{
		return;
	}

	const int IndexToRemove = HandleToIndexMap.at(DelegateEventHandle);

	if (Events.empty() || IndexToRemove > Events.size() - 1)
	{
		return;
	}

	const size_t LastIndex = Events.size() - 1;

	if (IndexToRemove != LastIndex)
	{
		std::function<void(Args...)>* LastRef = &Events[LastIndex];
		std::function<void(Args...)>* RemovedRef = &Events[IndexToRemove];

		HandleToIndexMap[FuncToHandleMap[LastRef]] = IndexToRemove;
		HandleToIndexMap.erase(FuncToHandleMap[RemovedRef]);

		FuncToHandleMap[RemovedRef] = FuncToHandleMap[LastRef];
		FuncToHandleMap.erase(LastRef);

		Events[IndexToRemove] = Events[LastIndex];
	}
	else
	{
		HandleToIndexMap.erase(DelegateEventHandle);
		FuncToHandleMap.erase(&Events[IndexToRemove]);
	}

	Events.pop_back();
}

template<typename ...Args>
template<typename TCallerType, typename TFuncRef>
inline void LDelegate<Args...>::AddEvent(TCallerType* Caller, TFuncRef&& FuncRef, LDelegateEventHandle& DelegateEventHandle)
{
	if (!Caller)
	{
		return;
	}

	if (Events.size() >= MAX_BINDINGS)
	{
		Log(LLogLevel::ERROR, "LDelegate<Args...>::AddEvent - MAX_BINDINGS reached. Unable to add new binding.");
		return;
	}

	// TODO: We need to somehow define multiple placeholders here
	Events.emplace_back(std::bind(FuncRef, Caller, std::placeholders::_1));

	HandleToIndexMap.emplace(&DelegateEventHandle, Events.size() - 1);
	FuncToHandleMap.emplace((void*)&Events.back(), &DelegateEventHandle);

	InitDelegateEventHandle(DelegateEventHandle);
}
