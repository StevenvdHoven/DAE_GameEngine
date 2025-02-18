#pragma once
#include "Component.h"
#include <glm.hpp>

class RotatorComponent final : public Component
{
public:
	RotatorComponent(GameObject* pOwner,const glm::vec2& center,bool clockWise, float rotationDistance, float angle, float speed);
	void Update() override;

private:
	glm::vec2 m_Center;
	bool m_ClockWise;
	float m_RotationDistance;
	float m_Angle;
	float m_Speed;	
};

