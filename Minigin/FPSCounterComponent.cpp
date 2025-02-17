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
	m_pValueWatchComponent = GetGameObject()->GetComponent<ValueWatchComponent<float, std::function<void(float)>>>();
	
	m_pValueWatchComponent->SetWatchValue(&m_FPS);
	m_pValueWatchComponent->AddListener([&](float value)
		{
			float fps{ value };
			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << fps << " FPS";
			m_pTextRenderer->SetText(stream.str());
		});
}

void FPSCounterComponent::Update()
{
	m_FPS = 1.f / Time::GetInstance().GetDeltaTime();
}
