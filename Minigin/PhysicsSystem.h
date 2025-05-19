#pragma once
#include <vector>
#include <map>
#include "Vector2.h"



namespace Engine
{
	class Collider;
	class PhysicsBody;
	enum struct LayerMask;
	class GameObject;
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
		enum struct CollisionType
		{
			Begin,
			End,
			Stay
		};


		bool CheckCollisions(Collider* collider);

		void EvaluteOverlappingColliders(Collider* first, Collider* other, bool collided);
		void FireCollisionEvents(GameObject* firstGameObject, GameObject* other, bool isTrigger, CollisionType eventType);
		
		bool AlreadyCollided(Collider* first, Collider* other) const;

		bool m_RenderDebugColliders{ true };

		std::vector<Collider*> m_Colliders;
		std::vector<PhysicsBody*> m_PhysicsBodies;
	};
}



