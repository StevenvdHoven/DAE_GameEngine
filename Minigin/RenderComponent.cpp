#include "RenderComponent.h"
#include "Renderer.h"
#include "imgui.h"

Engine::RenderComponent::RenderComponent(GameObject* pOwner):
	Engine::Component{pOwner},
	m_Order{0}
{
	Renderer::GetInstance().Add(this);
}

Engine::RenderComponent::~RenderComponent()
{
	Renderer::GetInstance().Remove(this);
}

void Engine::RenderComponent::GUI()
{
	int order{ m_Order };
	if (ImGui::InputInt("Order", &order))
	{
		m_Order = order;
		Renderer::GetInstance().Reorder();
	}
}

void Engine::RenderComponent::Serialize(nlohmann::json& json) const
{
	json["render_component_order"] = m_Order;
}

void Engine::RenderComponent::Deserialize(const nlohmann::json& json)
{
	m_Order = json["render_component_order"].get<int>();
}

std::string Engine::RenderComponent::GetTypeName() const
{
	return "RenderComponent";
}
