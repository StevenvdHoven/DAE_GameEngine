#include "PhysicsSystem.h"
#include "Collider.h"

using namespace Engine;

void Engine::PhysicsSystem::Update()
{
}

void Engine::PhysicsSystem::FixedUpdate()
{
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
			m_Colliders.erase(it, m_Colliders.end());
		}
	}
}

void Engine::PhysicsSystem::ClearColliders()
{
	m_Colliders.clear();
}
