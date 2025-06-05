#pragma once
#include "Collider.h"


namespace Engine
{
	class CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* pOwner, float radius, bool isTrigger = false, LayerMask mask = LayerMask::Default);
		virtual ~CircleCollider() override = default;

		virtual bool IsOverlapping(const Collider* other) const override;
		virtual bool IsOverlapping(const BoxCollider2D* other) const override;
		virtual bool IsOverlapping(const CircleCollider* other) const override;

		virtual bool IsOverlappingTest(const Vector2& location, const Vector2& size) const override;
		virtual bool IsOverlappingTest(const Vector2& location, const float radius) const override;

		virtual bool RayCast(const Vector2& origin, const Vector2& direction, float& t) const override;

		virtual void DebugRender() override;

		float GetRadius() const { return m_Radius; }

	private:
		float m_Radius;
	};
}



