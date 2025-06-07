#pragma once
#include "Component.h"
#include <vector>
#include "Command.h"

class InputUnbinder final : public Engine::Component
{
public:
	InputUnbinder(Engine::GameObject* pOwner);
	InputUnbinder(Engine::GameObject* pOwner, int playerIndex, std::vector<void*>&& pCommands);
	virtual ~InputUnbinder();

	std::string GetTypeName() const override;
private:
	int m_PlayerIndex;
	std::vector<void*> m_pCommands;
};

