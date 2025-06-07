#pragma once
#include <memory>
#include "Component.h"
#include "Observers.h"
#include "TextRenderer.h"

class ScoreComponent final : public Engine::Component
{
public:
	ScoreComponent(Engine::GameObject* pOwner);
	~ScoreComponent();

	void Start() override;

	int GetScore() const;
	void AddScore(int score);

	Engine::Subject* GetOnScoreChange() const { return m_OnScoreChange.get(); }

	std::string GetTypeName() const override;

private:
	int m_Score;
	Engine::TextRenderer* m_pTextRenderer;
	std::unique_ptr<Engine::Subject> m_OnScoreChange;
	
};

