#pragma once
#include "Singleton.h"
#include <vector>
#include "GUI.h"

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

	class Exercise3UI final : public GUI
	{
	public:
		Exercise3UI() = default;
		void Render();
		void Update();

	private:
		void CaculateSamples();
		void CaculateSamplesAlt();

		int m_SampleAmount = 100;
		std::vector<float> m_Samples;
		std::vector<float> m_SamplesAlt;
		std::vector<float> m_Steps{1, 2, 4, 8, 16, 32, 64 , 128, 256, 512, 1024};
	};
}
