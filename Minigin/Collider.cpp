#include "Collider.h"
#include "ServiceLocator.h"
#include "PhysicsSystem.h"

Engine::Collider::Collider(GameObject* pOwner):
	Component(pOwner)
{
	// Register the collider with the physics system
	ServiceLocator::GetPhysicsSystem().AddCollider(this);
}

Engine::Collider::~Collider()
{
	// Unregister the collider from the physics system
	ServiceLocator::GetPhysicsSystem().RemoveCollider(this);
}
