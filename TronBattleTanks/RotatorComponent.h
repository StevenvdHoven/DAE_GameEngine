#pragma once
#include "Component.h"
#include "Vector2.h"



class RotatorComponent final : public Engine::Component
{
public:
	RotatorComponent(Engine::GameObject* pOwner,const Engine::Vector2& center,bool clockWise, float rotationDistance, float angle, float speed);
	void Update() override;

private:
	Engine::Vector2 m_Center;
	bool m_ClockWise;
	float m_RotationDistance;
	float m_Angle;
	float m_Speed;	
};

