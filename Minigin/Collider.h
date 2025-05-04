#pragma once
#include "Component.h"
#include "Vector2.h"

namespace Engine
{
	class BoxCollider2D;
	class CircleCollider;

	class Collider : public Component
	{
	public:
		Collider(GameObject* pOwner);
		
		virtual ~Collider();

		virtual bool IsOverlapping(const Collider* other) const { return other != nullptr; };
		virtual bool IsOverlapping(const BoxCollider2D* other) const { return other != nullptr; };
		virtual bool IsOverlapping(const CircleCollider* other) const { return other != nullptr; };

		virtual void DebugRender() {};

		void SetIsTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
		bool IsTrigger() const { return m_IsTrigger; }

		Vector2 Center{ 0.f, 0.f };

	private:
		bool m_IsTrigger{ false };
	};
}



