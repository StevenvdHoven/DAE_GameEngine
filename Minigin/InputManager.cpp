#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <vector>
#include "Vector2.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>         
#include <Xinput.h>          

#pragma comment(lib, "xinput.lib")  

#pragma region PIMPL

class PlayerController
{
	static constexpr int NUM_KEYS = SDL_NUM_SCANCODES;
public:
	PlayerController(int index) :
		Index{ index },
		CurrentState{},
		PreviousState{},
		ButtonsPressedThisFrame{ 0 },
		ButtonsReleasedThisFrame{ 0 }
	{
		ZeroMemory(m_PreviousKeyboardState, NUM_KEYS);
		ZeroMemory(m_CurrentKeyboardState, NUM_KEYS);
	};
	
	~PlayerController()
	{
		m_ButtonBindings.clear();
		m_ValueBindings.clear();
		m_2DValueBindings.clear();
	}

	void Update(const Uint8* keyboardState)
	{
		CopyMemory(&PreviousState, &CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));

		auto result{ XInputGetState(Index, &CurrentState) };

		if (result)
		{
			auto buttonChanges{ CurrentState.Gamepad.wButtons ^ PreviousState.Gamepad.wButtons };
			ButtonsPressedThisFrame = buttonChanges & CurrentState.Gamepad.wButtons;
			ButtonsReleasedThisFrame = buttonChanges & (~CurrentState.Gamepad.wButtons);
		}

		CopyMemory(m_PreviousKeyboardState, m_CurrentKeyboardState, NUM_KEYS);
		CopyMemory(m_CurrentKeyboardState, keyboardState, NUM_KEYS);
	}

	bool Is_Pressed(unsigned int button) const { return CurrentState.Gamepad.wButtons & button; }
	bool Is_Released(unsigned int button) const { return ButtonsReleasedThisFrame & button; }
	bool Is_Down(unsigned int button) const { return ButtonsPressedThisFrame & button; }

	bool Is_KeyPressed(unsigned int key) const { return !m_PreviousKeyboardState[key] && m_CurrentKeyboardState[key];}
	bool Is_KeyReleased(unsigned int key) const { return m_PreviousKeyboardState[key] && !m_CurrentKeyboardState[key]; }
	bool Is_KeyDown(unsigned int key) const { return m_CurrentKeyboardState[key]; }

	void BindButton(int button, Engine::Command* pCommand)
	{
		Engine::PlayerBindingButton binding;
		binding.InputMask = button;
		binding.pCommand = pCommand;
		m_ButtonBindings.emplace_back(binding);
	}

	void BindValue(int button, Engine::ValueCommand<float>* pCommand)
	{
		Engine::PlayerBindingValue binding;
		binding.InputMask = button;
		binding.pCommand = pCommand;
		m_ValueBindings.emplace_back(binding);
	}

	void Bind2DValue(Engine::ValueCommand<Engine::Vector2>* pCommand)
	{
		Engine::PlayerBinding2DValue binding;
		binding.pCommand = pCommand;
		m_2DValueBindings.emplace_back(binding);
	}

	void CheckBindings()
	{
		for (auto binding : m_ButtonBindings)
		{
			bool pressed{ binding.pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_Pressed(binding.InputMask) : Is_KeyPressed(binding.InputMask) };
			bool released{ binding.pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_Released(binding.InputMask) : Is_KeyReleased(binding.InputMask) };

			if (pressed)
			{
				binding.pCommand->Execute();
			}
			else if (released)
			{
				binding.pCommand->Execute();
			}
		}
	}

	void CheckValueBindings()
	{
		for (auto binding : m_ValueBindings)
		{
			if (Is_Pressed(binding.InputMask))
			{
				binding.pCommand->Execute(1.0f);
			}
			else if (Is_Released(binding.InputMask))
			{
				binding.pCommand->Execute(0.0f);
			}
		}
	}

	void Check2DValueBindings()
	{
		for (auto& binding : m_2DValueBindings)
		{
			auto type = binding.pCommand->GetInputType();
			Engine::Vector2 value{};
			bool pressed = false, released = false;

			if (type == Engine::GameActorCommand2D::InputType2D::WASD ||
				type == Engine::GameActorCommand2D::InputType2D::ARROW_KEYS)
			{
				pressed = Is_KeyboardPressed(type);
				released = Is_KeyboardReleased(type);
				value = (type == Engine::GameActorCommand2D::InputType2D::WASD) ? GetWASD() : GetArrows();
			}
			else
			{
				int mask = Get2DBindingMask(type);
				pressed = Is_Pressed(mask);
				released = Is_Released(mask);

				switch (type)
				{
				case Engine::GameActorCommand2D::InputType2D::LEFT_STICK: value = GetLeftStick(); break;
				case Engine::GameActorCommand2D::InputType2D::RIGHT_STICK: value = GetRightStick(); break;
				case Engine::GameActorCommand2D::InputType2D::D_PAD: value = GetDPad(); break;
				}
			}

			if (pressed)
				binding.pCommand->Execute(value);
			else if (released)
				binding.pCommand->Execute(Engine::Vector2{}); // Stop movement
		}
	}

	int Index;

private:
	Engine::Vector2 GetLeftStick() const
	{
		Engine::Vector2 stick;
		stick.x = CurrentState.Gamepad.sThumbLX / 32767.0f;
		stick.y = CurrentState.Gamepad.sThumbLY / 32767.0f;
		return stick;
	}

	Engine::Vector2 GetRightStick() const
	{
		Engine::Vector2 stick{};
		stick.x = CurrentState.Gamepad.sThumbRX / 32767.0f;
		stick.y = CurrentState.Gamepad.sThumbRY / 32767.0f;
		return stick;
	}

	Engine::Vector2 GetDPad() const
	{
		float x{ 0 };
		float y{ 0 };

		if (Is_Pressed(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			x += -1.f;
		}
		else if (Is_Pressed(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			x += 1.f;
		}

		if (Is_Pressed(XINPUT_GAMEPAD_DPAD_UP))
		{
			y += -1.f;
		}
		else if (Is_Pressed(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			y += 1.f;
		}

		return Engine::Vector2{ x, y };
	}

	Engine::Vector2 GetWASD() const
	{
		Engine::Vector2 value{ 0, 0 };

		if (Is_KeyDown(SDL_SCANCODE_W)) value.y -= 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_S)) value.y += 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_A)) value.x -= 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_D)) value.x += 1.0f;

		return value;
	}

	Engine::Vector2 GetArrows() const
	{
		Engine::Vector2 value{ 0, 0 };

		if (Is_KeyDown(SDL_SCANCODE_UP)) value.y -= 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_DOWN)) value.y += 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_LEFT)) value.x -= 1.0f;
		if (Is_KeyDown(SDL_SCANCODE_RIGHT)) value.x += 1.0f;

		return value;
	}

	int Get2DBindingMask(Engine::GameActorCommand2D::InputType2D inputType)
	{
		switch (inputType)
		{
		case Engine::GameActorCommand2D::InputType2D::LEFT_STICK:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case Engine::GameActorCommand2D::InputType2D::RIGHT_STICK:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case Engine::GameActorCommand2D::InputType2D::D_PAD:
			return XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN |
				XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT;
		default:
			return 0;
		}
	}

	bool Is_KeyboardPressed(Engine::GameActorCommand2D::InputType2D type)
	{
		switch (type)
		{
		case Engine::GameActorCommand2D::InputType2D::WASD:
			return Is_KeyPressed(SDL_SCANCODE_W) || Is_KeyPressed(SDL_SCANCODE_A) ||
				Is_KeyPressed(SDL_SCANCODE_S) || Is_KeyPressed(SDL_SCANCODE_D);
		case Engine::GameActorCommand2D::InputType2D::ARROW_KEYS:
			return Is_KeyPressed(SDL_SCANCODE_UP) || Is_KeyPressed(SDL_SCANCODE_DOWN) ||
				Is_KeyPressed(SDL_SCANCODE_LEFT) || Is_KeyPressed(SDL_SCANCODE_RIGHT);
		default:
			return false;
		}
	}

	bool Is_KeyboardReleased(Engine::GameActorCommand2D::InputType2D type)
	{
		switch (type)
		{
		case Engine::GameActorCommand2D::InputType2D::WASD:
			return Is_KeyReleased(SDL_SCANCODE_W) || Is_KeyReleased(SDL_SCANCODE_A) ||
				Is_KeyReleased(SDL_SCANCODE_S) || Is_KeyReleased(SDL_SCANCODE_D);
		case Engine::GameActorCommand2D::InputType2D::ARROW_KEYS:
			return Is_KeyReleased(SDL_SCANCODE_UP) || Is_KeyReleased(SDL_SCANCODE_DOWN) ||
				Is_KeyReleased(SDL_SCANCODE_LEFT) || Is_KeyReleased(SDL_SCANCODE_RIGHT);
		default:
			return false;
		}
	}

	Engine::Vector2 Get2DValue(Engine::GameActorCommand2D::InputType2D inputType)
	{
		switch (inputType)
		{
		case Engine::GameActorCommand2D::InputType2D::LEFT_STICK:
			return GetLeftStick();
		case Engine::GameActorCommand2D::InputType2D::RIGHT_STICK:
			return GetRightStick();
		case Engine::GameActorCommand2D::InputType2D::D_PAD:
			return GetDPad();
		case Engine::GameActorCommand2D::InputType2D::WASD:
			return GetWASD();
		case Engine::GameActorCommand2D::InputType2D::ARROW_KEYS:
			return GetArrows();
		default:
			return Engine::Vector2{};
		}
	}

	XINPUT_STATE PreviousState;
	XINPUT_STATE CurrentState;

	int ButtonsPressedThisFrame;
	int ButtonsReleasedThisFrame;

	Uint8 m_PreviousKeyboardState[NUM_KEYS];
	Uint8 m_CurrentKeyboardState[NUM_KEYS];

	std::vector<Engine::PlayerBindingButton> m_ButtonBindings;
	std::vector<Engine::PlayerBindingValue> m_ValueBindings;
	std::vector<Engine::PlayerBinding2DValue> m_2DValueBindings;
};

class Engine::InputManager::InputManagerImpl
{
public:
	InputManagerImpl()
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			m_Controllers.emplace_back(i);
		}
	}
	~InputManagerImpl()
	{
		m_Controllers.clear();
	}

	void Update();

	void BindButton(int playerIndex, int button, Command* pCommand);
	void BindValue(int playerIndex, int button, ValueCommand<float>* pCommand);
	void Bind2DValue(int playerIndex, ValueCommand<Vector2>* pCommand);

private:
	std::vector<PlayerController> m_Controllers;
};

void Engine::InputManager::InputManagerImpl::Update()
{
	auto state{ SDL_GetKeyboardState(nullptr) };
	
	for (auto playerController : m_Controllers)
	{
		playerController.Update(state);
		playerController.CheckBindings();
		playerController.CheckValueBindings();
		playerController.Check2DValueBindings();
	}
}

void Engine::InputManager::InputManagerImpl::BindButton(int playerIndex, int button, Command* pCommand)
{
	m_Controllers[playerIndex].BindButton(button, pCommand);
}

void Engine::InputManager::InputManagerImpl::BindValue(int playerIndex, int button, ValueCommand<float>* pCommand)
{
	m_Controllers[playerIndex].BindValue(button, pCommand);
}

void Engine::InputManager::InputManagerImpl::Bind2DValue(int playerIndex, ValueCommand<Vector2>* pCommand)
{
	m_Controllers[playerIndex].Bind2DValue(pCommand);
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

Engine::InputManager::InputManager():
	m_pImpl{ new InputManagerImpl() }
{
}

Engine::InputManager::~InputManager()
{
	delete m_pImpl;
}

bool Engine::InputManager::ProcessInput()
{
	m_pImpl->Update();
	return ProcessKeyboard();
}

void Engine::InputManager::BindButton(int playerIndex, int button, Command* pCommand)
{
	m_pImpl->BindButton(playerIndex, button, pCommand);
}

void Engine::InputManager::BindValue(int playerIndex, int button, ValueCommand<float>* pCommand)
{
	m_pImpl->BindValue(playerIndex, button, pCommand);
}

void Engine::InputManager::Bind2DValue(int playerIndex, ValueCommand<Vector2>* pCommand)
{
	m_pImpl->Bind2DValue(playerIndex, pCommand);
}


