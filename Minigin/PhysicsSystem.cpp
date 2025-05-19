#include "PhysicsSystem.h"
#include "Collider.h"
#include "PhysicsBody.h"
#include "Collider.h"

#include "Transform.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "BoxCollider2D.h"
#include <unordered_set>

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

		auto* bodyTransform = physicsBody->GetGameObject()->GetTransform();
		const Vector2 perviousPosition = bodyTransform->GetWorldLocation();

		Vector2& bodyVelocity = physicsBody->Velocity;
		const auto worldPosition = bodyTransform->GetWorldLocation();
		bodyTransform->SetWorldLocation(worldPosition + bodyVelocity * fixedDeltaTime);

		auto* bodyCollider = physicsBody->GetGameObject()->GetComponent<Collider>();
		if (!bodyCollider || !bodyCollider->IsEnabled) continue;

		if (CheckCollisions(bodyCollider))
		{
			bodyTransform->SetWorldLocation(perviousPosition);
		}
	}
}

bool Engine::PhysicsSystem::CheckCollisions(Collider* collider)
{
	bool collided = false;
	std::for_each(m_Colliders.begin(), m_Colliders.end(), [&](Collider* pOther)
		{
			if (pOther == collider) return;

			if (!pOther->IsEnabled) return;

			if (pOther->IsOverlapping(collider))
			{
				EvaluteOverlappingColliders(collider, pOther, true);
				if (!pOther->IsTrigger() && !collider->IsTrigger())
					collided = true;
			}
			else
			{
				EvaluteOverlappingColliders(collider, pOther, false);
			}
		});

	if (collider->IsTrigger())
		return false;

	return collided;
}

void Engine::PhysicsSystem::EvaluteOverlappingColliders(Collider* first, Collider* other, bool collided)
{
	std::unordered_set<Collider*>& overlappingColliders = first->GetOverlappingColliders();
	std::unordered_set<Collider*>& otherOverlappingColliders = other->GetOverlappingColliders();
	if (collided)
	{
		if (overlappingColliders.insert(other).second)
		{
			FireCollisionEvents(first->GetGameObject(), other->GetGameObject(), first->IsTrigger(), CollisionType::Begin);
			otherOverlappingColliders.insert(first);
		}
		else
		{
			FireCollisionEvents(first->GetGameObject(), other->GetGameObject(), first->IsTrigger(), CollisionType::Stay);
		}
	}
	else
	{
		if (overlappingColliders.erase(other) > 0)
		{
			FireCollisionEvents(first->GetGameObject(), other->GetGameObject(), first->IsTrigger(), CollisionType::End);
		}
		if (otherOverlappingColliders.erase(first) > 0)
		{
			FireCollisionEvents(other->GetGameObject(), first->GetGameObject(), other->IsTrigger(), CollisionType::End);
		}
	}
}

void Engine::PhysicsSystem::FireCollisionEvents(GameObject* firstGameObject, GameObject* other, bool isTrigger, CollisionType eventType)
{
	switch (eventType)
	{
	case Engine::PhysicsSystem::CollisionType::Begin:
		if (isTrigger)
			firstGameObject->OnTriggerEnter(other);
		else
			firstGameObject->OnCollisionEnter(other);
		break;
	case Engine::PhysicsSystem::CollisionType::End:
		if (isTrigger)
			firstGameObject->OnTriggerExit(other);
		else
			firstGameObject->OnCollisionExit(other);
		break;
	case Engine::PhysicsSystem::CollisionType::Stay:
		if (isTrigger)
			firstGameObject->OnTriggerStay(other);
		else
			firstGameObject->OnCollisionStay(other);
		break;
	}
}

bool Engine::PhysicsSystem::AlreadyCollided(Collider* first, Collider* other) const
{
	auto overlappingColliders = first->GetOverlappingColliders();
	auto it = std::find(overlappingColliders.begin(), overlappingColliders.end(), other);

	return it != overlappingColliders.end();
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

		if (pOther->IsOverlappingTest(location, size))
		{
			return true;
		}
	}
	return false;
}

void Engine::PhysicsSystem::ClearColliders()
{
	m_Colliders.clear();
}
