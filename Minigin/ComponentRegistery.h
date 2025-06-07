#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <unordered_map>
#include "GameObject.h"

using ComponentAdder = std::function<void(Engine::GameObject*, const nlohmann::json&)>;

class ComponentRegistery final
{
public:
    ComponentRegistery();

    template<typename T>
    void Register(const std::string& typeName) {
        m_Adders[typeName] = [](Engine::GameObject* obj, const nlohmann::json& data) {
            auto* comp = obj->AddComponent<T>();
            comp->Deserialize(data);
            };
    }

    void AddComponentByType(Engine::GameObject* obj, const std::string& typeName, const nlohmann::json& data) const {
        auto it = m_Adders.find(typeName);
        if (it != m_Adders.end()) {
            it->second(obj, data);
        }
    }

private:
    std::unordered_map<std::string, ComponentAdder> m_Adders;
};
     