#include "RotatorComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"



RotatorComponent::RotatorComponent(GameObject* pOwner, const glm::vec2& center, bool clockWise, float rotationDistance, float angle, float speed):
	Component{ pOwner },
	m_Center{ center },
	m_ClockWise{ clockWise },
	m_RotationDistance{ rotationDistance },
	m_Angle{ angle },
	m_Speed{ speed }
{
}

void RotatorComponent::Update()
{
	if (m_ClockWise)
		m_Angle -= m_Speed * Time::GetInstance().GetDeltaTime();
	else
		m_Angle += m_Speed * Time::GetInstance().GetDeltaTime();
	
	GetGameObject()->GetTransform()->SetLocalPosition(m_Center + glm::vec2{ cos(m_Angle) * m_RotationDistance, sin(m_Angle) * m_RotationDistance });
}

