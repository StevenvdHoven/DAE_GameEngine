#include "Projectile.h"
#include "PhysicsBody.h"
#include "BoxCollider2D.h"
#include "GameObject.h"
#include "EnemyHealthComponent.h"
#include "PlayerHealthComponent.h"
#include "EngineTime.h"
#include "ServiceLocator.h"
#include "SceneManager.h"

using namespace Engine;

Projectile::Projectile(Engine::GameObject* pOwner, EProjectileTarget targetType, int damage, float speed, int bounces, float lifeTime, LayerMask ignoreLayer):
	Engine::Component(pOwner)
	, m_TargetType{targetType}
	, m_Damage{ damage }
	, m_Bounces{ bounces }
	, m_LifeTimer{lifeTime}
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

void Projectile::Update()
{
	m_LifeTimer -= Time::GetInstance().GetDeltaTime();
	if (m_LifeTimer < 0)
	{
		Destroy(GetGameObject());
	}
}

void Projectile::Launch(Engine::GameObject* pSender, const Engine::Vector2& direction)
{
	m_pSender = pSender;
	m_pBody->Velocity = direction * m_Speed;
}


void Projectile::OnTriggerEnter(Engine::GameObject* other)
{
	if (m_pSender == nullptr) return;

	auto pCollider = other->GetComponent<Collider>();
	if (other == m_pSender 
		|| pCollider == nullptr 
		|| pCollider->GetLayerMask() == m_IgnoreLayer)
	{
		return;
	}

	const LayerMask projectileMask{ LayerMask::Projectile };
	if (static_cast<int>(pCollider->GetLayerMask()) == static_cast<int>(projectileMask))
		return;

	if (HandleEnemy(other) || HandlePlayer(other))
	{
		SpawnExplosion();
		Destroy(GetGameObject());
		return;
	}

	SpawnExplosion();
	m_Bounces--;
	if (m_Bounces < 0)
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

bool Projectile::HandlePlayer(Engine::GameObject* other)
{
	if (m_TargetType == EProjectileTarget::ENEMY) return false;

	auto const player{ other->GetComponent<PlayerHealthComponent>() };
	if (player)
	{
		player->TakeDamage(1);
		return true;
	}

	return false;
}

bool Projectile::HandleEnemy(Engine::GameObject* other)
{
	if (m_TargetType == EProjectileTarget::PLAYER) return false;

	auto const enemy{ other->GetComponent<EnemyHealthComponent>() };
	if (enemy)
	{
		enemy->TakeDamage(1);
		return true;
	}

	return false;
}

void Projectile::SpawnExplosion()
{
	auto result{ Engine::EnginePrefabFactory::LoadPrefabs("bullet_particle.json") };
	result.Parent->GetTransform()->SetWorldLocation(GetGameObject()->GetTransform()->GetWorldLocation());
	Engine::EnginePrefabFactory::AddPrefabToScene(std::move(result), SceneManager::GetInstance().GetActiveScene());

}

std::string Projectile::GetTypeName() const
{
	return "Projectile";
}
