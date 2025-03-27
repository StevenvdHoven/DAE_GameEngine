#pragma once
#include "Singleton.h"

namespace Engine
{
	class GameEngine;

	class Time final : public Singleton<Time>
	{
	public:
		float timeScale = 1;
		float fixedTimeScale = 1;

		float GetDeltaTime() { return m_DeltaTime * timeScale; }
		float GetFixedDeltaTime() { return m_FixedDeltaTime * m_FixedDeltaTime; }

		float GetDelta() { return m_DeltaTime; }

	private:
		friend class Singleton<Time>;
		friend class GameEngine;
		Time() = default;

		float m_DeltaTime = 0.f;
		float m_FixedDeltaTime = 0.f;

	};
}

