#include "MovePlayerCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "EngineTime.h"
#include "PhysicsBody.h"

using namespace Engine;

MovePlayerCommand::MovePlayerCommand(GameObject* pActor, InputType2D inputType, float speed):
	Engine::GameActorCommand2D{ pActor, inputType }
	, m_Speed{speed}
	, m_pBody{ pActor->GetComponent<PhysicsBody>() }
{
}

void MovePlayerCommand::Execute(const Engine::Vector2& value)
{
	if (m_pBody)
	{
		m_pBody->Velocity = value * m_Speed;
	}
}
