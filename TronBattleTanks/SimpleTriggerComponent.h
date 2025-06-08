#pragma once
#include "Component.h"
#include "Observers.h"

class SimpleTriggerComponent : public Engine::Component
{
public:
	SimpleTriggerComponent(Engine::GameObject* pOwner);

	void OnTriggerEnter(Engine::GameObject* other) override;

	Engine::GameObject* GetLastHit() const { return m_LastHit; }
	Engine::Subject& GetOnTrigger() { return *m_OnTrigger; };

	std::string GetTypeName() const override;

private:
	std::unique_ptr<Engine::Subject> m_OnTrigger;
	Engine::GameObject* m_LastHit;


};

