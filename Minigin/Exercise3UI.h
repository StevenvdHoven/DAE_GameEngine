#pragma once
#include "Singleton.h"

class Exercise3UI : public Singleton<Exercise3UI>
{
public:
	void Render() const;
	void Destroy();
	void Init();
};

