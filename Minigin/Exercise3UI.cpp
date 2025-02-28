#include "Exercise3UI.h"
#include "imgui.h"
#include <imgui_plot.h>
#include <chrono>


void Exercise3::Exercise3UI::Render() const
{
	ImGui::Begin("Exercise 3");

	if (ImGui::Button("Trash the Cache GameObject3D"))
	{
		// Do something
		const_cast<Exercise3UI*>(this)->CaculateSamples();
	}

	if (!m_Samples.empty())
	{
		float max = *std::max_element(m_Samples.begin(), m_Samples.end());
		
		ImGui::PlotConfig conf;
		conf.values.ys = m_Samples.data();
		conf.values.count = static_cast<int>(m_Samples.size());
		conf.scale.min = 0;
		conf.scale.max = max;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(160, 80);
		conf.line_thickness = 2.f;

		ImGui::Plot("GameObject3D", conf);
	}

	if (ImGui::Button("Trash the Cache GameObject3DAlt"))
	{
		// Do something
		const_cast<Exercise3UI*>(this)->CaculateSamplesAlt();
	}

	if(!m_SamplesAlt.empty())
	{
		float max = *std::max_element(m_SamplesAlt.begin(), m_SamplesAlt.end());

		ImGui::PlotConfig conf;
		conf.values.ys = m_SamplesAlt.data();
		conf.values.count = static_cast<int>(m_SamplesAlt.size());
		conf.scale.min = 0;
		conf.scale.max = max;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(160, 80);
		conf.line_thickness = 2.f;

		ImGui::Plot("GameObject3DAlt", conf);
	}

	if (!m_Samples.empty() && !m_SamplesAlt.empty())
	{
		// Compute max value for consistent scaling
		float maxCombined = std::max(
			*std::max_element(m_Samples.begin(), m_Samples.end()),
			*std::max_element(m_SamplesAlt.begin(), m_SamplesAlt.end())
		);

		static const ImU32 colors[] = { ImColor(0, 255, 0), ImColor(255, 0, 0) };
		const float* y_data[] = { m_Samples.data(), m_SamplesAlt.data() };

		// Configure plot settings
		ImGui::PlotConfig conf;
		conf.values.ys_list = y_data;
		conf.values.ys_count = 2;
		conf.values.count = static_cast<int>(std::min(m_Samples.size(), m_SamplesAlt.size())); // Prevent out-of-bounds access
		conf.values.colors = colors;
		conf.scale = { 0, maxCombined };
		conf.tooltip.show = true;
		conf.grid_x = { true, 128, 4 };
		conf.grid_y = { true, 0.5f, 5 };
		conf.frame_size = ImVec2(160, 80);

		ImGui::Plot("Combined Performance", conf);
	}


	ImGui::End();
}

void Exercise3::Exercise3UI::Destroy()
{
	m_Samples.clear();
}

void Exercise3::Exercise3UI::Init()
{

}

void Exercise3::Exercise3UI::CaculateSamples()
{
	m_Samples.clear();

	const int arrLength{ 10000000 };
	GameObject3D* arr = new GameObject3D[arrLength];

	for (int i = 0; i < arrLength; i++)
	{
		arr[i].ID = 1;
	}

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto time = std::chrono::system_clock::now();

		for (int i = 0; i < arrLength; i += stepsize)
		{
			arr[i].ID *= 2;
		}

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - time);
		m_Samples.emplace_back(static_cast<float>(duration.count()));
	}

	delete[] arr;
}

void Exercise3::Exercise3UI::CaculateSamplesAlt()
{
	m_SamplesAlt.clear();

	const int arrLength{ 10000000 };
	GameObject3DAlt* arr = new GameObject3DAlt[arrLength];

	for (int i = 0; i < arrLength; i++)
	{
		arr[i].ID = 1;
	}

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto time = std::chrono::system_clock::now();

		for (int i = 0; i < arrLength; i += stepsize)
		{
			arr[i].ID *= 2;
		}

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - time);
		m_SamplesAlt.emplace_back(static_cast<float>(duration.count()));
	}

	delete[] arr;
}
