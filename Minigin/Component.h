#pragma once
#include <xmemory>

namespace Engine
{

    class GameObject;
    class Transform;
    class Component
    {
    public:
        virtual ~Component() = default;

        // Delete copy constructor and copy assignment (to prevent copying components)
        Component(const Component& other) = delete;
        Component& operator=(const Component& other) = delete;

        // Allow move semantics
        Component(Component&& other) noexcept = delete;
        Component& operator=(Component&& other) noexcept = delete;

        virtual void Start() {};
        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void LateUpdate() {};
        virtual void Render() const {};

        virtual void OnCollisionEnter(Component* other);
        virtual void OnCollisionStay(Component* other);
        virtual void OnCollisionExit(Component* other);

        virtual void OnTriggerEnter(Component* other);
        virtual void OnTriggerStay(Component* other);
        virtual void OnTriggerExit(Component* other);

        void Destroy(GameObject* pGameObject);
        void Destroy(Component* pComponent);

        bool IsDestroyed() const { return m_Destroyed; }
        GameObject* GetGameObject() const { return m_pGameObject; }

        bool IsEnabled;
    protected:
        explicit Component(GameObject* pOwner);

    private:

        GameObject* m_pGameObject = nullptr;
        bool m_Destroyed{ false };
    };
}

