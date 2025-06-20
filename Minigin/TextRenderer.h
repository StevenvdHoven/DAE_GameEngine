#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"

class Font;
namespace Engine
{

	class TextRenderer final : public RenderComponent
	{
	public:
		TextRenderer(GameObject* pOwner);
		TextRenderer(GameObject* pOwner, const std::string& text, const std::string& fontText,int size = 30, const Engine::Color& textColor = { 255,255,255,255 });
		virtual ~TextRenderer();

		TextRenderer(const TextRenderer& other) = delete;
		TextRenderer(TextRenderer&& other) = delete;
		TextRenderer& operator=(const TextRenderer& other) = delete;
		TextRenderer& operator=(TextRenderer&& other) = delete;

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		Engine::Color& Color()
		{
			m_needsUpdate = true;
			return m_TextColor;
		}

		void GUI() override;

		void Serialize(nlohmann::json& json) const override;
		void Deserialize(const nlohmann::json& json) override;
		std::string GetTypeName() const override;
	private:

		bool m_needsUpdate;
		std::string m_text;
		int m_FontSize;
		std::string m_FontText;
		Engine::Color m_TextColor;
		Font* m_font;
		std::unique_ptr<Texture2D> m_textTexture;
	};
}

