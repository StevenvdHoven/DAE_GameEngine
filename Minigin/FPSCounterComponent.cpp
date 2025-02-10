#include "FPSCounterComponent.h"
#include "Time.h"
#include "TextRenderer.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "GameObject.h"

void FPSCounterComponent::Start()
{
	m_pTextRenderer = GetGameObject()->GetComponent<TextRenderer>();
}

void FPSCounterComponent::Update()
{
	if (m_pTextRenderer != nullptr)
	{ 
		float delta{ Time::GetInstance().GetDeltaTime() };
		float fps{ 1.f / delta };
		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << fps << " FPS";
		m_pTextRenderer->SetText(stream.str());
	}
}
