#pragma once
#include "Component.h"
#include <vector>
#include "Vector2.h"

namespace Engine
{
	class Transform final : public Component
	{
	public:
		Transform(GameObject* pOwner);
		virtual ~Transform();

		void Update() override;

		Engine::Vector2 GetWorldLocation() const 
		{ 
			if (m_PositionIsDirty)
			{
				CaculateWorldPosition();
				m_PositionIsDirty = false;
			}
			return m_WorldPosition; 
		}
		Engine::Vector2 GetLocalPosition() const { return m_LocalPosition; }

		void SetWorldLocation(const Engine::Vector2& pos);
		void SetWorldLocation(float x, float y);

		void SetLocalPosition(const Engine::Vector2& pos);
		void SetLocalPosition(float x, float y);

		float GetWorldRotation() const { return m_WorldRotation; }
		float GetLocalRotation() const { return m_LocalRotation; }

		void SetWorldRotation(float angle);
		void SetLocalRotation(float angle);

		Engine::Vector2 GetForward() const;
		Engine::Vector2 GetUp() const;

		GameObject* FindObjectByNameInChilderen(const std::string& name);

		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		GameObject* GetParent() const { return m_pParent; }
		const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }

		void Serialize(nlohmann::json& out) const override;
		void Deserialize(const nlohmann::json& in) override;
		std::string GetTypeName() const override;

		void GUI() override;
	private:

		void SetPositionDirty(bool flag = true);
		void SetRotationDirty(bool flag = true);

		void CaculateWorldPosition() const;
		void CaculateWorldRotation();

		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
		bool IsChild(GameObject* pChild) const;

		mutable bool m_PositionIsDirty;
		mutable bool m_RotationIsDirty;

		Engine::Vector2 m_LocalPosition;
		mutable Engine::Vector2 m_WorldPosition;

		float m_LocalRotation;
		float m_WorldRotation;

		GameObject* m_pParent;
		std::vector<GameObject*> m_pChildren;
	};
}
