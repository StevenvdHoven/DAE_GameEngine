#pragma once
#include "Component.h"
#include "Vector2.h"
#include <unordered_set>

	namespace Engine
	{
		class BoxCollider2D;
		class CircleCollider;

		enum struct LayerMask
		{
			Default = 0,
			Player = 1 << 0,
			Enemy = 1 << 1,
			Projectile = 1 << 2,
			Wall = 1 << 3,
		};

		class Collider : public Component
		{
		public:
			Collider(GameObject* pOwner);
			Collider(GameObject* pOwner, LayerMask mask);
		
			virtual ~Collider();

			virtual bool IsOverlapping(const Collider* other) const { return other != nullptr; };
			virtual bool IsOverlapping(const BoxCollider2D* other) const { return other != nullptr; };
			virtual bool IsOverlapping(const CircleCollider* other) const { return other != nullptr; };

			virtual bool IsOverlappingTest(const Vector2& location, const Vector2& size) const { return location != size; }
			virtual bool IsOverlappingTest(const Vector2& location, const float radius) const { return location != Vector2{radius,radius}; }
			virtual bool RayCast(const Vector2&, const Vector2&, float&) const { return false; }

			virtual void DebugRender() {};

			virtual void Serialize(nlohmann::json& out) const override;
			virtual void Deserialize(const nlohmann::json& in) override;
			virtual std::string GetTypeName() const override;

			void SetIsTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
			bool IsTrigger() const { return m_IsTrigger; }

			Vector2 Center{ 0.f, 0.f };

			void SetLayerMask(LayerMask layerMask) { m_LayerMask = layerMask; }
			const LayerMask& GetLayerMask() const { return m_LayerMask; }

			std::unordered_set<LayerMask>& GetIgnoreLayerMask() { return m_IgnoreMask; }

			std::unordered_set<Collider*>& GetOverlappingColliders() { return m_OverlappingColliders; }

		private:
			Collider() = default;

			bool m_IsTrigger;
			LayerMask m_LayerMask;
			std::unordered_set<LayerMask> m_IgnoreMask;

			std::unordered_set<Collider*> m_OverlappingColliders;

			
		};
	}



