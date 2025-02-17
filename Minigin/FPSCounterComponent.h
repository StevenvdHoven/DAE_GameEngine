#pragma once
#include "Component.h"
#include "ValueWatchComponent.h"


class TextRenderer;
class FPSCounterComponent final : public Component
{
public:
	FPSCounterComponent(GameObject* pOwner);
	void Start() override;
	void Update() override;

private:
	TextRenderer* m_pTextRenderer;
	ValueWatchComponent<float, std::function<void(float)>>* m_pValueWatchComponent;

	float m_FPS{ 0.f };
};

