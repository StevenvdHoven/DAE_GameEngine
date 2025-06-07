#pragma once
#include <memory>
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "GraphEditor.h"
#include "PathFinding.h"
#include "ComponentRegistery.h"
#include "LevelEditor.h"

namespace Engine
{
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> m_SoundSystem;
		static std::unique_ptr<PhysicsSystem> m_PhysicsSystem;
		static std::unique_ptr<GraphEditor> m_GraphEditor;
		static std::unique_ptr<LevelEditor> m_LevelEditor;
		static std::unique_ptr<PathFinding> m_PathFinding;
		static std::unique_ptr<ComponentRegistery> m_ComponentRegistery;
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
		static LevelEditor& GetLevelEditor()
		{
			return *m_LevelEditor;
		}
		static PathFinding& GetPathFinding() 
		{
			return *m_PathFinding;
		}

		static ComponentRegistery& GetComponentRegistery()
		{
			return *m_ComponentRegistery;
		}

		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}
		static void RegisterPhysicsSystem(std::unique_ptr<PhysicsSystem>&& physicsSystem)
		{
			m_PhysicsSystem = std::move(physicsSystem);
		}
		static void RegisterLevelEditor(std::unique_ptr<LevelEditor>&& levelEditor)
		{
			m_LevelEditor = std::move(levelEditor);
		}
		static void RegisterGraphEditor(std::unique_ptr<GraphEditor>&& graphEditor)
		{
			m_GraphEditor = std::move(graphEditor);
		}
		static void RegisterPathFindingService(std::unique_ptr<PathFinding>&& pathFinding)
		{
			m_PathFinding = std::move(pathFinding);
		}

		static void RegisterComponentRegistery(std::unique_ptr<ComponentRegistery>&& registery)
		{
			m_ComponentRegistery = std::move(registery);
		}
	};
}

