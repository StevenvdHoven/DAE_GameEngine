#pragma once
#include "Component.h"
#include "Vector2.h"
#include <list>
#include "PathFinding.h"


class EnemyMovement : public Engine::Component
{
public:
	EnemyMovement(Engine::GameObject* pOwner);
	virtual ~EnemyMovement() = default;

	void Update() override;

	void SetTargetPosition(const Engine::Vector2& targetPos);

private:
	bool m_Moving;
	Engine::Vector2 m_Direction;
	std::list<Engine::NavigationNode> m_CurrentPath;
	Engine::Graph* m_MapGraph;
};

