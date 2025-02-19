#include "FPSCounterComponent.h"
#include "Time.h"
#include "TextRenderer.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "GameObject.h"

FPSCounterComponent::FPSCounterComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void FPSCounterComponent::Start()
{
	m_pTextRenderer = GetGameObject()->GetComponent<TextRenderer>();
}

void FPSCounterComponent::Update()
{
	m_FPS = 1.f / Time::GetInstance().GetDeltaTime();
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << m_FPS << " FPS";
	m_pTextRenderer->SetText(stream.str());
}
