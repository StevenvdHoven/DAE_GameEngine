#pragma once
#include <string>
#include "Component.h"
#include <functional>

template<typename ValueType>
class ValueWatchComponent final : public Component
{
public:
	ValueWatchComponent(ValueType& watchValue, std::function<void(const std::string&)> callEvent);

	void Update() override;
private:
	ValueType& m_WatchValue;
	ValueType m_OldValue;
	std::function<void(const std::string&)> m_CallEvent;
	
};

template<typename ValueType>
inline ValueWatchComponent<ValueType>::ValueWatchComponent(ValueType& watchValue, std::function<void(const std::string&)> callEvent):
	m_WatchValue{watchValue},
	m_OldValue{m_WatchValue},
	m_CallEvent{callEvent}
{
}

template<typename ValueType>
inline void ValueWatchComponent<ValueType>::Update()
{
	if (m_WatchValue != m_OldValue)
	{
		m_CallEvent(std::to_string(m_WatchValue));
	}
	m_OldValue = m_WatchValue;
}
