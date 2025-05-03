#pragma once
#include <vector>



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

		void ClearColliders();

	private:
		bool m_RenderDebugColliders{ true };

		std::vector<Collider*> m_Colliders;
		std::vector<PhysicsBody*> m_PhysicsBodies;
	};
}



