#include "PlayerShootCommand.h"
#include "Projectile.h"

using namespace Engine;

PlayerShootCommand::PlayerShootCommand(Engine::GameObject* pOwner, std::function<Engine::GameObject*()> projectileFunction) :
	GameActorCommand(pOwner),
	m_ProjectileFactoryFunction(projectileFunction)
{
}

void PlayerShootCommand::Execute()
{
	auto projectile{ m_ProjectileFactoryFunction() };
	Vector2 spawnLocation{ GetActor()->GetTransform()->GetWorldLocation() };

	projectile->GetTransform()->SetWorldLocation(spawnLocation);

	Projectile* projectileComponent{ projectile->GetComponent<Projectile>() };
	if (projectileComponent)
	{
		Vector2 direction{ GetActor()->GetTransform()->GetForward() };
		projectileComponent->Launch(GetActor(), direction);
	}
}
