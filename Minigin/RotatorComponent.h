#pragma once
#include "Component.h"
#include "Vector2.h"

using namespace Engine;

class RotatorComponent final : public Component
{
public:
	RotatorComponent(GameObject* pOwner,const Vector2& center,bool clockWise, float rotationDistance, float angle, float speed);
	void Update() override;

private:
	Vector2 m_Center;
	bool m_ClockWise;
	float m_RotationDistance;
	float m_Angle;
	float m_Speed;	
};

