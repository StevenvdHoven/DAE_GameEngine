#include "Projectile.h"
#include "PhysicsBody.h"
#include "BoxCollider2D.h"
#include "GameObject.h"

using namespace Engine;

Projectile::Projectile(Engine::GameObject* pOwner, int damage, float speed, int bounces, LayerMask ignoreLayer):
	Engine::Component(pOwner)
	, m_Damage{ damage }
	, m_Bounces{ bounces }
	, m_IgnoreLayer{ignoreLayer}
	, m_Speed{ speed }
	, m_pSender{ nullptr }
	, m_pBody{ nullptr }
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
	auto pCollider = other->GetComponent<Collider>();
	if (other == m_pSender || pCollider == nullptr || pCollider->GetLayerMask() == m_IgnoreLayer)
	{
		return;
	}

	m_Bounces--;
	if (m_Bounces <= 0)
	{
		Destroy(GetGameObject());
	}
	else
	{
		auto pBoxCollider = dynamic_cast<BoxCollider2D*>(pCollider);
		if (!pBoxCollider)
			return;

		Vector2 projectilePos = GetGameObject()->GetTransform()->GetWorldLocation();
		Vector2 targetPos = other->GetTransform()->GetWorldLocation();
		Vector2 targetSize = pBoxCollider->GetSize();

		float left = targetPos.x;
		float right = targetPos.x + targetSize.x;
		float top = targetPos.y;
		float bottom = targetPos.y + targetSize.y;

		float dx = std::min(std::abs(projectilePos.x - left), std::abs(projectilePos.x - right));
		float dy = std::min(std::abs(projectilePos.y - top), std::abs(projectilePos.y - bottom));

		if (dx < dy)
			m_pBody->Velocity = { -m_pBody->Velocity.x, m_pBody->Velocity.y }; // Horizontal flip
		else
			m_pBody->Velocity = { m_pBody->Velocity.x, -m_pBody->Velocity.y }; // Vertical flip
	}
}
