#include "Observers.h"
#include <algorithm>

using namespace Engine;

void Subject::AddObserver(IObserver* observer)
{
	if (!observer) return;

	m_Observers.emplace_back(observer);
}

void Subject::RemoveObserver(IObserver* observer)
{
	auto it{ std::find(m_Observers.begin(),m_Observers.end(),observer) };
	if (it == m_Observers.end()) return;

	m_Observers.erase(it);
}

void Subject::Notify(Component* sender)
{
	for (auto observer : m_Observers)
	{
		observer->OnNotify(sender);
	}
}
