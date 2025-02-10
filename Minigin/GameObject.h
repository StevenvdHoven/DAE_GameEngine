#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Component.h"


class Texture2D;
class Transform;

class GameObject final
{
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render() const;

	GameObject();
	virtual ~GameObject();
	/*GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;*/

	void AddComponent(std::unique_ptr<Component> pComponent);

	template<typename ComponentType>
	ComponentType* GetComponent();

	Transform* GetTransform() const { return m_pTransform; };

private:

	Transform* m_pTransform{};
	// todo: mmm, every gameobject has a texture? Is that correct?
	std::shared_ptr<Texture2D> m_texture{};
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
