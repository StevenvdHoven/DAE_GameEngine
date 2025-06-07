#pragma once
#include "Component.h"
#include <memory>
class IComponentFactory
{
public:
	virtual std::unique_ptr<Engine::Component> Create() = 0;
	virtual ~IComponentFactory() = default;
};

