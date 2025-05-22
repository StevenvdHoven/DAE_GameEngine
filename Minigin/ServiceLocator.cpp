#include "ServiceLocator.h"

namespace Engine {
    std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem = nullptr;
	std::unique_ptr<PhysicsSystem> ServiceLocator::m_PhysicsSystem = nullptr;
	std::unique_ptr<GraphEditor> ServiceLocator::m_GraphEditor = nullptr;
}