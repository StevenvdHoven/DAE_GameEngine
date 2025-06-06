#include "PlayerShootCommand.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Engine;

PlayerShootCommand::PlayerShootCommand(Engine::GameObject* pOwner, const Engine::Vector2& offset, std::function<Engine::GameObject*()> projectileFunction) :
	GameActorValueCommand(pOwner,Engine::ValueCommand<float>::InputType::RIGHT_TRIGGER),
	m_Offset{offset},
	m_ProjectileFactoryFunction{ projectileFunction }
{
}

void PlayerShootCommand::Execute(const float&)
{
	auto projectile{ m_ProjectileFactoryFunction() };
	Vector2 offset{ GetActor()->GetTransform()->GetForward() * m_Offset.x + GetActor()->GetTransform()->GetUp() * m_Offset.y };
	Vector2 spawnLocation{ GetActor()->GetTransform()->GetWorldLocation() };

	projectile->GetTransform()->SetWorldLocation(spawnLocation + offset);

	Projectile* projectileComponent{ projectile->GetComponent<Projectile>() };
	if (projectileComponent)
	{
		Vector2 direction{ GetActor()->GetTransform()->GetForward() };
		projectileComponent->Launch(GetActor()->GetTransform()->GetParent(), direction);
	}
}

