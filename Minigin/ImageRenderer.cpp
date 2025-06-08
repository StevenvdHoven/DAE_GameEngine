#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "json.hpp"
#include "imgui.h"

using namespace Engine;

Engine::ImageRenderer::ImageRenderer(GameObject* pOwner):
	ImageRenderer{pOwner,""}
{
}

Engine::ImageRenderer::ImageRenderer(GameObject* pOwner,const std::string& imagePath):
	Component{pOwner},
	m_Order{0},
	m_ImagePath{imagePath},
	m_pTexture{ ResourceManager::GetInstance().LoadTexture(imagePath)},
	m_ImageAllignment{ ImageAllignment::TopLeft }
{
	Renderer::GetInstance().Add(this);
}

Engine::ImageRenderer::~ImageRenderer()
{
	Renderer::GetInstance().Remove(this);
}

void Engine::ImageRenderer::Update()
{
}

void Engine::ImageRenderer::Render() const
{
	const Transform* transform{ GetGameObject()->GetTransform()};

	auto pos{ transform->GetWorldLocation() };
	auto rot{ transform->GetWorldRotation() };

	Vector2 size{ 0,0 };
	m_pTexture->GetSize(size);

	SDL_Point pivot{ 0, 0 };

	switch (m_ImageAllignment)
	{
	case Engine::ImageAllignment::TopRight:
		pos.x -= size.x;
		pivot.x = static_cast<int>(size.x);
		pivot.y = 0;
		break;
	case Engine::ImageAllignment::BottomLeft:
		pos.y -= size.y;
		pivot.x = 0;
		pivot.y = static_cast<int>(size.y);
		break;
	case Engine::ImageAllignment::BottomRight:
		pos.x -= size.x;
		pos.y -= size.y;
		pivot.x = static_cast<int>(size.x);
		pivot.y = static_cast<int>(size.y);
		break;
	case Engine::ImageAllignment::Centre:
		pos.x -= size.x / 2;
		pos.y -= size.y / 2;
		pivot.x = static_cast<int>(size.x / 2);
		pivot.y = static_cast<int>(size.y / 2);
		break;
	case Engine::ImageAllignment::Custom:
		pos.x -= m_Pivot.x;
		pos.y -= m_Pivot.y;
		pivot.x = 0;
		pivot.y = 0;
		break;
	}

	pivot.x += static_cast<int>(m_Pivot.x);
	pivot.y += static_cast<int>(m_Pivot.y);

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, rot,&pivot);
	Renderer::GetInstance().SetColor(SDL_Color{ 0, 0, 255, 255 });
	Renderer::GetInstance().RenderRectangle(pos, size.x, size.y);
}

void Engine::ImageRenderer::GUI()
{
	ImGui::Text("Image Renderer Component");
	ImGui::Separator();

	char pathBuffer[256];
	strcpy_s(pathBuffer, m_ImagePath.c_str());

	int order{ m_Order };
	if (ImGui::InputInt("Order", &order))
	{
		m_Order = order;
		Renderer::GetInstance().Reorder();
	}

	if (ImGui::InputText("Image Path", pathBuffer, sizeof(pathBuffer)))
	{
		m_ImagePath = pathBuffer;
		m_pTexture = ResourceManager::GetInstance().LoadTexture(m_ImagePath);
	}

	const char* alignmentOptions[] = {
		"Top Left", "Top Right", "Bottom Left", "Bottom Right",
		"Left", "Right", "Top", "Bottom", "Centre", "Custom"
	};

	int currentSelection = static_cast<int>(m_ImageAllignment);
	if (ImGui::Combo("Alignment", &currentSelection, alignmentOptions, IM_ARRAYSIZE(alignmentOptions)))
	{
		m_ImageAllignment = static_cast<ImageAllignment>(currentSelection);
	}

	// Show pivot if Custom alignment
	if (m_ImageAllignment == ImageAllignment::Custom)
	{
		Vector2 pivot = m_Pivot;
		if (ImGui::InputFloat2("Pivot", &pivot.x))
		{
			SetPivot(pivot);
		}
	}

}

void Engine::ImageRenderer::Serialize(nlohmann::json& json) const
{
	json["image_order"] = m_Order;
	json["image_path"] = m_ImagePath;
	json["image_allignment"] = static_cast<int>(m_ImageAllignment);
	json["image_pivot"] = m_Pivot.Serialize();
}

void Engine::ImageRenderer::Deserialize(const nlohmann::json& json)
{
	m_Order = json["image_order"].get<int>();
	m_ImagePath = json["image_path"].get<std::string>();
	m_pTexture = ResourceManager::GetInstance().LoadTexture(m_ImagePath);
	m_ImageAllignment = static_cast<ImageAllignment>(json["image_allignment"].get<int>());
	auto pivotJson{ json["image_pivot"] };
	m_Pivot.Deserialize(pivotJson);
}

std::string Engine::ImageRenderer::GetTypeName() const
{
	return "ImageRenderer";
}
