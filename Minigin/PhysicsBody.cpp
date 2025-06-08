#include "PhysicsBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysicsSystem.h"
#include "ServiceLocator.h"
#include "imgui.h"

Engine::PhysicsBody::PhysicsBody(GameObject* pOwner) :
	Component(pOwner),
	Velocity{ 0.f, 0.f }
{
	ServiceLocator::GetPhysicsSystem().AddPhysicsBody(this);
}

Engine::PhysicsBody::~PhysicsBody()
{
	ServiceLocator::GetPhysicsSystem().RemovePhysicsBody(this);
}

void Engine::PhysicsBody::Serialize(nlohmann::json& json) const
{
	nlohmann::json physcisBodyJson;
	physcisBodyJson["body_velocity"] = Velocity.Serialize();
	json["component_physics_body"] = physcisBodyJson;
}

void Engine::PhysicsBody::Deserialize(const nlohmann::json& json)
{
	nlohmann::json physcisBodyJson{ json["component_physics_body"] };
	Velocity.Deserialize(physcisBodyJson["body_velocity"]);
}

std::string Engine::PhysicsBody::GetTypeName() const
{
	return "PhysicsBody";
}

void Engine::PhysicsBody::GUI()
{
	ImGui::Text("Velocity: (%.2f, %.2f)", Velocity.x, Velocity.y);
	ImGui::DragFloat2("Velocity", &Velocity.x, 0.1f);
}
