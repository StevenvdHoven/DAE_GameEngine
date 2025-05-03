#include "MovePlayerCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "EngineTime.h"

using namespace Engine;

MovePlayerCommand::MovePlayerCommand(GameObject* pActor, InputType2D inputType, float speed):
	Engine::GameActorCommand2D{ pActor, inputType }
	, m_Speed{speed}
	, m_pTransform{ pActor->GetComponent<Transform>() }
{
}

void MovePlayerCommand::Execute(const Engine::Vector2& value)
{
	if (m_pTransform)
	{
		m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() + value * m_Speed * Engine::Time::GetInstance().GetDeltaTime());
	}
}
