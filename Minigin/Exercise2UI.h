#pragma once
#include "imgui.h"
#include "Singleton.h"
#include <vector>
#include "GUI.h"

class Exercise2UI final : public GUI
{
public:
	void Render();
	void Update();

private:
	void CaculateSamples();

	int m_SampleAmount = 10;
	std::vector<float> m_Samples;
	std::vector<float> m_Steps{ 1, 2, 4, 8, 16, 32, 64 , 128, 256, 512, 1024 };
};

