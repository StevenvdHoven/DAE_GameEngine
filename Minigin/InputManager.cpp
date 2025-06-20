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
		for (auto binding : m_ButtonBindings)
			delete binding;

		for (auto binding : m_ValueBindings)
			delete binding;

		for (auto binding : m_2DValueBindings)
			delete binding;

		m_ButtonBindings.clear();
		m_ValueBindings.clear();
		m_2DValueBindings.clear();
	}

	void Update(const Uint8* keyboardState)
	{
		CopyMemory(&PreviousState, &CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));

		XInputGetState(Index, &CurrentState);


		auto buttonChanges{ CurrentState.Gamepad.wButtons ^ PreviousState.Gamepad.wButtons };
		ButtonsPressedThisFrame = buttonChanges & CurrentState.Gamepad.wButtons;
		auto inv{ (~CurrentState.Gamepad.wButtons) };
		ButtonsReleasedThisFrame = buttonChanges & inv;

		CopyMemory(m_PreviousKeyboardState, m_CurrentKeyboardState, NUM_KEYS);
		CopyMemory(m_CurrentKeyboardState, keyboardState, NUM_KEYS);
	}

	bool Is_Pressed(unsigned int button) const
	{
		return ButtonsPressedThisFrame & button;
	}
	bool Is_Released(unsigned int button) const
	{
		return ButtonsReleasedThisFrame & button;
	}
	bool Is_Down(unsigned int button) const
	{
		return CurrentState.Gamepad.wButtons & button;
	}

	bool Is_KeyPressed(unsigned int key) const
	{
		return !m_PreviousKeyboardState[key] && m_CurrentKeyboardState[key];
	}
	bool Is_KeyReleased(unsigned int key) const
	{
		return m_PreviousKeyboardState[key] && !m_CurrentKeyboardState[key];
	}
	bool Is_KeyDown(unsigned int key) const
	{
		return m_CurrentKeyboardState[key];
	}

	void BindButton(int button, Engine::Command* pCommand)
	{
		Engine::PlayerBindingButton* binding{ new Engine::PlayerBindingButton{} };
		binding->InputMask = button;
		binding->Pressed = false;
		binding->pCommand = pCommand;
		m_ButtonBindings.emplace_back(binding);
	}

	void BindValue(int button, Engine::ValueCommand<float>* pCommand)
	{
		Engine::PlayerBindingValue* binding{ new Engine::PlayerBindingValue{} };
		binding->InputMask = button;
		binding->Pressed = false;
		binding->pCommand = pCommand;
		m_ValueBindings.emplace_back(binding);
	}

	void Bind2DValue(Engine::ValueCommand<Engine::Vector2>* pCommand)
	{
		Engine::PlayerBinding2DValue* binding{ new Engine::PlayerBinding2DValue{} };
		binding->Pressed = false;
		binding->pCommand = pCommand;
		m_2DValueBindings.emplace_back(binding);
	}

	void Unbind(void* pCommand)
	{
		auto buttonIT{ std::find_if(m_ButtonBindings.begin(), m_ButtonBindings.end(), [pCommand](Engine::PlayerBindingButton* binding)
			{
				return binding->pCommand == pCommand;
			})};
		if (buttonIT != m_ButtonBindings.end()) {
			m_ButtonBindings.erase(buttonIT);
			return;
		}

		auto valueIT{ std::find_if(m_ValueBindings.begin(), m_ValueBindings.end(), [pCommand](Engine::PlayerBindingValue* binding)
			{
				return binding->pCommand == pCommand;
			}) };
		if (valueIT != m_ValueBindings.end())
		{
			m_ValueBindings.erase(valueIT);
			return;
		}

		auto value2dIT{ std::find_if(m_2DValueBindings.begin(), m_2DValueBindings.end(), [pCommand](Engine::PlayerBinding2DValue* binding)
			{
				return binding->pCommand == pCommand;
			}) };
		if (value2dIT != m_2DValueBindings.end())
		{
			m_2DValueBindings.erase(value2dIT);
			return;
		}
	}

	void CheckBindings()
	{
		auto copyBindings{ m_ButtonBindings };
		for (auto& binding : copyBindings)
		{
			const auto triggerState{ binding->pCommand->GetTriggerState() };

			bool pressed{ binding->pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_Pressed(binding->InputMask) : Is_KeyPressed(binding->InputMask) };
			bool released{ binding->pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_Released(binding->InputMask) : Is_KeyReleased(binding->InputMask) };

			if (pressed && triggerState == Engine::TriggerState::PRESSED)
			{
				binding->pCommand->Execute();
			}
			if (released && triggerState == Engine::TriggerState::RELEASED)
			{
				binding->pCommand->Execute();
			}
		}
	}

	void CheckValueBindings()
	{
		for (auto& binding : m_ValueBindings)
		{
			const auto triggerState{ binding->pCommand->GetTriggerState() };

			const bool right{ binding->pCommand->GetInputType() == Engine::ValueCommand<float>::InputType::RIGHT_TRIGGER };
			const bool pressed{ binding->pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_TriggerPressed(right) : Is_KeyPressed(binding->InputMask)};
			const bool down{ binding->pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_TriggerDown(right) : Is_KeyDown(binding->InputMask) };
			const bool released{ binding->pCommand->GetDeviceType() == Engine::DeviceType::GAMEPAD ? Is_TriggerReleased(right) : Is_KeyReleased(binding->InputMask)};


			if (pressed && triggerState == Engine::TriggerState::PRESSED)
			{
				if (binding->pCommand->GetDeviceType() == Engine::DeviceType::KEYBOARD)
				{
					binding->pCommand->Execute(1);
				}
				else 
				{
					float value{ static_cast<float>(right ? CurrentState.Gamepad.bRightTrigger : CurrentState.Gamepad.bLeftTrigger) };
					binding->pCommand->Execute(value);
				}
				
			}
			if (down && triggerState == Engine::TriggerState::HELD)
			{
				if (binding->pCommand->GetDeviceType() == Engine::DeviceType::KEYBOARD)
				{
					binding->pCommand->Execute(1);
				}
				else
				{
					float value{ static_cast<float>(right ? CurrentState.Gamepad.bRightTrigger : CurrentState.Gamepad.bLeftTrigger) };
					binding->pCommand->Execute(value);
				}
			}

			if (released && triggerState == Engine::TriggerState::RELEASED)
			{
				if (binding->pCommand->GetDeviceType() == Engine::DeviceType::KEYBOARD)
				{
					binding->pCommand->Execute(0);
				}
				else
				{
					float value{ static_cast<float>(right ? CurrentState.Gamepad.bRightTrigger : CurrentState.Gamepad.bLeftTrigger) };
					binding->pCommand->Execute(value);
				}
			}
		}

	}

	void Check2DValueBindings()
	{
		for (auto& binding : m_2DValueBindings)
		{
			auto triggerState{ binding->pCommand->GetTriggerState() };
			auto type = binding->pCommand->GetInputType();
			Engine::Vector2 value{};
			bool pressed = false;
			bool down = false;
			bool released = false;

			if (type == Engine::GameActorCommand2D::InputType::WASD ||
				type == Engine::GameActorCommand2D::InputType::ARROW_KEYS)
			{
				pressed = Is_KeyboardPressed(type);
				down = Is_KeyboardDown(type);
				released = Is_KeyboardReleased(type);
				value = (type == Engine::GameActorCommand2D::InputType::WASD) ? GetWASD() : GetArrows();
			}
			else
			{
				int mask = Get2DBindingMask(type);
				pressed = Is_Pressed(mask);
				down = Is_Down(mask);
				released = Is_Released(mask);

				
				switch (type)
				{
				case Engine::GameActorCommand2D::InputType::LEFT_STICK: value = GetLeftStick(); break;
				case Engine::GameActorCommand2D::InputType::RIGHT_STICK: value = GetRightStick(); break;
				case Engine::GameActorCommand2D::InputType::D_PAD: value = GetDPad(); break;
				}
			}

			if (pressed && triggerState == Engine::TriggerState::PRESSED)
				binding->pCommand->Execute(value);
			if (down && triggerState == Engine::TriggerState::HELD)
				binding->pCommand->Execute(value);
			if (released && triggerState == Engine::TriggerState::RELEASED)
				binding->pCommand->Execute(Engine::Vector2{});
			if (triggerState == Engine::TriggerState::CONSTANT)
				binding->pCommand->Execute(value);
		}
	}

	int Index;

private:
	Engine::Vector2 GetLeftStick() const
	{
		Engine::Vector2 stick;
		stick.x = CurrentState.Gamepad.sThumbLX / 32767.0f;
		stick.y = CurrentState.Gamepad.sThumbLY / 32767.0f;
		return stick * 1.1f;
	}

	Engine::Vector2 GetRightStick() const
	{
		Engine::Vector2 stick{};
		stick.x = CurrentState.Gamepad.sThumbRX / 32767.0f;
		stick.y = CurrentState.Gamepad.sThumbRY / 32767.0f;
		return stick * 1.1f;
	}

	Engine::Vector2 GetDPad() const
	{
		float x{ 0 };
		float y{ 0 };

		if (Is_Down(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			x += -1.f;
		}
		else if (Is_Down(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			x += 1.f;
		}

		if (Is_Down(XINPUT_GAMEPAD_DPAD_UP))
		{
			y += -1.f;
		}
		else if (Is_Down(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			y += 1.f;
		}

		return Engine::Vector2{ x, y } *1.1f;
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

	int Get2DBindingMask(Engine::GameActorCommand2D::InputType inputType)
	{
		switch (inputType)
		{
		case Engine::GameActorCommand2D::InputType::LEFT_STICK:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case Engine::GameActorCommand2D::InputType::RIGHT_STICK:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case Engine::GameActorCommand2D::InputType::D_PAD:
			return XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN |
				XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT;
		default:
			return 0;
		}
	}

	bool Is_TriggerPressed(bool right) 
	{
		if (right)
		{
			return CurrentState.Gamepad.bRightTrigger >  0 && PreviousState.Gamepad.bRightTrigger == 0;
		}
		else
		{
			return CurrentState.Gamepad.bLeftTrigger > 0 && PreviousState.Gamepad.bLeftTrigger == 0;
		}
	}

	bool Is_TriggerDown(bool right)
	{
		if (right)
		{
			return CurrentState.Gamepad.bRightTrigger > 0;
		}
		else
		{
			return CurrentState.Gamepad.bLeftTrigger > 0;
		}
	}

	bool Is_TriggerReleased(bool right)
	{
		if (right)
		{
			return CurrentState.Gamepad.bRightTrigger <= 0 && PreviousState.Gamepad.bRightTrigger != 0;
		}
		else
		{
			return CurrentState.Gamepad.bLeftTrigger <= 0 && PreviousState.Gamepad.bLeftTrigger != 0;
		}
	}

	bool Is_KeyboardPressed(Engine::GameActorCommand2D::InputType type)
	{
		switch (type)
		{
		case Engine::GameActorCommand2D::InputType::WASD:
			return Is_KeyPressed(SDL_SCANCODE_W) || Is_KeyPressed(SDL_SCANCODE_A) ||
				Is_KeyPressed(SDL_SCANCODE_S) || Is_KeyPressed(SDL_SCANCODE_D);
		case Engine::GameActorCommand2D::InputType::ARROW_KEYS:
			return Is_KeyPressed(SDL_SCANCODE_UP) || Is_KeyPressed(SDL_SCANCODE_DOWN) ||
				Is_KeyPressed(SDL_SCANCODE_LEFT) || Is_KeyPressed(SDL_SCANCODE_RIGHT);
		default:
			return false;
		}
	}

	bool Is_KeyboardDown(Engine::GameActorCommand2D::InputType type)
	{
		switch (type)
		{
		case Engine::GameActorCommand2D::InputType::WASD:
			return Is_KeyDown(SDL_SCANCODE_W) || Is_KeyDown(SDL_SCANCODE_A) ||
				Is_KeyDown(SDL_SCANCODE_S) || Is_KeyDown(SDL_SCANCODE_D);
		case Engine::GameActorCommand2D::InputType::ARROW_KEYS:
			return Is_KeyDown(SDL_SCANCODE_UP) || Is_KeyDown(SDL_SCANCODE_DOWN) ||
				Is_KeyDown(SDL_SCANCODE_LEFT) || Is_KeyDown(SDL_SCANCODE_RIGHT);
		default:
			return false;
		}
	}

	bool Is_KeyboardReleased(Engine::GameActorCommand2D::InputType type)
	{
		switch (type)
		{
		case Engine::GameActorCommand2D::InputType::WASD:
			return Is_KeyReleased(SDL_SCANCODE_W) || Is_KeyReleased(SDL_SCANCODE_A) ||
				Is_KeyReleased(SDL_SCANCODE_S) || Is_KeyReleased(SDL_SCANCODE_D);
		case Engine::GameActorCommand2D::InputType::ARROW_KEYS:
			return Is_KeyReleased(SDL_SCANCODE_UP) || Is_KeyReleased(SDL_SCANCODE_DOWN) ||
				Is_KeyReleased(SDL_SCANCODE_LEFT) || Is_KeyReleased(SDL_SCANCODE_RIGHT);
		default:
			return false;
		}
	}

	Engine::Vector2 Get2DValue(Engine::GameActorCommand2D::InputType inputType)
	{
		switch (inputType)
		{
		case Engine::GameActorCommand2D::InputType::LEFT_STICK:
			return GetLeftStick();
		case Engine::GameActorCommand2D::InputType::RIGHT_STICK:
			return GetRightStick();
		case Engine::GameActorCommand2D::InputType::D_PAD:
			return GetDPad();
		case Engine::GameActorCommand2D::InputType::WASD:
			return GetWASD();
		case Engine::GameActorCommand2D::InputType::ARROW_KEYS:
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

	std::vector<Engine::PlayerBindingButton*> m_ButtonBindings;
	std::vector<Engine::PlayerBindingValue*> m_ValueBindings;
	std::vector<Engine::PlayerBinding2DValue*> m_2DValueBindings;


};

class Engine::InputManager::InputManagerImpl
{
public:
	InputManagerImpl()
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			m_Controllers.emplace_back(std::move(std::make_unique<PlayerController>(i)));
		}
	}
	~InputManagerImpl()
	{
		m_Controllers.clear();
	}

	void Update();

	void BindButton(int playerIndex, int button, std::unique_ptr<Command> pCommand);
	void BindValue(int playerIndex, int button, std::unique_ptr <ValueCommand<float>> pCommand);
	void Bind2DValue(int playerIndex, std::unique_ptr <ValueCommand<Vector2>> pCommand);

	void Unbind(int playerIndex, void* pCommand);

private:
	std::vector<std::unique_ptr<PlayerController>> m_Controllers;

	std::vector<std::unique_ptr<Engine::Command>> m_Commands;
	std::vector<std::unique_ptr<Engine::ValueCommand<float>>> m_ValueCommands;
	std::vector<std::unique_ptr<Engine::ValueCommand<Engine::Vector2>>> m_2DValueCommands;
};

void Engine::InputManager::InputManagerImpl::Update()
{
	auto state{ SDL_GetKeyboardState(nullptr) };

	for (auto& playerController : m_Controllers)
	{
		playerController->Update(state);
		playerController->CheckBindings();
		playerController->CheckValueBindings();
		playerController->Check2DValueBindings();
	}
}

void Engine::InputManager::InputManagerImpl::BindButton(int playerIndex, int button, std::unique_ptr<Command> pCommand)
{
	m_Controllers[playerIndex]->BindButton(button, pCommand.get());
	m_Commands.emplace_back(std::move(pCommand));
}

void Engine::InputManager::InputManagerImpl::BindValue(int playerIndex, int button, std::unique_ptr <ValueCommand<float>> pCommand)
{
	m_Controllers[playerIndex]->BindValue(button, pCommand.get());
	m_ValueCommands.emplace_back(std::move(pCommand));
}

void Engine::InputManager::InputManagerImpl::Bind2DValue(int playerIndex, std::unique_ptr <ValueCommand<Vector2>> pCommand)
{
	m_Controllers[playerIndex]->Bind2DValue(pCommand.get());
	m_2DValueCommands.emplace_back(std::move(pCommand));
}

void Engine::InputManager::InputManagerImpl::Unbind(int playerIndex, void* pCommand)
{
	const int length{ static_cast<int>(m_Controllers.size()) };
	if (length <= playerIndex)
	{
		return; // Invalid player index
	}
	m_Controllers[playerIndex]->Unbind(pCommand);
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

Engine::InputManager::InputManager() :
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

void Engine::InputManager::BindButton(int playerIndex, int button, std::unique_ptr<Command> pCommand)
{
	m_pImpl->BindButton(playerIndex, button, std::move(pCommand));
}

void Engine::InputManager::BindValue(int playerIndex, int button, std::unique_ptr<ValueCommand<float>> pCommand)
{
	m_pImpl->BindValue(playerIndex, button, std::move(pCommand));
}

void Engine::InputManager::Bind2DValue(int playerIndex, std::unique_ptr<ValueCommand<Vector2>> pCommand)
{
	m_pImpl->Bind2DValue(playerIndex, std::move(pCommand));
}

void Engine::InputManager::Unbind(int playerIndex, void* pCommand)
{
	m_pImpl->Unbind(playerIndex, pCommand);
}


