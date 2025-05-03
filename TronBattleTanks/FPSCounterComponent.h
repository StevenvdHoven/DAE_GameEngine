#pragma once
#include "Component.h"
#include "ValueWatchComponent.h"

namespace Engine
{
	class TextRenderer;
}


class FPSCounterComponent final : public Engine::Component
{
public:
	FPSCounterComponent(Engine::GameObject* pOwner);
	void Start() override;
	void Update() override;

private:
	Engine::TextRenderer* m_pTextRenderer;

	float m_FPS{ 0.f };
};

