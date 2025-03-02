#pragma once
#include "Singleton.h"
#include "GUI.h"
#include <vector>
#include <memory>

class ViewPort final : public Singleton<ViewPort>
{
public:
	GUI* AddGUI(std::unique_ptr<GUI> pGUI);

	void Render();

private:
	std::vector<std::unique_ptr<GUI>> m_pGUIs;
};

