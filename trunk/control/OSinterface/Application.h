#pragma once
class Graphics;


class Application
{
public:
	Application();
	~Application();

	//! Initializes the application. Should be called before the first update
	bool init(); // returns false on failure

	//! Frees memory. Should be called after the last update.
	void destroy();

	//! Renders stuffz!
	void render(Graphics& g) const;

	//! Updates stuffz!
	void update(float dt);
};
