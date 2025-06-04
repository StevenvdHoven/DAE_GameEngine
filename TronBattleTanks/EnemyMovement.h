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
public:
	EnemyMovement(Engine::GameObject* pOwner, float speed = 10.f);
	virtual ~EnemyMovement() = default;

	void Start() override;
	void FixedUpdate() override;
	void Render() const override;

	void SetTargetPosition(const Engine::Vector2& targetPos);
	bool CheckDirection(const Engine::Vector2& newDirection) const;

private:
	void CaculateRotation(Engine::Vector2 direction);

	bool m_Moving;
	const float m_Speed;
	Engine::Vector2 m_Direction;
	Engine::Vector2 m_TestingDirection;
	Engine::PhysicsBody* m_Body;
	std::list<Engine::NavigationNode> m_CurrentPath;
	Engine::Graph* m_MapGraph;
};

