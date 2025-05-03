#pragma once
#include <string>
#include "Component.h"
#include <functional>
#include <vector>
#include <type_traits>
#include <concepts>

namespace Engine
{
	class GameObject;


	template<typename ValueType, typename CallBack>
	class ValueWatchComponent final : public Component
	{
	public:
		ValueWatchComponent(Engine::GameObject* pOwner);
		ValueWatchComponent(Engine::GameObject* pOwner, ValueType* watchValue);
		ValueWatchComponent(Engine::GameObject* pOwner, ValueType* watchValue, CallBack callEvent);

		void SetWatchValue(ValueType* watchValue);
		void AddListener(CallBack callEvent);
		void RemoveListener(CallBack callEvent);
		void Update() override;

	private:
		ValueType* m_WatchValue;
		ValueType m_OldValue;
		std::vector<CallBack> m_Events;
		bool m_Filled{ false };

	};
}

template<typename ValueType, typename CallBack>
inline Engine::ValueWatchComponent<ValueType, CallBack>::ValueWatchComponent(Engine::GameObject* pOwner):
	Component{ pOwner },
	m_WatchValue{nullptr}
{
}

template<typename ValueType, typename CallBack>
inline Engine::ValueWatchComponent<ValueType, CallBack>::ValueWatchComponent(Engine::GameObject* pOwner, ValueType* watchValue):
	Component{ pOwner },
	m_WatchValue{ watchValue },
	m_OldValue{ *m_WatchValue }
	
{
}

template<typename ValueType, typename CallBack>
inline Engine::ValueWatchComponent<ValueType, CallBack>::ValueWatchComponent(Engine::GameObject* pOwner, ValueType* watchValue, CallBack callEvent):
	Component{ pOwner },
	m_WatchValue{ watchValue },
	m_OldValue{ *m_WatchValue },
	m_Filled{ true }
{
	m_Events.emplace_back(callEvent);
}



template<typename ValueType, typename CallBack>
inline void Engine::ValueWatchComponent<ValueType, CallBack>::SetWatchValue(ValueType* watchValue)
{
	m_WatchValue = watchValue;
	m_OldValue = *m_WatchValue;

	if (!m_Filled && m_Events.size() > 0) m_Filled = true;
}

template<typename ValueType, typename CallBack>
inline void Engine::ValueWatchComponent<ValueType, CallBack>::AddListener(CallBack callEvent)
{
	m_Events.emplace_back(callEvent);

	
	if (!m_Filled) m_Filled = true;
}

template<typename ValueType, typename CallBack>
inline void Engine::ValueWatchComponent<ValueType, CallBack>::RemoveListener(CallBack callEvent)
{
	if (m_Events.size() <= 0) return;

	auto it{ std::find(m_Events.begin(), m_Events.end(), callEvent) };
	if (it == m_Events.end()) return;

	m_Events.erase(it);
}

template<typename ValueType, typename CallBack>
inline void Engine::ValueWatchComponent<ValueType, CallBack>::Update()
{
	if (!m_Filled) return;

	if (*m_WatchValue != m_OldValue)
	{
		for (CallBack callBack : m_Events)
		{
			callBack(*m_WatchValue);
		}
	}
	m_OldValue = *m_WatchValue;
}



