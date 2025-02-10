#pragma once
#include "Component.h"

class TextRenderer;
class FPSCounterComponent final : public Component
{
public:
	void Start() override;
	void Update() override;

private:
	TextRenderer* m_pTextRenderer;
};

