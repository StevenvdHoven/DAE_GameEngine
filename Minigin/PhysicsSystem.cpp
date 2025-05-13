#include "PhysicsSystem.h"
#include "Collider.h"
#include "PhysicsBody.h"
#include "Collider.h"

#include "Transform.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "BoxCollider2D.h"

using namespace Engine;

void Engine::PhysicsSystem::Update()
{
}

void Engine::PhysicsSystem::FixedUpdate()
{
	const float fixedDeltaTime = Time::GetInstance().GetFixedDeltaTime();
	for (auto& physicsBody : m_PhysicsBodies)
	{
		if (!physicsBody->IsEnabled) continue;

		auto* gameObject = physicsBody->GetGameObject();
		if (gameObject == nullptr || gameObject->IsDestroyed()) continue;
		
		auto* bodyCollider = gameObject->GetComponent<Collider>();
		if (!bodyCollider || !bodyCollider->IsEnabled) continue;

		auto* bodyTransform = gameObject->GetTransform();
		const Vector2 perviousPosition = bodyTransform->GetWorldLocation();

		Vector2& bodyVelocity = physicsBody->Velocity;
		const auto worldPosition = bodyTransform->GetWorldLocation();
		bodyTransform->SetWorldLocation(worldPosition + bodyVelocity * fixedDeltaTime);

		bool updateMovement = true;

		for (auto* collider : m_Colliders)
		{
			if (!collider->IsEnabled || collider == bodyCollider) continue;

			bool isOverlapping = bodyCollider->IsOverlapping(collider);

			bool resultMovement;
			HandleCollidingEvents(bodyCollider, collider, isOverlapping, resultMovement);

			if (!resultMovement)
			{
				updateMovement = false;
			}
		}

		if (!updateMovement)
		{
			bodyTransform->SetWorldLocation(perviousPosition);
		}
	}
}


void Engine::PhysicsSystem::Render()
{
	if (m_RenderDebugColliders)
	{
		for (const auto& collider : m_Colliders)
		{
			collider->DebugRender();
		}
	}
}

void Engine::PhysicsSystem::AddPhysicsBody(PhysicsBody* pPhysicsBody)
{
	if (pPhysicsBody)
	{
		m_PhysicsBodies.emplace_back(pPhysicsBody);
	}

}

void Engine::PhysicsSystem::RemovePhysicsBody(PhysicsBody* pPhysicsBody)
{
	if (pPhysicsBody)
	{
		auto it = std::remove(m_PhysicsBodies.begin(), m_PhysicsBodies.end(), pPhysicsBody);
		if (it != m_PhysicsBodies.end())
		{
			m_PhysicsBodies.erase(it);
		}
	}
}

void Engine::PhysicsSystem::AddCollider(Collider* pCollider)
{
	if (pCollider)
	{
		m_Colliders.emplace_back(pCollider);
	}

}

void Engine::PhysicsSystem::RemoveCollider(Collider* pCollider)
{
	if (pCollider)
	{
		auto it = std::remove(m_Colliders.begin(), m_Colliders.end(), pCollider);
		if (it != m_Colliders.end())
		{
			m_Colliders.erase(it);
		}
	}
}

bool Engine::PhysicsSystem::BoxCast(const Engine::Vector2& location, const Engine::Vector2& size, const Collider* ignore, std::vector<LayerMask> layermask) const
{

	for (const auto& pOther : m_Colliders)
	{
		// Check if the collider is enabled and not the one we are ignoring
		if (!pOther->IsEnabled) continue;

		// Check if the collider is in the specified layer mask
		if (pOther == ignore) continue;


		if (!layermask.empty())
		{
			auto it{ std::find(layermask.begin(), layermask.end(), pOther->GetLayerMask()) };
			if (it != layermask.end()) continue;
		}

		if (pOther->IsOverlappingTest(location,size))
		{
			return true;
		}
	}
	return false;
}

void Engine::PhysicsSystem::HandleCollidingEvents(Collider* first, Collider* other, bool collided, bool& updateMovement)
{
	if (!first || !other || (first == other)) return;

	updateMovement = !collided || (first->IsTrigger() || other->IsTrigger());

	auto firstGameObject{ first->GetGameObject() };
	if (collided)
	{
		if (m_ColliderPairs.find(first) == m_ColliderPairs.end())
		{
			m_ColliderPairs[first] = std::vector<Collider*>{ other };

			// TODO: Call OnTriggerEnter or OnCollisionEnter on the colliders
			if (!first->IsTrigger())
			{
				firstGameObject->OnCollisionEnter(other->GetGameObject());
				other->GetGameObject()->OnCollisionEnter(firstGameObject);
			}
			else
			{
				firstGameObject->OnTriggerEnter(other->GetGameObject());
				other->GetGameObject()->OnTriggerEnter(firstGameObject);
			}
		}
		else
		{
			std::vector<Collider*>& colliders = m_ColliderPairs[first];
			if (std::find(colliders.begin(), std::end(colliders), other) != colliders.end())
			{
				// TODO: Call OnTriggerStay or OnCollisionStay on the colliders
				if (!first->IsTrigger())
				{
					firstGameObject->OnCollisionStay(other->GetGameObject());
					other->GetGameObject()->OnCollisionStay(firstGameObject);
				}
				else
				{
					firstGameObject->OnTriggerStay(other->GetGameObject());
					other->GetGameObject()->OnTriggerStay(firstGameObject);
				}
			}
			else
			{
				m_ColliderPairs[first].emplace_back(other);

				// TODO: Call OnTriggerEnter or OnCollisionEnter on the colliders
				if (!first->IsTrigger())
				{
					firstGameObject->OnCollisionEnter(other->GetGameObject());
					other->GetGameObject()->OnCollisionEnter(firstGameObject);
				}
				else
				{
					firstGameObject->OnTriggerEnter(other->GetGameObject());
					other->GetGameObject()->OnTriggerEnter(firstGameObject);
				}
			}
		}
	}
	else
	{
		if (m_ColliderPairs.find(first) == m_ColliderPairs.end())
		{
			return;
		}

		std::vector<Collider*>& colliders = m_ColliderPairs[first];
		auto it = std::find(colliders.begin(), colliders.end(), other);
		if (it != colliders.end())
		{
			colliders.erase(it);
			if (colliders.empty())
			{
				m_ColliderPairs.erase(first);
			}

			// TODO: Call OnTriggerExit or OnCollisionExit on the colliders
			if (!first->IsTrigger())
			{
				firstGameObject->OnCollisionExit(other->GetGameObject());
				other->GetGameObject()->OnCollisionExit(firstGameObject);
			}
			else
			{
				firstGameObject->OnTriggerExit(other->GetGameObject());
				other->GetGameObject()->OnTriggerExit(firstGameObject);
			}
		}
	}

}



void Engine::PhysicsSystem::ClearColliders()
{
	m_Colliders.clear();
}
