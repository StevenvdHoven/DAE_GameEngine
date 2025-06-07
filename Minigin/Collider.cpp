#include "Collider.h"
#include "ServiceLocator.h"
#include "PhysicsSystem.h"

Engine::Collider::Collider(GameObject* pOwner, LayerMask mask):
	Component(pOwner),
	m_IsTrigger{ false },
	m_LayerMask{mask}
{
	// Register the collider with the physics system
	ServiceLocator::GetPhysicsSystem().AddCollider(this);
}

Engine::Collider::~Collider()
{
	// Unregister the collider from the physics system
	ServiceLocator::GetPhysicsSystem().RemoveCollider(this);
}

void Engine::Collider::Serialize(nlohmann::json& out) const
{
	nlohmann::json colliderJson;
	colliderJson["collider_is_trigger"] = m_IsTrigger;
	colliderJson["collider_layer_mask"] = static_cast<int>(m_LayerMask);
	nlohmann::json ignoreMaskJson;
	std::for_each(m_IgnoreMask.begin(), m_IgnoreMask.end(), [&ignoreMaskJson](const LayerMask& mask)
		{
			ignoreMaskJson.emplace_back(static_cast<int>(mask));
		});
	colliderJson["collider_ignore_mask"] = ignoreMaskJson;
	out["component_collider"] = colliderJson;
}

void Engine::Collider::Deserialize(const nlohmann::json& in)
{
	nlohmann::json colliderJson = in["component_collider"];
	m_IsTrigger = colliderJson["collider_is_trigger"];
	m_LayerMask = static_cast<LayerMask>(colliderJson["collider_layer_mask"]);

	m_IgnoreMask.clear();
	const nlohmann::json& ignoreMaskJson = colliderJson["collider_ignore_mask"];
	for (const auto& maskValue : ignoreMaskJson)
	{
		m_IgnoreMask.emplace(static_cast<LayerMask>(maskValue.get<int>()));
	}
}

std::string Engine::Collider::GetTypeName() const
{
	return std::string();
}
