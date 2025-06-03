#pragma once
#include "Component.h"

class EnemyMovement;
class GameLoop;

class EnemyBrain final : public Engine::Component
{
public:
	EnemyBrain(Engine::GameObject* pOwner, float pathUpdateTime, GameLoop* pGame);
	virtual ~EnemyBrain() = default;

	void Start() override;
	void Update() override;
private:
	float m_PathUpdateRate;
	float m_PathUpdateTimer;
	EnemyMovement* m_pEnemyMovement;
	GameLoop* const m_pGame;
};

