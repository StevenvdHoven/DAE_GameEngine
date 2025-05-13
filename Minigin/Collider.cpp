#include "Collider.h"
#include "ServiceLocator.h"
#include "PhysicsSystem.h"

Engine::Collider::Collider(GameObject* pOwner, LayerMask mask):
	Component(pOwner),
	m_IsTrigger{ false },
	m_LayerMask{mask}
{
	// Register the collider with the physics system
	ServiceLocator::GetPhysicsSystem().AddCollider(this);
}

Engine::Collider::~Collider()
{
	// Unregister the collider from the physics system
	ServiceLocator::GetPhysicsSystem().RemoveCollider(this);
}
