#include "BoxCollider2D.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

using namespace Engine;

BoxCollider2D::BoxCollider2D(GameObject* pOwner, const Vector2& size, bool isTrigger, LayerMask mask):
	Collider(pOwner,mask),
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

bool Engine::BoxCollider2D::IsOverlappingTest(const Vector2& location, const Vector2& size) const
{
	auto thisTopLeft = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	auto otherTopLeft = location;

	Vector2 thisBottomRight = thisTopLeft + m_Size;
	Vector2 otherBottomRight = otherTopLeft + size;


	bool overlap = !(thisBottomRight.x <= otherTopLeft.x ||
		thisTopLeft.x >= otherBottomRight.x ||
		thisBottomRight.y <= otherTopLeft.y ||
		thisTopLeft.y >= otherBottomRight.y);

	return overlap;
}

bool Engine::BoxCollider2D::IsOverlappingTest(const Vector2& location, const float radius) const
{
	Vector2 boxTopLeft = GetGameObject()->GetTransform()->GetWorldLocation() + Center;
	Vector2 boxBottomRight = boxTopLeft + m_Size;

	Vector2 circleCenter = location;

	float closestX = std::clamp(circleCenter.x, boxTopLeft.x, boxBottomRight.x);
	float closestY = std::clamp(circleCenter.y, boxTopLeft.y, boxBottomRight.y);

	Vector2 closestPoint{ closestX, closestY };
	float squaredDistance = (circleCenter - closestPoint).SquaredMagnitude();
	float radiusSquared = radius * radius;

	return squaredDistance <= radiusSquared;
}

bool Engine::BoxCollider2D::RayCast(const Vector2& origin, const Vector2& direction, float& t) const
{
	if (direction.x == 0.f && direction.y == 0.f)
		return false;
	
	const Vector2 location{ GetGameObject()->GetTransform()->GetWorldLocation() + Center};
	const Vector2 halfSize{ m_Size * 0.5f };
	const Vector2 min{ location - halfSize};
	const Vector2 max{ location + halfSize };

	const Vector2 invDir{
		direction.x != 0.f ? 1.f / direction.x : std::numeric_limits<float>::infinity(),
		direction.y != 0.f ? 1.f / direction.y : std::numeric_limits<float>::infinity()
	} ;

	const float tx1{ (min.x - origin.x) * invDir.x };
	const float tx2{(max.x - origin.x) * invDir.x};

	const float ty1{ (min.y - origin.y) * invDir.y };
	const float ty2{ (max.y - origin.y) * invDir.y };

	const float tmin = std::max(std::min(tx1, tx2), std::min(ty1, ty2));
	const float tmax = std::min(std::max(tx1, tx2), std::max(ty1, ty2));

	const float caculatedT{ std::max(tmin, 0.0f) };
	if (tmax >= caculatedT)
	{
		t = caculatedT;
		return true;
	}

	return false;
}

void Engine::BoxCollider2D::DebugRender()
{
	auto pos = GetGameObject()->GetTransform()->GetWorldLocation() + Center;

	Renderer::GetInstance().SetColor({ 255, 0, 0, 255 });
	Renderer::GetInstance().RenderRectangle(pos, m_Size.x, m_Size.y);
}
