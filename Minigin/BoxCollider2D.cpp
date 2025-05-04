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
	auto thisTopLeft = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	auto otherTopLeft = other->GetGameObject()->GetTransform()->GetWorldLocation() + other->Center;

	Vector2 thisBottomRight = thisTopLeft + m_Size;
	Vector2 otherBottomRight = otherTopLeft + other->GetSize();

	
	bool overlap = !(thisBottomRight.x <= otherTopLeft.x ||  
		thisTopLeft.x >= otherBottomRight.x ||  
		thisBottomRight.y <= otherTopLeft.y ||  
		thisTopLeft.y >= otherBottomRight.y);   

	return overlap;
}

bool Engine::BoxCollider2D::IsOverlapping(const CircleCollider* other) const
{
	Vector2 boxTopLeft = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	Vector2 boxBottomRight = boxTopLeft + m_Size;

	Vector2 circleCenter = other->GetGameObject()->GetTransform()->GetWorldLocation() + other->Center;

	float closestX = std::clamp(circleCenter.x, boxTopLeft.x, boxBottomRight.x);
	float closestY = std::clamp(circleCenter.y, boxTopLeft.y, boxBottomRight.y);

	Vector2 closestPoint{ closestX, closestY };
	float squaredDistance = (circleCenter - closestPoint).SquaredMagnitude();
	float radiusSquared = other->GetRadius() * other->GetRadius();

	return squaredDistance <= radiusSquared;
}

void Engine::BoxCollider2D::DebugRender()
{
	auto pos = GetGameObject()->GetTransform()->GetWorldLocation() + Center;

	Renderer::GetInstance().SetColor({ 255, 0, 0, 255 });
	Renderer::GetInstance().RenderRectangle(pos, m_Size.x, m_Size.y);
}
