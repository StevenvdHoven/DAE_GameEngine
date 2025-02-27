#include "Exercise2UI.h"
#include "imgui.h"
#include <chrono>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

void Exercise2UI::Render() const
{
	ImGui::NewFrame();

	ImGui::Text("Exercise 2");

	if (ImGui::Button("Button"))
	{
		// Do something
		const_cast<Exercise2UI*>(this)->CaculateSamples();
	}

	if (!m_Samples.empty())
	{
		float max = *std::max_element(m_Samples.begin(), m_Samples.end());
		ImGui::PlotLines("Performance", m_Samples.data(), static_cast<int>(m_Samples.size()), 0, nullptr, 0, max, ImVec2(0, 80));
	}

}

void Exercise2UI::Destroy()
{
	m_Samples.clear();
}

void Exercise2UI::Init()
{

}

void Exercise2UI::CaculateSamples()
{
	m_Samples.clear();

	const int arrLength{ 1000000000 };
	int* arr = new int[arrLength];

	for (int i = 0; i < arrLength; i++)
	{
		arr[i] = 1;
	}

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto time = std::chrono::system_clock::now();

		for (int i = 0; i < arrLength; i += stepsize)
		{
			arr[i] *= 2;
		}

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time);
		m_Samples.emplace_back(static_cast<float>( duration.count()));
	}

	delete[] arr;
}
