#pragma once
class Graphics;


class Application
{
public:
	Application();
	~Application();

	//! Initializes the application. Should be called before the first update
	bool init(); // returns false on failure

	//! Frees memory. Should be called after the last update
	void destroy();

	//! Renders stuffz!
	void render(Graphics& g) const;

	//! Updates stuffz!
	void update();

	//! Tells the application to close
	void quit() { m_wantToQuit = true; }
	bool wantToQuit() { return m_wantToQuit; }

private:
	bool m_wantToQuit;
};
