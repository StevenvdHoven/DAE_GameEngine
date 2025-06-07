#include "ServiceLocator.h"

namespace Engine {
    std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem = nullptr;
	std::unique_ptr<PhysicsSystem> ServiceLocator::m_PhysicsSystem = nullptr;
	std::unique_ptr<GraphEditor> ServiceLocator::m_GraphEditor = nullptr;
	std::unique_ptr<LevelEditor> ServiceLocator::m_LevelEditor = nullptr;
	std::unique_ptr<PathFinding> ServiceLocator::m_PathFinding = nullptr;
	std::unique_ptr<ComponentRegistery> ServiceLocator::m_ComponentRegistery = nullptr;
}