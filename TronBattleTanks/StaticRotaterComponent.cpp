#include "StaticRotaterComponent.h"
#include "GameObject.h"
#include "EngineTime.h"

using namespace Engine;

StaticRotaterComponent::StaticRotaterComponent(Engine::GameObject* pOwner):
	StaticRotaterComponent{pOwner,0,0}
{
}

StaticRotaterComponent::StaticRotaterComponent(Engine::GameObject* pOwner, float rotationRate, int rotationSteps):
	Component(pOwner),
	m_Rotate{false},
	m_TargetOrientation{0},
	m_RotationRate{rotationRate},
	m_RotationTimer{0.f},
	m_InitialRotation{0.f},
	m_RotationSteps{rotationSteps},
	m_CurrentRotationStep{0}
{
}

StaticRotaterComponent::~StaticRotaterComponent()
{
}

void StaticRotaterComponent::Update()
{
	if (!m_Rotate) return;

	m_RotationTimer += Time::GetInstance().GetDeltaTime();
	if (m_RotationTimer >= m_RotationRate)
	{
		m_RotationTimer = 0.f;

		const float t{ static_cast<float>(m_CurrentRotationStep + 1) / static_cast<float>(m_RotationSteps) };
		const float newRotation{ m_InitialRotation + t * (m_TargetOrientation - m_InitialRotation) };

		GetGameObject()->GetTransform()->SetWorldRotation(newRotation);

		++m_CurrentRotationStep;
		if (m_CurrentRotationStep >= m_RotationSteps)
		{
			m_Rotate = false;
		}
	}
}

void StaticRotaterComponent::SetTargetRotation(float targetRotation)
{
	if (m_Rotate) return;

	m_Rotate = true;
	m_InitialRotation = GetGameObject()->GetTransform()->GetWorldRotation();
	m_TargetOrientation = targetRotation;

	m_CurrentRotationStep = 0;
	m_RotationTimer = 0.f;
}

std::string StaticRotaterComponent::GetTypeName() const
{
	return "StaticRotaterComponent";
}
