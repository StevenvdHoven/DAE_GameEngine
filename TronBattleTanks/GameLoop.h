#pragma once
#include "Component.h"
#include "Observers.h"

namespace Engine
{
	class TextRenderer;
}

enum struct GameState
{
	Start,
	Running,
	GameOver
};

class ScoreComponent;
class PlayerHealthComponent;

class GameLoop final : public Engine::Component, public Engine::IObserver
{
public:
	GameLoop(Engine::GameObject* pOwner, ScoreComponent* pScoreComponent);
	virtual ~GameLoop() = default;

	void Start() override;

	void BeginGame();

	// Inherited via IObserver
	void OnNotify(Component* sender) override;

	const GameState& GetGameState() const { return m_GameState; }
private:
	void HandleScoreInfo();

	void CreateStartText();

	Engine::GameObject* m_pMapObject;
	Engine::GameObject* m_pPlayerObject;
	ScoreComponent* m_pScoreComponent;
	PlayerHealthComponent* m_pPlayerHealthComponent;


	Engine::TextRenderer* m_pStartText{ nullptr };

	GameState m_GameState{ GameState::Start };

};

