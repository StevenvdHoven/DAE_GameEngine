#include "BoxCollider2D.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

using namespace Engine;

BoxCollider2D::BoxCollider2D(GameObject* pOwner, const Vector2& size, bool isTrigger):
	Collider(pOwner),
	m_Size(size)
{
	SetIsTrigger(isTrigger);
}

bool Engine::BoxCollider2D::IsOverlapping(const Collider* other) const
{
	return other->IsOverlapping(this);
}

bool Engine::BoxCollider2D::IsOverlapping(const BoxCollider2D* other) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation();
	auto otherPos = other->GetGameObject()->GetTransform()->GetWorldLocation();

	const float halfWidthA = m_Size.x / 2;
	const float halfHeightA = m_Size.y / 2;

	const float halfWidthB = other->GetSize().x / 2;
	const float halfHeightB = other->GetSize().y / 2;

	if (std::abs(thisPos.x - otherPos.x) <= halfWidthA + halfWidthB &&
		std::abs(thisPos.y - otherPos.y) <= halfHeightA + halfHeightB)
	{
		return true;
	}
	return false;
}

bool Engine::BoxCollider2D::IsOverlapping(const CircleCollider* other) const
{
	auto thisPos = GetGameObject()->GetTransform()->GetWorldLocation();
	auto otherPos = other->GetGameObject()->GetTransform()->GetWorldLocation();

	const float halfWidth = m_Size.x / 2;
	const float halfHeight = m_Size.y / 2;
	const float closestX = std::clamp(otherPos.x, thisPos.x - halfWidth, thisPos.x + halfWidth);
	const float closestY = std::clamp(otherPos.y, thisPos.y - halfHeight, thisPos.y + halfHeight);

	const float sqaureDistanceToClosest = (otherPos - Vector2{ closestX, closestY }).SquaredMagnitude();
	const float radiusSquare = other->GetRadius() * other->GetRadius();
	return sqaureDistanceToClosest <= radiusSquare;
}

void Engine::BoxCollider2D::DebugRender()
{
	auto pos = GetGameObject()->GetTransform()->GetWorldLocation();

	Renderer::GetInstance().SetColor({ 255, 0, 0, 255 });
	Renderer::GetInstance().RenderRectangle(pos, m_Size.x, m_Size.y);
}
