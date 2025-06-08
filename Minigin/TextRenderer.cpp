#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRenderer.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "imgui.h"

using namespace Engine;

Engine::TextRenderer::TextRenderer(GameObject* pOwner) :
	TextRenderer{ pOwner,"","" }
{
}

Engine::TextRenderer::TextRenderer(GameObject* pOwner, const std::string& text, const std::string& fontText, int size, const Engine::Color& textColor) :
	RenderComponent{ pOwner },
	m_needsUpdate{ true },
	m_text{ text },
	m_FontSize{ size },
	m_FontText{ fontText },
	m_TextColor{ textColor },
	m_font{ ResourceManager::GetInstance().LoadFont(fontText, static_cast<uint8_t>(size)) },
	m_textTexture{ nullptr }
{
}

Engine::TextRenderer::~TextRenderer()
{
}

void Engine::TextRenderer::GUI()
{
	RenderComponent::GUI();

	ImGui::Text("Text Renderer Component");
	ImGui::Separator();

	char textBuffer[256];
	strcpy_s(textBuffer, sizeof(textBuffer), m_text.c_str());
	if (ImGui::InputText("Text", textBuffer, sizeof(textBuffer)))
	{
		m_text = textBuffer;
		m_needsUpdate = true;
		Update();
	}

	char fontBuffer[256];
	strcpy_s(fontBuffer, sizeof(fontBuffer), m_FontText.c_str());
	if (ImGui::InputText("Font", fontBuffer, sizeof(fontBuffer)))
	{
		m_FontText = fontBuffer;
		m_needsUpdate = true;
		Update();
	}

	if (ImGui::InputInt("Font Size", &m_FontSize))
	{
		if (m_FontSize < 1) m_FontSize = 1;
		m_needsUpdate = true;
		Update();
	}

	float color[4] = {
		m_TextColor.r / 255.f,
		m_TextColor.g / 255.f,
		m_TextColor.b / 255.f,
		m_TextColor.a / 255.f
	};

	if (ImGui::ColorEdit4("Color", color))
	{
		m_TextColor = {
			static_cast<uint8_t>(color[0] * 255),
			static_cast<uint8_t>(color[1] * 255),
			static_cast<uint8_t>(color[2] * 255),
			static_cast<uint8_t>(color[3] * 255)
		};
		m_needsUpdate = true;
	}

}

void Engine::TextRenderer::Serialize(nlohmann::json& json) const
{
	RenderComponent::Serialize(json);
	json["text_renderer_text"] = m_text;
	json["text_renderer_font"] = m_FontText;
	json["text_renderer_font_size"] = m_FontSize;
	json["text_renderer_color"] = m_TextColor.Serialize();
}

void Engine::TextRenderer::Deserialize(const nlohmann::json& json)
{
	RenderComponent::Deserialize(json);

	m_needsUpdate = true;
	m_text = json["text_renderer_text"].get<std::string>();
	m_FontText = json["text_renderer_font"].get<std::string>();
	m_FontSize = json["text_renderer_font_size"].get<int>();
	auto colorJson{ json["text_renderer_color"] };
	m_TextColor.Desrialize(colorJson);
}

std::string Engine::TextRenderer::GetTypeName() const
{
	return "TextRenderer";
}

void Engine::TextRenderer::Update()
{
	if (m_needsUpdate)
	{
		m_font = ResourceManager::GetInstance().LoadFont(m_FontText, static_cast<uint8_t>(m_FontSize));

		const SDL_Color color{ m_TextColor.ToSDLColor() };
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			m_needsUpdate = false;
			return;
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			m_needsUpdate = false;
			return;
		}
		SDL_FreeSurface(surf);
		m_textTexture.reset();
		m_textTexture = std::make_unique<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void Engine::TextRenderer::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetGameObject()->GetTransform()->GetWorldLocation();
		Renderer::GetInstance().RenderTexture(*m_textTexture.get(), pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void Engine::TextRenderer::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void Engine::TextRenderer::SetPosition(const float x, const float y)
{
	GetGameObject()->GetTransform()->SetLocalPosition(x, y);
}


