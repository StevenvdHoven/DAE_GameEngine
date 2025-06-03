#pragma once
#include "Component.h"

namespace Engine
{
	struct Vector2;
	class PhysicsBody;
	enum struct LayerMask;
}

class Projectile : public Engine::Component
{
public:
	Projectile(Engine::GameObject* pOwner, int damage, float speed, int bounces, Engine::LayerMask ignoreLayer);
	~Projectile() override = default;

	void Start() override;
	void Launch(Engine::GameObject* pSender, const Engine::Vector2& direction);

	void OnTriggerEnter(Engine::GameObject* other) override;
private:
	int m_Damage;
	int m_Bounces;
	Engine::LayerMask m_IgnoreLayer;
	float m_Speed;
	Engine::GameObject* m_pSender;
	Engine::PhysicsBody* m_pBody;
};

