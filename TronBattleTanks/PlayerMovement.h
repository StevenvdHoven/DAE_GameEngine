#pragma once
#include "Component.h"
#include "Vector2.h"

namespace Engine
{
	class PhysicsBody;
}

class PlayerMovement : public Engine::Component
{
public:
	PlayerMovement(Engine::GameObject* pOwner, float speed);
	virtual ~PlayerMovement() = default;

	void Start() override;

	void SetDirection(const Engine::Vector2& newDirection);

	std::string GetTypeName() const override;
private:
	bool CheckDirection(const Engine::Vector2& newDirection) const;

	Engine::Vector2 m_CurrentDirection;
	Engine::PhysicsBody* m_Body;
	float m_Speed;
};

