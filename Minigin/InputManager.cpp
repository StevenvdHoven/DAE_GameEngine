#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <Xinput.h>
#include <Windows.h>

struct PlayerController
{
	int Index;

	XINPUT_STATE PreviousState;
	XINPUT_STATE CurrentState;

	int ButtonsPressedThisFrame;
	int ButtonsReleasedThisFrame;
};

#pragma region PIMPL

class Engine::InputManager::InputManagerImpl
{
public:
	void ProcessController();

	bool Is_Pressed(unsigned int button) const;
	bool Is_Released(unsigned int button) const;
	bool Is_Down(unsigned int button) const;

private:
	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	int m_ButtonsPressedThisFrame;
	int m_ButtonsReleasedThisFrame;
};

void Engine::InputManager::InputManagerImpl::ProcessController()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Engine::InputManager::InputManagerImpl::Is_Pressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool Engine::InputManager::InputManagerImpl::Is_Released(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool Engine::InputManager::InputManagerImpl::Is_Down(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

#pragma endregion

bool Engine::InputManager::ProcessKeyboard()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		// etc...

		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	return true;
}


bool Engine::InputManager::ProcessInput()
{
	m_pImpl->ProcessController();
	return ProcessKeyboard();
}

bool Engine::InputManager::Is_Pressed(unsigned int button) const
{
	return m_pImpl->Is_Pressed(button);
}

bool Engine::InputManager::Is_Released(unsigned int button) const
{
	return m_pImpl->Is_Released(button);
}

bool Engine::InputManager::Is_Down(unsigned int button) const
{
	return m_pImpl->Is_Down(button);
}


