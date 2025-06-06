#include "Transform.h"
#include "GameObject.h"
#include "Matrix3x3.h"

using namespace Engine;

Engine::Transform::Transform(GameObject* pOwner) :
	Component(pOwner),
	m_LocalPosition{},
	m_WorldPosition{},
	m_pParent{ nullptr },
	m_PositionIsDirty{ false },
	m_RotationIsDirty{ false }
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
	CaculateWorldRotation();

}

void Engine::Transform::SetWorldLocation(const Engine::Vector2& pos)
{
	SetWorldLocation(pos.x, pos.y);
}

void Engine::Transform::SetWorldLocation(float x, float y)
{
	m_WorldPosition = { x,y };

	if (m_pParent)
		m_LocalPosition = m_WorldPosition - m_pParent->GetTransform()->GetWorldLocation();
	else
		m_LocalPosition = m_WorldPosition;

	for (auto pChild : m_pChildren)
	{
		pChild->GetTransform()->SetPositionDirty();
	}
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
	if (m_pParent)
		m_LocalRotation = m_WorldRotation - m_pParent->GetTransform()->GetWorldRotation();
	else
		m_LocalRotation = m_WorldRotation;

	for (auto pChild : m_pChildren)
	{
		pChild->GetTransform()->SetRotationDirty();
	}

}

void Engine::Transform::SetLocalRotation(float angle)
{
	m_LocalRotation = angle;
	SetRotationDirty();
}

Engine::Vector2 Engine::Transform::GetForward() const
{
	Matrix3x3 rotationMatrix = Matrix3x3::CreateRotationMatrix(m_WorldRotation);
	return rotationMatrix * Engine::Vector2(1, 0);
}

Engine::Vector2 Engine::Transform::GetUp() const
{
	Matrix3x3 rotationMatrix = Matrix3x3::CreateRotationMatrix(m_WorldRotation);
	return rotationMatrix * Engine::Vector2(0, 1);
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

void Engine::Transform::Serialize(nlohmann::json& json) const
{
	nlohmann::json transformJson;
	transformJson["transform_local_pos"] = m_LocalPosition.Serialize();
	transformJson["transform_local_rot"] = m_LocalRotation;
	json["component_transform"] = transformJson;
}

void Engine::Transform::Deserialize(const nlohmann::json& json)
{
	nlohmann::json transformJson = json["component_transform"];
	m_LocalPosition.Deserialize(transformJson["tranform_local_pos"]);
	m_LocalRotation = transformJson["transform_local_rot"];
}

std::string Engine::Transform::GetTypeName() const
{
	return "TransformComponent";
}

void Engine::Transform::SetPositionDirty(bool flag)
{
	m_PositionIsDirty = flag;

	for (auto pChild : m_pChildren)
	{
		pChild->GetTransform()->SetPositionDirty(flag);
	}

}

void Engine::Transform::SetRotationDirty(bool flag)
{
	m_RotationIsDirty = flag;

	for (auto pChild : m_pChildren)
	{
		pChild->GetTransform()->SetRotationDirty(flag);
	}
}

void Engine::Transform::CaculateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
		{
			Matrix3x3 rotationMatrix = Matrix3x3::CreateRotationMatrix(m_pParent->GetTransform()->GetWorldRotation());
			Vector2 rotatedPosition = rotationMatrix * m_LocalPosition;
			m_WorldPosition = m_pParent->GetTransform()->GetWorldLocation() + rotatedPosition;
		}
			
	}
	m_PositionIsDirty = false;

}

void Engine::Transform::CaculateWorldRotation()
{
	if (m_RotationIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldRotation = m_LocalRotation;
		else
			m_WorldRotation = m_pParent->GetTransform()->GetWorldRotation() + m_LocalRotation;
	}
	m_RotationIsDirty = false;
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
