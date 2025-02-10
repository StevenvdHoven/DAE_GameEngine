#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Component.h"


class Texture2D;
class Transform;
class Scene;

class GameObject final
{
public:
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;

	GameObject();
	~GameObject();

	void AddComponent(std::unique_ptr<Component> pComponent);
	void RemoveComponent(Component* pComponent);

	template<typename ComponentType>
	ComponentType* GetComponent();

	bool IsDestroyed() const { return m_IsDestroyed; }
	Transform* GetTransform() const { return m_pTransform; }

private:
	friend class Scene;

	bool m_IsDestroyed{ false };
	Transform* m_pTransform{nullptr};
	std::vector<std::unique_ptr<Component>> m_Components;
};

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
