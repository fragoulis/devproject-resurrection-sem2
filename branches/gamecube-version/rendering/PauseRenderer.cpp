#include "PauseRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "../Menu/PausePage.h"
#include "../Menu/InstructionPage.h"
#include "gl/glu.h"
#include <gl/glee.h>

PauseRenderer::PauseRenderer()
{
	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int screenWidth = viewPortDims[2];
	int screenHeight = viewPortDims[3];


	m_currentMenu = new PausePage();
	m_currentMenu->init(screenWidth, screenHeight);

	m_state = MENU_STATE_PAUSE;

	m_menuPool[MENU_STATE_PAUSE] = new PausePage();
	m_menuPool[MENU_STATE_INSTRUCTIONS] = new InstructionPage();

	for (int i = 0; i < NUM_MENU_STATES; i++) 
		m_menuPool[i]->init(screenWidth, screenHeight);

	m_currentMenu = m_menuPool[(int) m_state];
}

PauseRenderer::~PauseRenderer()
{
	delete m_currentMenu;
}

void PauseRenderer :: render(Graphics& g) const
{
	m_currentMenu->render(g);
}

void PauseRenderer :: update(float dt)
{
	m_currentMenu = m_menuPool[(int) m_state];

	m_currentMenu->update(dt);
}
