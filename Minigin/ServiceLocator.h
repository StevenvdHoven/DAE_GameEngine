#pragma once
#include <memory>
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "GraphEditor.h"

namespace Engine
{
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> m_SoundSystem;
		static std::unique_ptr<PhysicsSystem> m_PhysicsSystem;
		static std::unique_ptr<GraphEditor> m_GraphEditor;
	public:

		static SoundSystem& GetSoundSystem()
		{
			return *m_SoundSystem;
		}
		static PhysicsSystem& GetPhysicsSystem()
		{
			return *m_PhysicsSystem;
		}
		static GraphEditor& GetGraphEditor()
		{
			return *m_GraphEditor;
		}

		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}
		static void RegisterPhysicsSystem(std::unique_ptr<PhysicsSystem>&& physicsSystem)
		{
			m_PhysicsSystem = std::move(physicsSystem);
		}
		static void RegisterGraphEditor(std::unique_ptr<GraphEditor>&& graphEditor)
		{
			m_GraphEditor = std::move(graphEditor);
		}
	};
}

