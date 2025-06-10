#pragma once
#include "Component.h"
#include <vector>

namespace Engine
{
	class Texture2D;
	class ImageRenderer;

	class Animation : public Engine::Component
	{
	public:
		Animation(GameObject* pOwner);

		void Start() override;
		void Update() override;

		void Play();
		void Stop();

		void SetPlayRate(float playRate);
		void SetSequence(std::vector<std::string>&& filePath);

		virtual void GUI();

		virtual void Serialize(nlohmann::json& json) const;
		virtual void Deserialize(const nlohmann::json& json);
		std::string GetTypeName() const override;

	private:
		bool m_PlayAtStart;
		bool m_Playing;
		bool m_Loop;
		int m_AnimationIndex;

		float m_PlayRate;
		float m_AnimationTimer;

		ImageRenderer* m_Renderer;
		std::vector<std::string> m_AnimationImagePaths;
		std::vector<Texture2D*> m_AnimationImages;
	};
}

