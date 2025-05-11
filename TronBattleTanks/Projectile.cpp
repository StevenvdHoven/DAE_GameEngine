#include "Projectile.h"
#include "PhysicsBody.h"
#include "GameObject.h"

using namespace Engine;

Projectile::Projectile(Engine::GameObject* pOwner, int damage, float speed, int bounces):
	Engine::Component(pOwner)
	, m_Damage{ damage }
	, m_Speed{ speed }
	, m_Bounces{ bounces }
	, m_pSender{ nullptr }
{
	m_pBody = GetGameObject()->GetComponent<Engine::PhysicsBody>();
	if (m_pBody == nullptr)
	{
		m_pBody = GetGameObject()->AddComponent<Engine::PhysicsBody>();
	}
}

void Projectile::Start()
{
}

void Projectile::Launch(Engine::GameObject* pSender, const Engine::Vector2& direction)
{
	m_pSender = pSender;
	m_pBody->Velocity = direction * m_Speed;
}


void Projectile::OnTriggerEnter(Engine::GameObject* other)
{
	if (other != m_pSender)
	{




	}

	m_Bounces--;
	if (m_Bounces <= 0)
	{
		Destroy(GetGameObject());
	}
	else
	{
		Vector2 direction = GetGameObject()->GetTransform()->GetWorldLocation() - other->GetTransform()->GetWorldLocation();
		direction.Normalize();

		Vector2 normal;
		if (std::abs(direction.x) > std::abs(direction.y))
			normal = { direction.x,0 };
		else
			normal = { 0,direction.y };
		normal.Normalize();

		m_pBody->Velocity = m_pBody->Velocity.Reflect(normal);
	}
}
