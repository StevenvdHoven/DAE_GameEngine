#include "Transform.h"
#include "GameObject.h"
#include "Matrix3x3.h"
#include "imgui.h"

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
	json["transform_local_pos"] = m_LocalPosition.Serialize();
	json["transform_local_rot"] = m_LocalRotation;
}

void Engine::Transform::Deserialize(const nlohmann::json& json)
{
	auto positonIt = json["transform_local_pos"];
	m_LocalPosition.Deserialize(positonIt);
	m_LocalRotation = json["transform_local_rot"];
	m_PositionIsDirty = true;
	m_RotationIsDirty = true;
}

std::string Engine::Transform::GetTypeName() const
{
	return "TransformComponent";
}

void Engine::Transform::GUI()
{
	ImGui::Text("Transform Component");
	ImGui::Separator();

	Engine::Vector2 localPos = m_LocalPosition;
	if (ImGui::InputFloat2("Local Position", &localPos.x))
	{
		SetLocalPosition(localPos);
	}

	Engine::Vector2 worldPos = GetWorldLocation();
	ImGui::InputFloat2("World Position", &worldPos.x, "%.2f", ImGuiInputTextFlags_ReadOnly);

	float localRot = m_LocalRotation;
	if (ImGui::InputFloat("Local Rotation", &localRot))
	{
		SetLocalRotation(localRot);
	}

	float worldRot = GetWorldRotation();
	ImGui::InputFloat("World Rotation", &worldRot, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Spacing();

	if (ImGui::Button("Reset Transform"))
	{
		SetLocalPosition(0, 0);
		SetLocalRotation(0);
	}
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
