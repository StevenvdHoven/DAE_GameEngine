#include "ServiceLocator.h"

namespace Engine {
    std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem = nullptr;
}