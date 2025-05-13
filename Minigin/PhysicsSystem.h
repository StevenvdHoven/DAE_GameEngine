#pragma once
#include <vector>
#include <map>
#include "Vector2.h"



namespace Engine
{
	class Collider;
	class PhysicsBody;
	enum struct LayerMask;
	class PhysicsSystem
	{
	public:
		virtual ~PhysicsSystem() = default;

		void Update();
		void FixedUpdate();

		void Render();

		void AddPhysicsBody(PhysicsBody* pPhysicsBody);
		void RemovePhysicsBody(PhysicsBody* pPhysicsBody);

		void AddCollider(Collider* pCollider);
		void RemoveCollider(Collider* pCollider);

		bool BoxCast(const Engine::Vector2& location, const Engine::Vector2& size, const Collider* ignore, std::vector<LayerMask> layermask = {}) const;
		

		
		void ClearColliders();

	private:
		void HandleCollidingEvents(Collider* first, Collider* other, bool collided, bool& updateMovement);


		bool m_RenderDebugColliders{ true };

		std::vector<Collider*> m_Colliders;
		std::vector<PhysicsBody*> m_PhysicsBodies;

		std::map<Collider*, std::vector<Collider*>> m_ColliderPairs;
	};
}



