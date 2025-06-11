#pragma once
#include <memory>
#include "Component.h"
#include "EnemyShootCommand.h"

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
	EnemyBrain(Engine::GameObject* pOwner, EnemyType enemyType, float pathUpdateTime, float shootRate, GameLoop* const pGame, std::unique_ptr<EnemyShootCommand>&& shootCommand);
	virtual ~EnemyBrain() = default;

	void Start() override;
	void Update() override;
	
	std::string GetTypeName() const override;
private:
	void CheckForShoot();

	EnemyType m_EnemyType;
	float m_PathUpdateRate;
	float m_PathUpdateTimer;
	Engine::GameObject* m_pTargetPlayer;
	EnemyMovement* m_pEnemyMovement;
	GameLoop* const m_pGame;

	float m_ShootRate;
	float m_ShootTimer;
	std::unique_ptr<EnemyShootCommand> m_ShootCommand;
	Engine::GameObject* m_TestHit;
};

