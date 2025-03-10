#pragma once
#include <vector>

class Subject;
class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void OnNotify() = 0;
};

class Subject final
{
public:
	Subject() = default;

	void AddObserver(IObserver* observer);
	void RemoveObserver(IObserver* observer);

	void Notify();

private:
	std::vector<IObserver*> m_Observers{};
};

