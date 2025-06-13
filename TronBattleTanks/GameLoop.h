#pragma once
#include "Component.h"
#include "Observers.h"
#include <vector>
#include <forward_list>
#include "StartGameCommand.h"
#include "SoundSystem.h"
//GetRandomEnemyType#include "EnemyBrain.h"
#include <map>

namespace Engine
{
	class TextRenderer;
	class Scene;
	class Graph;
	struct Vector2;
	struct SoundClip;
}

enum struct GameMode;
enum struct EnemyType;
class ScoreComponent;
class PlayerHealthComponent;
class SimpleTriggerComponent;

enum struct GameState
{
	Start,
	Running,
	GameOver
};

struct PlayerState
{
	int Index;
	Engine::GameObject* pPlayer;
	PlayerHealthComponent* pHealthComp;
	Engine::TextRenderer* pTextComp;
	int Lives;

	PlayerState()
	{
		Index = 0;
		pPlayer = nullptr;
		pHealthComp = nullptr;
		pTextComp = nullptr;
		Lives = 3;
		
	}
};

class GameLoop final : public Engine::Component, public Engine::IObserver
{
public:
	GameLoop(Engine::GameObject* pOwner,GameMode mode, ScoreComponent* pScoreComponent);
	virtual ~GameLoop();

	void Start() override;
	void Update() override;

	void BeginGame();

	void OnNotify(Component* sender) override;

	const GameState& GetGameState() const { return m_GameState; }
	Engine::GameObject* const GetRandomPlayer() const;

	std::string GetTypeName() const override;
private:
	bool IsPlayerEvent(Component* pSender, auto& iterator);
	bool IsAllPlayersDead();

	void UpdateEnemies();

	void SpawnMaps();
	void CreateStartText();
	void CreateLivesText();

	void CreateSinglePlayerLoop();
	void CreatePVPPlayerLoop();
	void CreateCo_OpPlayerLoop();

	Engine::Vector2 GetRandomMapLocation() const;

	void EndGame();
	void NextRound();

	void SetInstruction(const std::string& instruction);
	void UpdateInstructionText();

	void NextMap();
	void SpawnEnemies(Engine::Scene* const pScene);
	void SpawnPlayer(int index, const Engine::Vector2& pos, Engine::Scene* const pScene);

	bool IsGameOver();

	int GetAmountOfEnemies() const;
	EnemyType GetRandomEnemyType() const;

	int m_RoundCount;
	ScoreComponent* m_pScoreComponent;
	SimpleTriggerComponent* m_CristalTrigger;

	bool m_SwitchMap;
	float m_SwitchDelay;
	int m_CurrentMapIndex;
	std::vector<Engine::GameObject*> m_pMapObjects;
	std::map<int, std::pair<Engine::GameObject*, Engine::GameObject*>> m_pPlayerSpawns;
	std::vector<Engine::Graph*> m_pGraphs;
	std::vector< PlayerState> m_pPlayers;
	std::vector<Engine::TextRenderer*> m_pLivesTexts;
	std::forward_list<Engine::GameObject*> m_pSpawnedEnemies;

	Engine::TextRenderer* m_pStartText{ nullptr };

	std::string m_InstructionText;
	std::string m_TargetInstructionText;
	int m_InstructionTextIndex{ 0 };
	float m_InstructionTextTimer{ 0.0f };
	Engine::TextRenderer* m_pInstructionText{ nullptr };

	Engine::MusicClip m_GameMusic;
	Engine::SoundClip m_CrystalClip;
	Engine::SoundClip m_TypingTickClip;
	GameState m_GameState;
	GameMode m_Mode;
	
	void* m_pStartGameCommand;
	void* m_pKeyboardStartCommand;
	void* m_pSkipSceneCommand;

};

