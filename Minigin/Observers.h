#pragma once
#include <vector>

namespace Engine
{
	class Subject;
	class Component;
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(Component* sender) = 0;
	};

	class Subject final
	{
	public:
		Subject() = default;

		void AddObserver(IObserver* observer);
		void RemoveObserver(IObserver* observer);

		void Notify(Component* sender = nullptr);

	private:
		std::vector<IObserver*> m_Observers{};
	};
}



