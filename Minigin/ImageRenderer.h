#pragma once
#include <string>
#include "Component.h"

namespace Engine
{
	class Texture2D;
	class GameObject;

	class ImageRenderer final : public Component
	{
	public:
		ImageRenderer(GameObject* pOwner, const std::string& imagePath);

		void Update() override;
		void Render() const override;

	private:
		Texture2D* m_pTexture;
	};
}

