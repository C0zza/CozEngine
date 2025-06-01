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
	void RawResetDelegateEventHandle(LDelegateEventHandle& DelegateEventHandle) { DelegateEventHandle.BoundDelegate = nullptr; }

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
	
	void Reset();

	template<typename TCallerType, typename TFuncRef>
	void AddEvent(TCallerType* Caller, TFuncRef&& FuncRef, LDelegateEventHandle& DelegateEventHandle);

	virtual void RemoveEvent(LDelegateEventHandle* DelegateEventHandle) final;

private:
	std::vector<std::function<void(Args...)>> Events;
	std::map<void*, LDelegateEventHandle*> FuncToHandleMap;

	template <typename Func, typename Caller, std::size_t... Is>
	auto bind_with_placeholders(Func func, Caller caller, std::index_sequence<Is...>)
	{
		return std::bind(func, caller, (std::placeholders::_1 + Is)...);
	}

	template <typename Func, typename Caller, typename... Args>
	auto bind_event(Func func, Caller caller)
	{
		static_assert(sizeof...(Args) <= 20, "std::bind supports at most 20 placeholders");
		return bind_with_placeholders(func, caller, std::index_sequence_for<Args...>{});
	}
};

template<typename ...Args>
inline LDelegate<Args...>::LDelegate()
{
	Events.reserve(MAX_BINDINGS);
}

template<typename ...Args>
inline LDelegate<Args...>::~LDelegate()
{
	Reset();
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
inline void LDelegate<Args...>::Reset()
{
	for (auto& Pair : HandleToIndexMap)
	{
		if (Pair.first)
		{
			RawResetDelegateEventHandle(*Pair.first);
		}
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

	Events.emplace_back(bind_event<TFuncRef, TCallerType*, Args...>(FuncRef, Caller));

	HandleToIndexMap.emplace(&DelegateEventHandle, static_cast<int>(Events.size() - 1));
	FuncToHandleMap.emplace((void*)&Events.back(), &DelegateEventHandle);

	InitDelegateEventHandle(DelegateEventHandle);
}
