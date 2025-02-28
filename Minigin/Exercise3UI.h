#pragma once
#include "Singleton.h"
#include <vector>

namespace Exercise3
{
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		Transform transform;
		int ID;
	};

	class GameObject3DAlt
	{
	public:
		Transform* transform;
		int ID;
	};

	class Exercise3UI : public Singleton<Exercise3UI>
	{
	public:
		void Render() const;
		void Destroy();
		void Init();

	private:
		friend class Singleton<Exercise3UI>;
		Exercise3UI() = default;

		void CaculateSamples();
		void CaculateSamplesAlt();

		std::vector<float> m_Samples;
		std::vector<float> m_SamplesAlt;
	};
}
