#pragma once
#include <memory>
#include "Component.h"
#include "EnemyShootCommand.h"

namespace Engine
{
	class Graph;
}

class EnemyMovement;
class GameLoop;

enum struct EnemyType
{
	TANK,
	RECOGNIZER
};

class EnemyBrain final : public Engine::Component
{
public:
	EnemyBrain(Engine::GameObject* pOwner, EnemyType enemyType, Engine::Graph* pGraph, float pathUpdateTime, float shootRate, GameLoop* const pGame, std::unique_ptr<EnemyShootCommand>&& shootCommand);
	virtual ~EnemyBrain() = default;

	void Start() override;
	void Update() override;
	
	void OnCollisionEnter(Engine::GameObject* other) override;

	void RefreshPlayer();

	std::string GetTypeName() const override;
private:
	void CheckForShoot();

	EnemyType m_EnemyType;
	float m_PathUpdateRate;
	float m_PathUpdateTimer;
	Engine::GameObject* m_pTargetPlayer;
	EnemyMovement* m_pEnemyMovement;
	GameLoop* const m_pGame;
	Engine::Graph* m_pGraph;

	float m_ShootRate;
	float m_ShootTimer;
	std::unique_ptr<EnemyShootCommand> m_ShootCommand;
	Engine::GameObject* m_TestHit;
};

