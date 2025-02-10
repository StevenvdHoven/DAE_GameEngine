#pragma once
#include <string>
#include "Component.h"
#include <functional>

template<typename ValueType, typename CallBack>
class ValueWatchComponent final : public Component
{
public:
    ValueWatchComponent(ValueType& watchValue, CallBack callEvent);

    void Update() override;

private:
    ValueType& m_WatchValue;
    ValueType m_OldValue;
    CallBack m_CallEvent;
};


template<typename ValueType, typename CallBack>
inline ValueWatchComponent<ValueType, CallBack>::ValueWatchComponent(ValueType& watchValue, CallBack callEvent):
    m_WatchValue{ watchValue },
    m_OldValue{ m_WatchValue },
    m_CallEvent{ callEvent }
{
}

template<typename ValueType, typename CallBack>
inline void ValueWatchComponent<ValueType, CallBack>::Update()
{
    if (m_WatchValue != m_OldValue)
    {
        m_CallEvent(m_WatchValue);
    }
    m_OldValue = m_WatchValue;
}



