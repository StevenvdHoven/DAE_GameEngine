#include "PhysicsBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysicsSystem.h"
#include "ServiceLocator.h"

Engine::PhysicsBody::PhysicsBody(GameObject* pOwner):
	Component(pOwner),
	Velocity{ 0.f, 0.f }
{
	ServiceLocator::GetPhysicsSystem().AddPhysicsBody(this);
}
