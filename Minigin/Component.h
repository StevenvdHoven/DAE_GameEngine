#pragma once
#include <xmemory>

class GameObject;
class Transform;
class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    // Delete copy constructor and copy assignment (to prevent copying components)
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;

    // Allow move semantics
    Component(Component&& other) noexcept = default;
    Component& operator=(Component&& other) noexcept = default;

    virtual void Start() {}
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}
    virtual void Render() const {}

    void Destroy(GameObject* pGameObject);
    void Destroy(Component* pComponent);


    bool IsDestroyed() const { return m_Destroyed; }
    GameObject* GetGameObject() const { return m_pGameObject; }
    Transform* GetTransform() const { return m_pTransform; }

private:
    friend class GameObject;

    GameObject* m_pGameObject = nullptr;
    Transform* m_pTransform = nullptr;
    bool m_Destroyed{ false };
};


