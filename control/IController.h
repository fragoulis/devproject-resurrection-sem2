#pragma once

class IController
{
public:
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	virtual void update() = 0;
};
