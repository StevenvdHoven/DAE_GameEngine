#include "CircleCollider.h"
#include "BoxCollider2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Renderer.h"

using namespace Engine;

Engine::CircleCollider::CircleCollider(GameObject* pOwner, float radius, bool isTrigger):
	Collider(pOwner),
	m_Radius(radius)
{
	SetIsTrigger(isTrigger);
}

bool Engine::CircleCollider::IsOverlapping(const Collider* other) const
{
	return other->IsOverlapping(this);
}

bool Engine::CircleCollider::IsOverlapping(const BoxCollider2D* other) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation();
	auto otherPos = other->GetGameObject()->GetTransform()->GetWorldLocation();

	const float halfWidth = other->GetSize().x / 2;
	const float halfHeight = other->GetSize().y / 2;
	const float closestX = std::clamp(thisPos.x, otherPos.x - halfWidth, otherPos.x + halfWidth);
	const float closestY = std::clamp(thisPos.y, otherPos.y - halfHeight, otherPos.y + halfHeight);

	const float sqaureDistanceToClosest = (thisPos - Vector2{ closestX, closestY }).SquaredMagnitude();
	const float radiusSquare = m_Radius * m_Radius;
	return sqaureDistanceToClosest <= radiusSquare;
}

bool Engine::CircleCollider::IsOverlapping(const CircleCollider* other) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation();
	auto otherPos = other->GetGameObject()->GetTransform()->GetWorldLocation();

	const float sqaureDistance = (thisPos - otherPos).SquaredMagnitude();
	const float radiusSum = m_Radius + other->GetRadius();

	return sqaureDistance <= radiusSum * radiusSum;
}

void Engine::CircleCollider::DebugRender()
{
	auto pos = GetGameObject()->GetTransform()->GetWorldLocation();
	const float radius = m_Radius;
	

	Renderer::GetInstance().SetColor({ 255, 0, 0, 255 });
	Renderer::GetInstance().RenderOval(pos, radius, radius);
}
