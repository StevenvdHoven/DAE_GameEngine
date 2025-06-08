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

		void Serialize(nlohmann::json& json) const override;
		void Deserialize(const nlohmann::json& json) override;
		std::string GetTypeName() const override;

		void GUI() override;

		Vector2 Velocity{ 0.f, 0.f };
		
	
	};
}


