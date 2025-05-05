#pragma once
#include <string>
#include "Component.h"

namespace Engine
{
	class Texture2D;
	class GameObject;

	enum class ImageAllignment
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Centre
	};

	class ImageRenderer final : public Component
	{
	public:
		ImageRenderer(GameObject* pOwner, const std::string& imagePath);

		void Update() override;
		void Render() const override;

		void ChangeImageAllignment(ImageAllignment allignment) { m_ImageAllignment = allignment; }


	private:
		Texture2D* m_pTexture;
		ImageAllignment m_ImageAllignment;
	};
}

