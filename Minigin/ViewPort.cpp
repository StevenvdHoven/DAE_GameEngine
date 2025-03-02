#include "ViewPort.h"
#include "GUI.h"
#include "imgui.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>


GUI* ViewPort::AddGUI(std::unique_ptr<GUI> pGUI)
{
	auto rawPtr = pGUI.get();
	m_pGUIs.emplace_back(std::move(pGUI));
    return rawPtr;
}

void ViewPort::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	for (auto& pGUI : m_pGUIs)
	{
		pGUI->Render();
	}
}
