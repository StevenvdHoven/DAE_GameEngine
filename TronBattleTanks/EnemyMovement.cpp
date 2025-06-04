#include "EnemyMovement.h"
#include "ServiceLocator.h"
#include "GameObject.h"
#include "EngineTime.h"
#include "Renderer.h"
#include "PhysicsBody.h"
#include "Collider.h"

#define OFFSET_SCALAR 5.f
#define BOX_SIZE Engine::Vector2{30.f,30.f}
#define DIRECTION_ERROR 4.f

using namespace Engine;

EnemyMovement::EnemyMovement(Engine::GameObject* pOwner, float speed) :
	Component{ pOwner },
	m_Moving{ false },
	m_Speed{ speed },
	m_Direction{},
	m_Body{nullptr},
	m_CurrentPath{},
	m_MapGraph{ ServiceLocator::GetPathFinding().GetGraph("map_01.json") }
{
}

void EnemyMovement::Start()
{
	m_Body = GetGameObject()->GetComponent<PhysicsBody>();
}

void EnemyMovement::FixedUpdate()
{
	if (m_Moving)
	{
		auto node{ m_CurrentPath.front() };
		auto currentLocation{ GetGameObject()->GetTransform()->GetWorldLocation() };

		auto direction{ node.Position - currentLocation };
		if (std::abs(direction.x) > std::abs(direction.y))
		{
			direction.x = direction.x > 0 ? 1.0f : -1.0f;
			direction.y = 0.0f;
		}
		else
		{
			direction.y = direction.y > 0 ? 1.0f : -1.0f;
			direction.x = 0.0f;
		}

		m_TestingDirection = direction;
		if (direction != m_Direction && CheckDirection(direction)) 
		{
			m_Direction = direction;
			CaculateRotation(m_Direction);
		}

		const float distance{ Engine::Vector2::Distance(node.Position,currentLocation) };
		if (distance < 5.f)
		{
			m_CurrentPath.pop_front();

			if (m_CurrentPath.size() == 0)
			{
				m_Moving = false;
				m_Body->Velocity = Engine::Vector2::Zero();
				return;
			}
		}

		m_Body->Velocity = m_Direction * m_Speed;
	}
}

void EnemyMovement::Render() const
{
	Vector2 previousLocation{ GetGameObject()->GetTransform()->GetLocalPosition() };
	for (const auto& node : m_CurrentPath)
	{
		Renderer::GetInstance().SetColor({ 0,255,0,255 });
		Renderer::GetInstance().RenderLine(previousLocation, node.Position);
		previousLocation = node.Position;
	}

	auto transform{ GetGameObject()->GetTransform() };

	
	Engine::Vector2 boxSize{ BOX_SIZE };
	if (std::abs(m_TestingDirection.x) > std::abs(m_TestingDirection.y))
	{

		boxSize -= Engine::Vector2{ DIRECTION_ERROR, 0 };
	}
	else
	{
		boxSize -= Engine::Vector2{ 0,DIRECTION_ERROR };
	}
	const Vector2 boxPosition = transform->GetWorldLocation() + m_TestingDirection * OFFSET_SCALAR + (boxSize * -0.5f);

	Renderer::GetInstance().SetColor({ 255,255,0,255 });
	Renderer::GetInstance().RenderRectangle(boxPosition, boxSize.x, boxSize.y);

	if (m_CurrentPath.size() > 0)
	{
		const auto node{ m_CurrentPath.front() };
		Renderer::GetInstance().SetColor({ 255,0,0,255 });
		Renderer::GetInstance().RenderFilledRectangle(node.Position - Engine::Vector2{ 5,5 }, 10, 10);
	}
}

void EnemyMovement::SetTargetPosition(const Engine::Vector2& targetPos)
{
	const auto& myPos{ GetGameObject()->GetTransform()->GetWorldLocation() };
	auto futurePath = ServiceLocator::GetPathFinding().FindPath(m_MapGraph, myPos, targetPos);
	if (futurePath.size() <= 0) return;

	m_CurrentPath = std::move(futurePath);

	auto direction{ m_CurrentPath.front().Position - myPos };
	if (std::abs(direction.x) > std::abs(direction.y))
	{
		direction.x = direction.x > 0 ? 1.0f : -1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.y = direction.y > 0 ? 1.0f : -1.0f;
		direction.x = 0.0f;
	}

	m_Direction = direction;
	m_Moving = true;
}

bool EnemyMovement::CheckDirection(const Engine::Vector2& newDirection) const
{
	auto transform{ GetGameObject()->GetTransform() };
	auto collider{ GetGameObject()->GetComponent<Collider>() };

	Engine::Vector2 boxSize{ BOX_SIZE };
	if (std::abs(m_TestingDirection.x) > std::abs(m_TestingDirection.y))
	{
		
		boxSize -= Engine::Vector2{ DIRECTION_ERROR, 0 };
	}
	else
	{
		boxSize -= Engine::Vector2{ 0,DIRECTION_ERROR };
	}
	const Vector2 boxPosition = transform->GetWorldLocation() + newDirection * OFFSET_SCALAR + (boxSize * -0.5f);
	
	return !ServiceLocator::GetPhysicsSystem().BoxCast(boxPosition, boxSize, collider, { Engine::LayerMask::Wall });
}

void EnemyMovement::CaculateRotation(Engine::Vector2 direction)
{
	constexpr float radToDreg{ (180.f / 3.14f) };
	float rot{ std::atan2f(direction.y,direction.x) * radToDreg };
	GetGameObject()->GetTransform()->SetLocalRotation(rot);
}

