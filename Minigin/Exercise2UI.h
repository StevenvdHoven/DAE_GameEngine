#pragma once
#include "imgui.h"
#include "Singleton.h"
#include <vector>

class Exercise2UI final : public Singleton<Exercise2UI>
{
public:
	void Render() const;
	void Destroy();
	void Init();

private:
	void CaculateSamples();

	std::vector<float> m_Samples;
};

