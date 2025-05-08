#include "PlayerShootCommand.h"

using namespace Engine;

PlayerShootCommand::PlayerShootCommand(Engine::GameObject* pOwner, std::function<Engine::GameObject()> projectileFunction) :
	GameActorCommand(pOwner),
	m_ProjectileFactoryFunction(projectileFunction)
{
}

void PlayerShootCommand::Execute()
{
	auto projectile{ m_ProjectileFactoryFunction() };
	Vector2 spawnLocation{ GetActor()->GetTransform()->GetWorldLocation() };

}
