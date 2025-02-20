#pragma once
#include "Component.h"
#include <vector>
#include "Vector2.h"

using namespace Engine;

class Transform final : public Component
{
public:
	Transform(GameObject* pOwner);

	void Update() override;

	Vector2 GetWorldLocation() const { return m_WorldPosition; }
	Vector2 GetLocalPosition() const { return m_LocalPosition; }

	void SetWorldLocation(const Vector2& pos);
	void SetWorldLocation(float x, float y);

	void SetLocalPosition(const Vector2& pos);
	void SetLocalPosition(float x, float y);

	float GetWorldRotation() const { return m_WorldRotation; }
	float GetLocalRotation() const { return m_LocalRotation; }

	void SetWorldRotation(float angle);
	void SetLocalRotation(float angle);


	void SetParent(GameObject* pParent, bool keepWorldPosition = false);
private:
	void SetPositionDirty(bool flag = true);
	void CaculateWorldPosition();

	void AddChild(GameObject* pChild);
	void RemoveChild(GameObject* pChild);
	bool IsChild(GameObject* pChild) const;

	bool m_PositionIsDirty;
	Vector2 m_LocalPosition;
	Vector2 m_WorldPosition;

	float m_LocalRotation;
	float m_WorldRotation;

	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;
};
