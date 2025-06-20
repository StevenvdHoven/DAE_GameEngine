#pragma once
#include <string>
#include "RenderComponent.h"
#include "Vector2.h"

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
		Left,
		Right,
		Top,
		Bottom,
		Centre,
		Custom,
	};

	class ImageRenderer final : public RenderComponent
	{
	public:
		ImageRenderer(GameObject* pOwner);
		ImageRenderer(GameObject* pOwner, const std::string& imagePath);
		virtual ~ImageRenderer() = default;

		void Update() override;
		void Render() const override;

		void ChangeImageAllignment(ImageAllignment allignment) { m_ImageAllignment = allignment; }
		void SetPivot(const Vector2& pivot) { m_Pivot = pivot; }
		void SetImage(Texture2D* pImage) { m_pTexture = pImage; }

		void GUI() override;

		void Serialize(nlohmann::json& json) const override;
		void Deserialize(const nlohmann::json& json) override;
		std::string GetTypeName() const override;

	private:
		std::string m_ImagePath;
		Texture2D* m_pTexture;
		ImageAllignment m_ImageAllignment;
		Vector2 m_Pivot;
	
	};
}

