#include "Collider.h"
#include "ServiceLocator.h"
#include "imgui.h"

Engine::Collider::Collider(GameObject* pOwner) :
	Component{ pOwner },
	m_IsTrigger{ false },
	m_LayerMask{ LayerMask::Default }
{
}

Engine::Collider::Collider(GameObject* pOwner, LayerMask mask):
	Component(pOwner),
	m_IsTrigger{ false },
	m_LayerMask{mask}
{
	ServiceLocator::GetPhysicsSystem().AddCollider(this);
}

Engine::Collider::~Collider()
{
	ServiceLocator::GetPhysicsSystem().RemoveCollider(this);
}

void Engine::Collider::Serialize(nlohmann::json& json) const
{
	json["collider_is_trigger"] = m_IsTrigger;
	json["collider_layer_mask"] = static_cast<int>(m_LayerMask);
	nlohmann::json ignoreMaskJson;
	std::for_each(m_IgnoreMask.begin(), m_IgnoreMask.end(), [&ignoreMaskJson](const LayerMask& mask)
		{
			ignoreMaskJson.emplace_back(static_cast<int>(mask));
		});
	json["collider_ignore_mask"] = ignoreMaskJson;
	json["collider_center"] = Center.Serialize();
}

void Engine::Collider::Deserialize(const nlohmann::json& json)
{
	m_IsTrigger = json["collider_is_trigger"];
	m_LayerMask = static_cast<LayerMask>(json["collider_layer_mask"]);

	m_IgnoreMask.clear();
	const nlohmann::json& ignoreMaskJson = json["collider_ignore_mask"];
	for (const auto& maskValue : ignoreMaskJson)
	{
		m_IgnoreMask.emplace(static_cast<LayerMask>(maskValue.get<int>()));
	}
	auto CenterJson = json["collider_center"];
	Center.Deserialize(CenterJson);
}

std::string Engine::Collider::GetTypeName() const
{
	return std::string();
}

void Engine::Collider::GUI()
{
	ImGui::Text("Collider Component");
	ImGui::Separator();

	// Trigger checkbox
	bool isTrigger = m_IsTrigger;
	if (ImGui::Checkbox("Is Trigger", &isTrigger))
	{
		SetIsTrigger(isTrigger);
	}

	// Center offset
	Vector2 center = Center;
	if (ImGui::InputFloat2("Center Offset", &center.x))
	{
		Center = center;
	}

	// Layer mask dropdown
	const char* layerNames[] = { "Default", "Player", "Enemy", "Projectile", "Wall" };
	static int currentLayer = static_cast<int>(m_LayerMask);

	if (ImGui::Combo("Layer", &currentLayer, layerNames, IM_ARRAYSIZE(layerNames)))
	{
		m_LayerMask = static_cast<LayerMask>(1 << currentLayer);
	}

	// Ignore layers multi-select
	ImGui::Text("Ignore Layers:");
	for (int i = 0; i < IM_ARRAYSIZE(layerNames); ++i)
	{
		LayerMask mask = static_cast<LayerMask>(1 << i);
		bool ignored = m_IgnoreMask.find(mask) != m_IgnoreMask.end();

		if (ImGui::Checkbox(layerNames[i], &ignored))
		{
			if (ignored)
				m_IgnoreMask.insert(mask);
			else
				m_IgnoreMask.erase(mask);
		}
	}
}
