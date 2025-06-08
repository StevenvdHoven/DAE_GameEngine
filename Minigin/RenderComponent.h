#pragma once
#include "Component.h"

namespace Engine
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner);
		virtual ~RenderComponent();

		virtual void Render() const {}

		virtual void GUI();

		virtual void Serialize(nlohmann::json&) const;
		virtual void Deserialize(const nlohmann::json&);
		std::string GetTypeName() const override;

		int Order() const { return m_Order; }
	private:
		int m_Order;

		
	};
}



