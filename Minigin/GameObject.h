#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Component.h"
#include "Transform.h"
#include "json.hpp"

namespace Engine
{
	class Texture2D;
	class Scene;
	class Collider;

	class GameObject final
	{
	public:
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void OnCollisionEnter(GameObject* other);
		void OnCollisionStay(GameObject* other);
		void OnCollisionExit(GameObject* other);

		void OnTriggerEnter(GameObject* other);
		void OnTriggerStay(GameObject* other);
		void OnTriggerExit(GameObject* other);

		void Serialize(nlohmann::json& json);
		void Deserialize(nlohmann::json& json, std::vector<std::unique_ptr<GameObject>>& spawnChilderen);

		void SetActive(bool active);


		std::string Name() const { return m_Name; }
		bool PreviewGUI(GameObject*& selectedObject);
		void GUI();

		GameObject(const std::string& name = "GameObject");
		~GameObject();

		template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args);

		void RemoveComponent(Component* pComponent);
		bool HasComponent(const std::string& typeName) const
		{
			return std::any_of(m_Components.begin(), m_Components.end(),
				[&typeName](const std::unique_ptr<Component>& component) {
					return component->GetTypeName() == typeName;
				});
		}

		template<typename ComponentType>
		ComponentType* GetComponent();

		bool IsDestroyed() const { return m_IsDestroyed; }
		Transform* GetTransform() const { return m_pTransform; }

	private:
		friend class Scene;

		std::string m_Name;
		bool m_IsActive;
		bool m_IsDestroyed;
		std::vector<std::unique_ptr<Component>> m_Components;
		Transform* m_pTransform;
	};

	template<typename ComponentType, typename ...Args>
	inline ComponentType* GameObject::AddComponent(Args&&... args)
	{
		auto pComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);
		auto rawPtr = pComponent.get();

		m_Components.emplace_back(std::move(pComponent));
		return rawPtr;
	}

	template<typename ComponentType>
	inline ComponentType* GameObject::GetComponent()
	{
		for (auto& pComponent : m_Components)
		{
			ComponentType* type = dynamic_cast<ComponentType*>(pComponent.get());
			if (type != nullptr)
			{
				return type;
			}
		}

		return nullptr;
	}
}
