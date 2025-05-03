#include "PhysicsSystem.h"
#include "Collider.h"
#include "PhysicsBody.h"
#include "Collider.h"

#include "Transform.h"
#include "EngineTime.h"
#include "GameObject.h"

using namespace Engine;

void Engine::PhysicsSystem::Update()
{
}

void Engine::PhysicsSystem::FixedUpdate()
{
	for (auto& physicsBody : m_PhysicsBodies)
	{
		if (!physicsBody->IsEnabled) continue;

		auto bodyCollider{ physicsBody->GetGameObject()->GetComponent<Collider>() };
		auto bodyTransform{ physicsBody->GetGameObject()->GetTransform() };

		for (auto& collider : m_Colliders)
		{
			if (!collider->IsEnabled || collider == bodyCollider) continue;

			bool isOverlapping{ collider->IsOverlapping(bodyCollider) };

			Vector2& bodyVelocity{ physicsBody->Velocity };

			bool updateMovement;
			HandleColldingEvents(bodyCollider, collider, isOverlapping, updateMovement);

			const float fixedDeltaTime{ Time::GetInstance().GetFixedDeltaTime() };
			if (updateMovement)
			{
				const auto worldPosition{ bodyTransform->GetWorldLocation() };
				bodyTransform->SetWorldLocation(worldPosition + bodyVelocity * fixedDeltaTime);
			}
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

void Engine::PhysicsSystem::HandleColldingEvents(Collider* first, Collider* other, bool collided, bool& updateMovement)
{
	if (!first || !other) return;

	updateMovement = first->IsTrigger() || other->IsTrigger();

	auto firstGameObject{ first->GetGameObject() };
	if (collided)
	{
		if (m_ColliderPairs.find(first) == m_ColliderPairs.end())
		{
			m_ColliderPairs[first] = std::vector<Collider*>{ other };

			// TODO: Call OnTriggerEnter or OnCollisionEnter on the colliders
			if (firstGameObject)
			{
				firstGameObject->OnCollisionEnter(other);
			}
			else
			{
				first->GetGameObject()->OnTriggerEnter(other);
			}
		}
		else
		{
			std::vector<Collider*>& colliders = m_ColliderPairs[first];
			if (std::find(colliders.begin(), std::end(colliders), other) != colliders.end())
			{
				// TODO: Call OnTriggerStay or OnCollisionStay on the colliders
				if (firstGameObject)
				{
					firstGameObject->OnCollisionStay(other);
				}
				else
				{
					first->GetGameObject()->OnTriggerStay(other);
				}
			}
			else
			{
				m_ColliderPairs[first].emplace_back(other);

				// TODO: Call OnTriggerEnter or OnCollisionEnter on the colliders
				if (firstGameObject)
				{
					firstGameObject->OnCollisionEnter(other);
				}
				else
				{
					first->GetGameObject()->OnTriggerEnter(other);
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
			if (firstGameObject)
			{
				firstGameObject->OnCollisionExit(other);
			}
			else
			{
				first->GetGameObject()->OnTriggerExit(other);
			}
		}
	}

}



void Engine::PhysicsSystem::ClearColliders()
{
	m_Colliders.clear();
}
