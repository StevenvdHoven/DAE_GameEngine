#pragma once
#include "Component.h"
#include "Observers.h"
#include <vector>
#include <forward_list>
#include "StartGameCommand.h"

namespace Engine
{
	class TextRenderer;
	class Scene;
	struct Vector2;
}

enum struct GameMode;
class ScoreComponent;
class PlayerHealthComponent;

enum struct GameState
{
	Start,
	Running,
	GameOver
};

struct PlayerState
{
	Engine::GameObject* pPlayer;
	PlayerHealthComponent* pHealthComp;
	int Lives;

	PlayerState()
	{
		pPlayer = nullptr;
		pHealthComp = nullptr;
		Lives = 3;
		
	}
};

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
	Engine::GameObject* const GetRandomPlayer() const;
private:
	bool IsPlayerEvent(Component* pSender, auto& iterator);
	void CreateStartText();

	void CreateSinglePlayerLoop();
	void CreatePVPPlayerLoop();
	void CreateCo_OpPlayerLoop();

	Engine::Vector2 GetRandomMapLocation() const;

	void EndGame();
	void NextRound();

	void SpawnEnemies(Engine::Scene* const pScene);
	void SpawnPlayer(int index, const Engine::Vector2& pos, Engine::Scene* const pScene);

	Engine::GameObject* m_pMapObject;
	std::vector< PlayerState> m_pPlayers;
	std::forward_list<Engine::GameObject*> m_pSpawnedEnemies;
	ScoreComponent* m_pScoreComponent;

	Engine::TextRenderer* m_pStartText{ nullptr };

	GameState m_GameState;
	GameMode m_Mode;
	
	void* m_pStarGameCommand;

};

