#pragma once
#include "Component.h"

class EnemyMovement;
class GameLoop;

class EnemyBrain final : public Engine::Component
{
public:
	EnemyBrain(Engine::GameObject* pOwner, float pathUpdateTime, GameLoop* const pGame);
	virtual ~EnemyBrain() = default;

	void Start() override;
	void Update() override;
private:
	float m_PathUpdateRate;
	float m_PathUpdateTimer;
	Engine::GameObject* m_pTargetPlayer;
	EnemyMovement* m_pEnemyMovement;
	GameLoop* const m_pGame;
};

