#pragma once
#include <memory>
#include "Component.h"
#include "Observers.h"

class TextRenderer;
class ScoreComponent final : public Component
{
public:
	ScoreComponent(GameObject* pOwner);
	~ScoreComponent();

	void Start() override;

	int GetScore() const;
	void AddScore(int score);

	Engine::Subject* GetOnScoreChange() const { return m_OnScoreChange.get(); }

private:
	int m_Score;
	TextRenderer* m_pTextRenderer;
	std::unique_ptr<Engine::Subject> m_OnScoreChange;
	
};

