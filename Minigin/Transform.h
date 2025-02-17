#pragma once
#include "Component.h"
#include <vector>
#include <glm.hpp>


class Transform final : public Component
{
public:
	Transform(GameObject* pOwner);

	void Update() override;

	const glm::vec2& GetWorldLocation()
	{
		if (m_PositionIsDirty)
		{
			CaculateWorldPosition();
			return m_WorldPosition;
		}
		return m_pParent->GetTransform()->GetWorldLocation() + m_LocalPosition;  
	}
	const glm::vec2& GetLocalPosition() const { return m_LocalPosition; }
	void SetPosition(float x, float y);

	void SetParent(GameObject* pParent);
private:
	void SetPositionDirty(bool flag = true);
	void CaculateWorldPosition();

	void AddChild(GameObject* pChild);
	void RemoveChild(GameObject* pChild);

	bool m_PositionIsDirty;
	glm::vec2 m_LocalPosition;
	glm::vec2 m_WorldPosition;
	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;
};

