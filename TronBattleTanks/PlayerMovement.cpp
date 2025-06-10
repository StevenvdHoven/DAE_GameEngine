#include "PlayerMovement.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysicsSystem.h"
#include "PhysicsBody.h"
#include "ServiceLocator.h"
#include "Vector2.h"

#define BOX_SIZE Engine::Vector2{ 26.f, 26.f }
#define DIRECTION_ERROR 8.f
#define OFFSET_SCALAR 5.f

using namespace Engine;

PlayerMovement::PlayerMovement(Engine::GameObject* pOwner, float speed):
	Engine::Component{ pOwner },
	m_CurrentDirection{ 0.f, 0.f },
	m_Body{ nullptr },
	m_Speed{ speed }
{
}

void PlayerMovement::Start()
{
	m_Body = GetGameObject()->GetComponent<PhysicsBody>();

}

void PlayerMovement::SetDirection(const Engine::Vector2& newDirection)
{
	if (m_CurrentDirection == newDirection)
	{
		return;
	}

	if (CheckDirection(newDirection))
	{
		m_CurrentDirection = newDirection;
		m_Body->Velocity = newDirection * m_Speed;

		if (newDirection != Vector2::Zero())
		{
			float rotation = 0.f;
			if (newDirection.y > 0.f)
				rotation = 90.f;
			else if (newDirection.y < 0.f)
				rotation = -90.f;
			else if (newDirection.x > 0.f)
				rotation = 0.f;
			else if (newDirection.x < 0.f)
				rotation = 180.f;

			GetGameObject()->GetTransform()->SetWorldRotation(rotation);
		}
	}
}

std::string PlayerMovement::GetTypeName() const
{
	return "PlayerMovement";
}

bool PlayerMovement::CheckDirection(const Engine::Vector2& newDirection) const
{
	auto transform{ GetGameObject()->GetTransform() };
	auto collider{ GetGameObject()->GetComponent<Collider>() };

	Engine::Vector2 boxSize{ BOX_SIZE };
	if (newDirection.x == newDirection.y)
	{
		boxSize -= Engine::Vector2{ DIRECTION_ERROR,DIRECTION_ERROR };
	}
	else if (std::abs(newDirection.x) > std::abs(newDirection.y))
	{

		boxSize -= Engine::Vector2{ 0, DIRECTION_ERROR };
	}
	else
	{
		boxSize -= Engine::Vector2{ DIRECTION_ERROR,0 };
	}
	const Vector2 boxPosition = transform->GetWorldLocation() + newDirection * OFFSET_SCALAR + (boxSize * -0.5f);

	return !ServiceLocator::GetPhysicsSystem().BoxCast(boxPosition, boxSize, collider, { Engine::LayerMask::Wall });
}
