#pragma once
#include "Observers.h"
#include <string>

class GameObject;
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
	WinGameAchievement(int id, GameObject* pPlayer);

	// Inherited via IObserver
	void OnNotify() override;

	bool Validate() const override { return false; }
	
private:
	ScoreComponent* m_pScoreComponent;
	GameObject* m_pPlayer;
};

