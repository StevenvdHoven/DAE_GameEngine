#pragma once
#include "Component.h"
#include "Observers.h"
#include <vector>

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

enum struct GameMode;

class ScoreComponent;
class PlayerHealthComponent;

class GameLoop final : public Engine::Component, public Engine::IObserver
{
public:
	GameLoop(Engine::GameObject* pOwner,GameMode mode, ScoreComponent* pScoreComponent);
	virtual ~GameLoop() = default;

	void Start() override;

	void BeginGame();

	// Inherited via IObserver
	void OnNotify(Component* sender) override;

	const GameState& GetGameState() const { return m_GameState; }
private:
	void HandleScoreInfo();
	void CreateStartText();

	void CreateSinglePlayerLoop();
	void CreatePVPPlayerLoop();
	void CreateCo_OpPlayerLoop();

	Engine::GameObject* m_pMapObject;
	std::vector< Engine::GameObject*> m_pPlayers;
	ScoreComponent* m_pScoreComponent;
	PlayerHealthComponent* m_pPlayerHealthComponent;


	Engine::TextRenderer* m_pStartText{ nullptr };

	GameState m_GameState;
	GameMode m_Mode;
	

};

