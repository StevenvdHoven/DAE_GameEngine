#pragma once
#include "Component.h"
class AutoDestroyComponent : public Engine::Component
{
public:
	AutoDestroyComponent(Engine::GameObject* pOwner);
	AutoDestroyComponent(Engine::GameObject* pOwner, float lifeTime);

	void Start() override;
	void Update() override;

	std::string GetTypeName() const override;

private:
	bool m_CountingDown;
	float m_LifeTime;

};

