#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRenderer.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"

using namespace Engine;

Engine::TextRenderer::TextRenderer(GameObject* pOwner):
	TextRenderer{ pOwner,"",nullptr,Engine::Color{0,0,0,0} }
{
}

Engine::TextRenderer::TextRenderer(GameObject* pOwner, const std::string& text, Font* font, const Engine::Color& textColor) :
	Component{pOwner}, 
	m_needsUpdate{ true },
	m_TextColor{textColor},
	m_text{ text },
	m_font{ std::move(font) },
	m_textTexture{ nullptr }
{ }

void Engine::TextRenderer::Serialize(nlohmann::json & json) const
{
	nlohmann::json textRendererJson;
	textRendererJson["textrenderer_text"] = m_text;
	textRendererJson["textrenderer_color"] = m_TextColor.Serialize();
	json["component_text_renderer"] = textRendererJson;
}

void Engine::TextRenderer::Deserialize(const nlohmann::json& json)
{
	nlohmann::json textRendererJson{ json["component_text_renderer"] };

	m_needsUpdate = true;
	m_text = textRendererJson["textrender_text"];
	m_TextColor.Desrialize(textRendererJson["textrenderer_color"]);
}

std::string Engine::TextRenderer::GetTypeName() const
{
	return "TextRenderer";
}

void Engine::TextRenderer::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color{ m_TextColor.ToSDLColor() };
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
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


