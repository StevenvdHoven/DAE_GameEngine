#pragma once
#include "Collider.h"


namespace Engine
{
	class CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* pOwner,float radius,bool isTrigger);
		virtual ~CircleCollider() override = default;

		virtual bool IsOverlapping(const Collider* other) const override;
		virtual bool IsOverlapping(const BoxCollider2D* other) const override;
		virtual bool IsOverlapping(const CircleCollider* other) const override;

		virtual void DebugRender() override;

		float GetRadius() const { return m_Radius; }

	private:
		float m_Radius;
	};
}



