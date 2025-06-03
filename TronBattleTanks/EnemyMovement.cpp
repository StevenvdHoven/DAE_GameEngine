#include "EnemyMovement.h"
#include "ServiceLocator.h"
#include "GameObject.h"

using namespace Engine;

EnemyMovement::EnemyMovement(Engine::GameObject* pOwner) :
	Component{ pOwner },
	m_Moving{ false },
	m_Direction{},
	m_CurrentPath{},
	m_MapGraph{ ServiceLocator::GetPathFinding().GetGraph("map_01.json") }
{
}

void EnemyMovement::Update()
{
	if (m_Moving)
	{
		auto node{ m_CurrentPath.front() };
		auto currentLocation{ GetGameObject()->GetTransform()->GetWorldLocation() };
		const float distance{ Engine::Vector2::Distance(node.Position,currentLocation) };
		if (distance < 2.f)
		{
			m_Direction = m_CurrentPath.front().Direction;
			m_CurrentPath.pop_front();

			if (m_CurrentPath.size() == 0)
			{
				m_Moving = false;
			}
		}
	}
}

void EnemyMovement::SetTargetPosition(const Engine::Vector2& targetPos)
{
	m_Moving = true;
	const auto& myPos{ GetGameObject()->GetTransform()->GetWorldLocation() };
	m_CurrentPath = ServiceLocator::GetPathFinding().FindPath(m_MapGraph, myPos, targetPos);

	m_Direction = (m_CurrentPath.front().Position - GetGameObject()->GetTransform()->GetWorldLocation()).Normalize();

}

