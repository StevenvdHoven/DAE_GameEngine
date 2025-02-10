#pragma once
#include <string>
#include "Component.h"


class Texture2D;
class ImageRenderer final : public Component
{
public:
	ImageRenderer(const std::string& imagePath);

	void Update() override;
	void Render() const override;

private:
	Texture2D* m_pTexture;
};

