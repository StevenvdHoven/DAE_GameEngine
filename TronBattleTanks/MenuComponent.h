#pragma once
#include "Component.h"
#include "Vector2.h"
#include "SoundSystem.h"

enum struct GameMode
{
	SinglePlayer,
	CoOp,
	VS
};

class MenuComponent : public Engine::Component
{
public:
	MenuComponent(Engine::GameObject* pGameObject);
	void Start() override;

	void Update() override;

	void SetGameMode(GameMode mode);

	std::string GetTypeName() const override;
private:
	void CreateMenuPlayer();

	Engine::GameObject* m_pMenuPlayer;

	bool m_GameModeSelected;
	float m_TransitionTime;
	Engine::Vector2 m_MenuPlayerStartLocation;
	Engine::Vector2 m_MenuPlayerTargetLocation;
	GameMode m_SelectedGameMode;

	Engine::SoundClip m_IntroSound;

	void* m_MenuMoveCommand;
	void* m_MenuKeyboardMoveCommand;
	void* m_SkipSceneCommand;

};

