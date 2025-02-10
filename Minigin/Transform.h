#pragma once
#include "Component.h"
#include <glm.hpp>


class Transform : public Component
{
public:
	const glm::vec2& GetPosition() const { return m_position; }
	void SetPosition(float x, float y);
private:
	glm::vec2 m_position;
};

