#include "MovePlayerCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"

Engine::MovePlayerCommand::MovePlayerCommand(GameObject* pActor, InputType2D inputType, float speed):
	GameActorCommand2D{ pActor, inputType }
	, m_Speed{speed}
	, m_pTransform{ pActor->GetComponent<Transform>() }
{
}

void Engine::MovePlayerCommand::Execute(const Vector2& value)
{
	if (m_pTransform)
	{
		m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() + value * m_Speed * Time::GetInstance().GetDeltaTime());
	}
}
