#pragma once
#include <vector>



namespace Engine
{
	class Collider;
	class PhysicsSystem
	{
	public:
		virtual ~PhysicsSystem() = default;

		void Update();
		void FixedUpdate();

		void Render();

		void AddCollider(Collider* pCollider);
		void RemoveCollider(Collider* pCollider);

		void ClearColliders();

	private:
		bool m_RenderDebugColliders{ true };

		std::vector<Collider*> m_Colliders;
	};
}



