#include "Transform.h"
#include "GameObject.h"

using namespace Engine;

Engine::Transform::Transform(GameObject* pOwner) :
	Component(pOwner),
	m_LocalPosition{},
	m_WorldPosition{},
	m_pParent{ nullptr },
	m_PositionIsDirty{ false }
{

}

Engine::Transform::~Transform()
{
	if (m_pParent) // If a child is deleted notify parent
	{
		m_pParent->GetTransform()->RemoveChild(GetGameObject());
	}

	for (auto& child : m_pChildren) // Apply the same for all children
	{
		child->GetTransform()->SetParent(nullptr);
		Destroy(child);
	}
}

void Engine::Transform::Update()
{

	CaculateWorldPosition();

}

void Engine::Transform::SetWorldLocation(const Engine::Vector2& pos)
{
	SetWorldLocation(pos.x, pos.y);
}

void Engine::Transform::SetWorldLocation(float x, float y)
{
	m_WorldPosition = { x,y };
	m_LocalPosition = m_WorldPosition - m_pParent->GetTransform()->GetWorldLocation();
}

void Engine::Transform::SetLocalPosition(const Engine::Vector2& pos)
{
	SetLocalPosition(pos.x, pos.y);
}

void Engine::Transform::SetLocalPosition(const float x, const float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	SetPositionDirty();
}

void Engine::Transform::SetWorldRotation(float angle)
{
	m_WorldRotation = angle;
	if(m_pParent)
		m_LocalRotation = m_WorldRotation - m_pParent->GetTransform()->GetWorldRotation();
	else
		m_LocalRotation = m_WorldRotation;

}

void Engine::Transform::SetLocalRotation(float angle)
{
	m_LocalRotation = angle;
	if (m_pParent)
		m_WorldRotation = m_LocalRotation + m_pParent->GetTransform()->GetWorldRotation();
	else
		m_WorldRotation = m_LocalRotation;
}

void Engine::Transform::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (IsChild(pParent) || pParent == GetGameObject() || m_pParent == pParent)
		return;
	if (pParent == nullptr)
		SetLocalPosition(GetWorldLocation());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldLocation() - pParent->GetTransform()->GetWorldLocation());
		SetPositionDirty();
	}
	if (m_pParent) m_pParent->GetTransform()->RemoveChild(GetGameObject());
	m_pParent = pParent;
	if (m_pParent) m_pParent->GetTransform()->AddChild(GetGameObject());
}

void Engine::Transform::SetPositionDirty(bool flag)
{
	m_PositionIsDirty = flag;

	for (auto pChild : m_pChildren)
	{
		pChild->GetTransform()->SetPositionDirty(flag);
	}

}

void Engine::Transform::CaculateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pParent->GetTransform()->GetWorldLocation() + m_LocalPosition;
	}
	m_PositionIsDirty = false;

}

void Engine::Transform::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(pChild);
}

void Engine::Transform::RemoveChild(GameObject* pChild)
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (it != m_pChildren.end())
		m_pChildren.erase(it);
}

bool Engine::Transform::IsChild(GameObject* pChild) const
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	return it != m_pChildren.end();
}
