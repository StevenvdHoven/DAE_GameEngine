#pragma once
#include "Component.h"

namespace Engine
{
	struct Vector2;
	class PhysicsBody;
	class Collider;
	enum struct LayerMask;
}

enum struct EProjectileTarget
{
	PLAYER,
	ENEMY
};

class Projectile : public Engine::Component
{
public:
	Projectile(Engine::GameObject* pOwner, EProjectileTarget targetType, int damage, float speed, int bounces,float lifeTime, Engine::LayerMask ignoreLayer);
	~Projectile() override = default;

	void Start() override;
	void Update() override;
	void Launch(Engine::GameObject* pSender, const Engine::Vector2& direction);

	void OnTriggerEnter(Engine::GameObject* other) override;
private:
	bool HandlePlayer(Engine::GameObject* other);
	bool HandleEnemy(Engine::GameObject* other);

	EProjectileTarget m_TargetType;
	int m_Damage;
	int m_Bounces;
	float m_LifeTimer;
	Engine::LayerMask m_IgnoreLayer;
	float m_Speed;
	Engine::GameObject* m_pSender;
	Engine::PhysicsBody* m_pBody;

	// Inherited via Component
	std::string GetTypeName() const override;
};

