#pragma once
class GUI
{
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual ~GUI() = default;
};

