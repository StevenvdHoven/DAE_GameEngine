#pragma once
#include "Component.h"
#include "Vector2.h"
#include <list>
#include "PathFinding.h"

namespace Engine
{
	class PhysicsBody;
}


class EnemyMovement : public Engine::Component
{
	enum struct EEnemyMovementState
	{
		DIRECTION,
		PATHFINDING
	};

public:
	EnemyMovement(Engine::GameObject* pOwner, float speed = 10.f);
	virtual ~EnemyMovement() = default;

	void Start() override;
	void FixedUpdate() override;
	//void Render() const override;

	void SetTargetPosition(const Engine::Vector2& targetPos);
	bool CheckDirection(const Engine::Vector2& newDirection) const;

private:
	void FollowPathMethod();
	void DirectionMethod();
	void CaculateRotation(Engine::Vector2 direction);

	bool m_Moving;
	const float m_Speed;
	EEnemyMovementState m_State;
	Engine::Vector2 m_TargetPosition;
	Engine::Vector2 m_Direction;
	Engine::Vector2 m_TestingDirection;
	Engine::PhysicsBody* m_Body;
	std::list<Engine::NavigationNode> m_CurrentPath;
	Engine::Graph* m_MapGraph;
};

