#pragma once
#include <vector>
#include <map>



namespace Engine
{
	class Collider;
	class PhysicsBody;
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

		void HandleCollidingEvents(Collider* first, Collider* other, bool collided, bool& updateMovement);

		void ClearColliders();

	private:
		bool m_RenderDebugColliders{ true };

		std::vector<Collider*> m_Colliders;
		std::vector<PhysicsBody*> m_PhysicsBodies;

		std::map<Collider*, std::vector<Collider*>> m_ColliderPairs;
	};
}



