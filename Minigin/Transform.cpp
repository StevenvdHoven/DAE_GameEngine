#include "Transform.h"
#include "GameObject.h"

Transform::Transform(GameObject* pOwner) : Component(pOwner)
{
}

void Transform::Update()
{
	if (m_PositionIsDirty)
	{
		CaculateWorldPosition();
	}
}

void Transform::SetPosition(const float x, const float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
}

void Transform::SetParent(GameObject* pParent)
{
}

void Transform::SetPositionDirty(bool flag)
{
	m_PositionIsDirty = flag;
}

void Transform::CaculateWorldPosition()
{
	if (m_pParent == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
		return;
	}
	else
	{
		Transform* pParentTransform = m_pParent->GetTransform();
		m_WorldPosition = pParentTransform->GetWorldLocation() + m_LocalPosition;
	}

	m_PositionIsDirty = false;
}

void Transform::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(pChild);
}

void Transform::RemoveChild(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
}
