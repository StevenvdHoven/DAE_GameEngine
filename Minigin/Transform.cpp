#include "Transform.h"
#include "GameObject.h"

Transform::Transform(GameObject* pOwner) : 
	Component(pOwner),
	m_LocalPosition{},
	m_WorldPosition{},
	m_pParent{ nullptr },
	m_PositionIsDirty{ false }
{
	
}

void Transform::Update()
{
	if (m_PositionIsDirty)
	{
		CaculateWorldPosition();
	}
}

void Transform::SetLocalPosition(const glm::vec2& pos)
{
	SetLocalPosition(pos.x, pos.y);
}

void Transform::SetLocalPosition(const float x, const float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	SetPositionDirty();
}

void Transform::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (IsChild(pParent) || pParent == GetGameObject() || pParent == pParent)
		return;
	if (pParent == nullptr)
		SetLocalPosition(GetWorldLocation());
	else
	{
		if(keepWorldPosition)
			SetLocalPosition(GetWorldLocation() - pParent->GetTransform()->GetWorldLocation());
		SetPositionDirty();
	}
	if (m_pParent) m_pParent->GetTransform()->RemoveChild(GetGameObject());
	m_pParent = pParent;
	if (m_pParent) m_pParent->GetTransform()->AddChild(GetGameObject());
}

void Transform::SetPositionDirty(bool flag)
{
	m_PositionIsDirty = flag;
}

void Transform::CaculateWorldPosition()
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

void Transform::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(pChild);
}

void Transform::RemoveChild(GameObject* pChild)
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (it != m_pChildren.end())
		m_pChildren.erase(it);
}

bool Transform::IsChild(GameObject* pChild) const
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	return it != m_pChildren.end();
}
