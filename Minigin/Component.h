#pragma once
#include <xmemory>
#include "json.hpp"

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

        virtual void OnCollisionEnter(GameObject* other);
        virtual void OnCollisionStay(GameObject* other);
        virtual void OnCollisionExit(GameObject* other);

        virtual void OnTriggerEnter(GameObject* other);
        virtual void OnTriggerStay(GameObject* other);
        virtual void OnTriggerExit(GameObject* other);

        virtual void Serialize(nlohmann::json&) const {};
        virtual void Deserialize(const nlohmann::json&) {};
        virtual std::string GetTypeName() const = 0;

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

