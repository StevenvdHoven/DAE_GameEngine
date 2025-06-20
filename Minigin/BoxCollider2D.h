#pragma once
#include "Collider.h"
#include "Vector2.h"

namespace Engine
{
	class BoxCollider2D : public Collider
	{
	public:
		BoxCollider2D(GameObject* pOwner);
		BoxCollider2D(GameObject* pOwner,const Vector2& size, bool isTrigger = false, LayerMask mask = LayerMask::Default);
		virtual ~BoxCollider2D() = default;

		virtual bool IsOverlapping(const Collider* other) const override;
		virtual bool IsOverlapping(const BoxCollider2D* other) const override;
		virtual bool IsOverlapping(const CircleCollider* other) const override;

		virtual bool IsOverlappingTest(const Vector2& location, const Vector2& size) const override;
		virtual bool IsOverlappingTest(const Vector2& location, const float radius) const override;

		virtual bool RayCast(const Vector2& origin, const Vector2& direction, float& t) const override;

		virtual void DebugRender() override;

		const Vector2& GetSize() const { return m_Size; }

		virtual void Serialize(nlohmann::json& out) const override;
		virtual void Deserialize(const nlohmann::json& in) override;
		virtual std::string GetTypeName() const override;

		void GUI() override;

	private:
		Vector2 m_Size;
	};
}



