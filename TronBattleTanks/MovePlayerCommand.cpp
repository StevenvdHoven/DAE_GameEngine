#include "MovePlayerCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "EngineTime.h"
#include "PhysicsBody.h"
#include "Collider.h"
#include "ServiceLocator.h"
#include "PhysicsSystem.h"
#include "PlayerMovement.h"

using namespace Engine;

MovePlayerCommand::MovePlayerCommand(GameObject* pActor, InputType inputType):
	Engine::GameActorCommand2D{ pActor, inputType },
	m_pPlayerMovement{ pActor->GetComponent<PlayerMovement>() }
{
}

void MovePlayerCommand::Execute(const Engine::Vector2& value)
{
	if (m_pPlayerMovement)
	{
		if (value == Vector2{ 0.f, 0.f }) {
			m_pPlayerMovement->SetDirection(value);
			return;
		}
			

		Engine::Vector2 filterValue{ value };
		if (filterValue.x != 0)
		{
			filterValue.y = 0;
		}
		else if (filterValue.y != 0)
		{
			filterValue.x = 0;
		}

		m_pPlayerMovement->SetDirection(filterValue);
	}
}
