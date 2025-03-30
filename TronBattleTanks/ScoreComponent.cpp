#include "ScoreComponent.h"
#include "GameObject.h"
#include "TextRenderer.h"

ScoreComponent::ScoreComponent(GameObject* pOwner) :
	Component{ pOwner },
	m_Score{ 0 }
{
	m_OnScoreChange = std::make_unique<Engine::Subject>();
}

ScoreComponent::~ScoreComponent()
{
}

void ScoreComponent::Start()
{
	m_pTextRenderer = GetGameObject()->GetComponent<TextRenderer>();
	m_pTextRenderer->SetText("Score: " + std::to_string(m_Score));
}

int ScoreComponent::GetScore() const
{
	return m_Score;
}

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	m_pTextRenderer->SetText("Score: " + std::to_string(m_Score));

	m_OnScoreChange->Notify();
}
