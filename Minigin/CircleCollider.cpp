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
	Vector2 circleCenter = GetGameObject()->GetTransform()->GetWorldLocation() + Center;

	// Box bounds in world space
	Vector2 boxTopLeft = other->GetGameObject()->GetTransform()->GetWorldLocation() + other->Center;
	Vector2 boxBottomRight = boxTopLeft + other->GetSize();

	float closestX = std::clamp(circleCenter.x, boxTopLeft.x, boxBottomRight.x);
	float closestY = std::clamp(circleCenter.y, boxTopLeft.y, boxBottomRight.y);
	Vector2 closestPoint{ closestX, closestY };

	float squaredDistance = (circleCenter - closestPoint).SquaredMagnitude();
	float radiusSquared = m_Radius * m_Radius;

	return squaredDistance <= radiusSquared;
}

bool Engine::CircleCollider::IsOverlapping(const CircleCollider* other) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	auto otherPos = other->GetGameObject()->GetTransform()->GetWorldLocation() + other->Center;

	const float sqaureDistance = (thisPos - otherPos).SquaredMagnitude();
	const float radiusSum = m_Radius + other->GetRadius();

	return sqaureDistance <= radiusSum * radiusSum;
}

bool Engine::CircleCollider::IsOverlappingTest(const Vector2& location, const Vector2& size) const
{
	Vector2 circleCenter = GetGameObject()->GetTransform()->GetWorldLocation() + Center;

	// Box bounds in world space
	Vector2 boxTopLeft = location;
	Vector2 boxBottomRight = boxTopLeft + size;

	float closestX = std::clamp(circleCenter.x, boxTopLeft.x, boxBottomRight.x);
	float closestY = std::clamp(circleCenter.y, boxTopLeft.y, boxBottomRight.y);
	Vector2 closestPoint{ closestX, closestY };

	float squaredDistance = (circleCenter - closestPoint).SquaredMagnitude();
	float radiusSquared = m_Radius * m_Radius;

	return squaredDistance <= radiusSquared;
}

bool Engine::CircleCollider::IsOverlappingTest(const Vector2& location, const float radius) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	auto otherPos = location;

	const float sqaureDistance = (thisPos - otherPos).SquaredMagnitude();
	const float radiusSum = m_Radius + radius;

	return sqaureDistance <= radiusSum * radiusSum;
}

void Engine::CircleCollider::DebugRender()
{

	const float radius = m_Radius;
	auto pos = GetGameObject()->GetTransform()->GetWorldLocation() + Center - Vector2{ radius / 2.f,radius / 2.f };

	Renderer::GetInstance().SetColor({ 255, 0, 0, 255 });
	Renderer::GetInstance().RenderRectangle(pos, radius, radius);
}
