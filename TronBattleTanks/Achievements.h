#pragma once
#include "Observers.h"
#include <string>

namespace Engine
{
	class GameObject;
	class Component;
}

class ScoreComponent;

class Achievement
{
public:
	Achievement(int id) : ID{ id } {}

	virtual bool Validate() const { return true; };

	int GetID() const { return ID; }

protected:
	int ID;
};

class WinGameAchievement : public Achievement, public Engine::IObserver
{
public:
	WinGameAchievement(int id, Engine::GameObject* pPlayer);

	// Inherited via IObserver
	void OnNotify(Engine::Component* sender) override;

	bool Validate() const override { return false; }
	
private:
	ScoreComponent* m_pScoreComponent;
	Engine::GameObject* m_pPlayer;
};

