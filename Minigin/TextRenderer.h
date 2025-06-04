#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Texture2D.h"
#include "Renderer.h"

class Font;
namespace Engine
{

	class TextRenderer final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		TextRenderer(GameObject* pOwner, const std::string& text, Font* font, const Engine::Color& textColor = { 255,255,255,255 });
		virtual ~TextRenderer() = default;
		TextRenderer(const TextRenderer& other) = delete;
		TextRenderer(TextRenderer&& other) = delete;
		TextRenderer& operator=(const TextRenderer& other) = delete;
		TextRenderer& operator=(TextRenderer&& other) = delete;

		Engine::Color& Color()
		{
			return m_TextColor;
		}
	private:
		bool m_needsUpdate;
		Engine::Color m_TextColor;
		std::string m_text;
		Font* m_font;
		std::unique_ptr<Texture2D> m_textTexture;
	
	};
}

