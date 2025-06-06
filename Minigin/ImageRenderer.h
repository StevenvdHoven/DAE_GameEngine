#pragma once
#include <string>
#include "Component.h"
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

	class ImageRenderer final : public Component
	{
	public:
		ImageRenderer(GameObject* pOwner);
		ImageRenderer(GameObject* pOwner, const std::string& imagePath);

		void Update() override;
		void Render() const override;

		void ChangeImageAllignment(ImageAllignment allignment) { m_ImageAllignment = allignment; }
		void SetPivot(const Vector2& pivot) { m_Pivot = pivot; }

		void Serialize(nlohmann::json& json) const override;
		void Deserialize(const nlohmann::json& json) override;
		std::string GetTypeName() const override;

	private:
		const std::string m_ImagePath;
		Texture2D* m_pTexture;
		ImageAllignment m_ImageAllignment;
		Vector2 m_Pivot;
	
	};
}

