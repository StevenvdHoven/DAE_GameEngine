#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Texture2D.h"

class Font;
class TextRenderer final : public Component
{
public:
	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);

	TextRenderer(GameObject* pOwner,const std::string& text, Font* font);
	virtual ~TextRenderer() = default;
	TextRenderer(const TextRenderer& other) = delete;
	TextRenderer(TextRenderer&& other) = delete;
	TextRenderer& operator=(const TextRenderer& other) = delete;
	TextRenderer& operator=(TextRenderer&& other) = delete;
private:
	bool m_needsUpdate;
	std::string m_text;
	Font* m_font;
	std::unique_ptr<Texture2D> m_textTexture;
};

