#include "Exercise2UI.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <chrono>

void Exercise2UI::Render()
{
	ImGui::Begin("Exercise 2");

	ImGui::InputInt("Samples", &m_SampleAmount);

	if (ImGui::Button("Trash the Cache"))
	{
		// Do something
		CaculateSamples();
	}

	if (!m_Samples.empty())
	{
		float max = *std::max_element(m_Samples.begin(), m_Samples.end());

		ImGui::PlotConfig conf{};
		conf.values.xs = m_Steps.data();
		conf.values.ys = m_Samples.data();
		conf.values.count = static_cast<int>(m_Steps.size());
		conf.values.ys_count = 1;
		conf.scale.min = 0;
		conf.scale.max = max;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.frame_size = ImVec2(160, 80);
		conf.line_thickness = 2.f;

		ImGui::Plot("Performance", conf);
	}

	ImGui::End();
}

void Exercise2UI::Update()
{

}

void Exercise2UI::CaculateSamples()
{
	m_Samples.clear();

	const int arrLength{ 1000000 };
	int* arr = new int[arrLength];

	for (int i = 0; i < arrLength; i++)
	{
		arr[i] = 1;
	}

	for (int i = 0; i < m_SampleAmount; i++)
	{
		int stepsize = 1 << i;

		if (stepsize > arrLength)
			break;

		auto time = std::chrono::system_clock::now();

		for (int j = 0; j < arrLength; j += stepsize)
		{
			arr[j] *= 2;
		}

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - time);
		m_Samples.emplace_back(static_cast<float>(duration.count()));
	}

	delete[] arr;
}
