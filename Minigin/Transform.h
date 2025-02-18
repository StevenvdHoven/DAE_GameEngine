#pragma once
#include "Component.h"
#include <vector>
#include <glm.hpp>


class Transform final : public Component
{
public:
	Transform(GameObject* pOwner);

	void Update() override;

	glm::vec2 GetWorldLocation() const { return m_WorldPosition; }
	glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
	void SetLocalPosition(const glm::vec2& pos);
	void SetLocalPosition(float x, float y);

	void SetParent(GameObject* pParent, bool keepWorldPosition = false);
private:
	void SetPositionDirty(bool flag = true);
	void CaculateWorldPosition();

	void AddChild(GameObject* pChild);
	void RemoveChild(GameObject* pChild);
	bool IsChild(GameObject* pChild) const;

	bool m_PositionIsDirty;
	glm::vec2 m_LocalPosition;
	glm::vec2 m_WorldPosition;
	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;
};

