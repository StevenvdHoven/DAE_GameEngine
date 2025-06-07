#include "InputUnbinder.h"
#include "InputManager.h"

InputUnbinder::InputUnbinder(Engine::GameObject* pOwner):
	InputUnbinder{pOwner,0,std::vector<void*>{}}
{
}

InputUnbinder::InputUnbinder(Engine::GameObject* pOwner, int playerIndex, std::vector<void*>&& pCommands):
	Component{pOwner},
	m_PlayerIndex{playerIndex},
	m_pCommands{std::move(pCommands)}
	
{
}

InputUnbinder::~InputUnbinder()
{
	auto& inputInstance{ Engine::InputManager::GetInstance() };
	std::for_each(m_pCommands.begin(), m_pCommands.end(), [&inputInstance,this](void* pCommand)
		{
			inputInstance.Unbind(m_PlayerIndex, pCommand);
		});
}

std::string InputUnbinder::GetTypeName() const
{
	return "InputUnbinder";
}
