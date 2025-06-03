#pragma once
#include "Component.h"

class StaticRotaterComponent : public Engine::Component
{
public:
	StaticRotaterComponent(Engine::GameObject* pOwner, float rotationRate, int rotationSteps);
	virtual ~StaticRotaterComponent();

	void Update() override;
	
	void SetTargetRotation(float targetRotation);

private:
	bool m_Rotate;
	float m_TargetOrientation;
	float m_RotationRate;
	float m_RotationTimer;
	float m_InitialRotation;
	int m_RotationSteps;
	int m_CurrentRotationStep;
};

