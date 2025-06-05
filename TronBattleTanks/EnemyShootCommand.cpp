#include "EnemyShootCommand.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Engine;

EnemyShootCommand::EnemyShootCommand(Engine::GameObject* pActor, const Engine::Vector2& offset, std::function<Engine::GameObject* ()> projectileFunction):
	Engine::GameActorCommand{pActor},
	m_ProjectileFunction{projectileFunction},
	m_Offset{offset}
{
}

void EnemyShootCommand::Execute()
{
	auto const projectile{ m_ProjectileFunction()};
	const Vector2 offset{ GetActor()->GetTransform()->GetForward() * m_Offset.x + GetActor()->GetTransform()->GetUp() * m_Offset.y };
	const Vector2 spawnLocation{ GetActor()->GetTransform()->GetWorldLocation() };

	projectile->GetTransform()->SetWorldLocation(spawnLocation + offset);

	Projectile* const projectileComponent{ projectile->GetComponent<Projectile>() };
	if (projectileComponent)
	{
		const Vector2 direction{ GetActor()->GetTransform()->GetForward() };
		const float orientation{ std::atan2(direction.y,direction.x) * (180.f / 3.14f) };
		projectile->GetTransform()->SetLocalRotation(orientation);
		projectileComponent->Launch(GetActor()->GetTransform()->GetParent(), direction);
	}
}
