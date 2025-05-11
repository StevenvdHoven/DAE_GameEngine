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

class GameLoop final : public Engine::Component, public Engine::IObserver
{
public:
	GameLoop(Engine::GameObject* pOwner, ScoreComponent* pScoreComponent,Engine::GameObject* pMapObject);
	virtual ~GameLoop() = default;

	void Start() override;


	// Inherited via IObserver
	void OnNotify(Component* sender) override;
private:
	void HandleScoreInfo();

	Engine::GameObject* m_pMapObject;
	ScoreComponent* m_pScoreComponent;

};

