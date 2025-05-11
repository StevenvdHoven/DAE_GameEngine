#pragma once
#include "Component.h"

namespace Engine
{
	struct Vector2;
	class PhysicsBody;
}

class Projectile : public Engine::Component
{
public:
	Projectile(Engine::GameObject* pOwner, int damage, float speed, int bounces);
	~Projectile() override = default;

	void Start() override;
	void Launch(Engine::GameObject* pSender, const Engine::Vector2& direction);

	void OnTriggerEnter(Engine::GameObject* other) override;
private:
	int m_Damage;
	float m_Speed;
	int m_Bounces;
	Engine::GameObject* m_pSender;
	Engine::PhysicsBody* m_pBody;
};

