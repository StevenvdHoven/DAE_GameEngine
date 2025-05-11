#pragma once
#include "Component.h"
#include "Vector2.h"

namespace Engine
{
	class Transform;

	class PhysicsBody final : public Component
	{
	public:
		PhysicsBody(GameObject* pOwner);
		virtual ~PhysicsBody();


		Vector2 Velocity{ 0.f, 0.f };
	};
}


