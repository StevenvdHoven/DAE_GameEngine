#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRenderer.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"

using namespace Engine;

TextRenderer::TextRenderer(GameObject* pOwner, const std::string& text, Font* font):
	Component{pOwner}, m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{ }

void TextRenderer::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
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

void TextRenderer::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetGameObject()->GetTransform()->GetLocalPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void TextRenderer::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void TextRenderer::SetPosition(const float x, const float y)
{
	GetGameObject()->GetTransform()->SetLocalPosition(x, y);
}


