#include "SoundSystemCommands.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

using namespace Engine;

void Engine::MuteSoundCommand::Execute()
{
	if (ServiceLocator::GetSoundSystem().Muted())
	{
		ServiceLocator::GetSoundSystem().UnMute();
	}
	else
	{
		ServiceLocator::GetSoundSystem().Mute();
	}
}
