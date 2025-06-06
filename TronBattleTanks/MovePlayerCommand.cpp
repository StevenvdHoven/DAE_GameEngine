#include "MovePlayerCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "EngineTime.h"
#include "PhysicsBody.h"
#include "Collider.h"
#include "ServiceLocator.h"
#include "PhysicsSystem.h"

using namespace Engine;

MovePlayerCommand::MovePlayerCommand(GameObject* pActor, InputType inputType, float speed):
	Engine::GameActorCommand2D{ pActor, inputType }
	, m_Speed{speed}
	, m_pTransform{ pActor->GetComponent<Transform>() }
	, m_pBody{ pActor->GetComponent<PhysicsBody>() }
	, m_pCollider{ pActor->GetComponent<Collider>() }
{
}

void MovePlayerCommand::Execute(const Engine::Vector2& value)
{
	if (m_pBody)
	{
		if (value == Vector2{ 0.f, 0.f }) {
			m_pBody->Velocity = Vector2{ 0.f, 0.f };
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

		const Vector2 futureVelocity = filterValue * m_Speed;

		const Vector2 boxPosition = m_pTransform->GetWorldLocation() + filterValue * 5 + Vector2{ -13.f, -13.f };
		const Vector2 boxSize{ 28,28 };

		if (m_pCollider && m_pCollider->IsEnabled)
		{
			if (ServiceLocator::GetPhysicsSystem().BoxCast(boxPosition, boxSize, m_pCollider,{LayerMask::Wall}))
			{
				return;
			}
		}


		m_pBody->Velocity = futureVelocity;

		

		float rotation = 0.f;
		if (filterValue.y > 0.f)
			rotation = 90.f;
		else if (filterValue.y < 0.f)
			rotation = -90.f;
		else if (filterValue.x > 0.f)
			rotation = 0.f;
		else if (filterValue.x < 0.f)
			rotation = 180.f;

		m_pTransform->SetWorldRotation(rotation);
	}
}
