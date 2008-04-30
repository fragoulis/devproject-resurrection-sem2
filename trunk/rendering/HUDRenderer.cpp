
#include "HUDRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "../GameLogic/GameLogic.h"
#include "../GameLogic/Objects/Playership.h"
#include "../Math/Point2.h"
#include "../Math/Point2.h"
#include "../Control/OSinterface/Input.h"
#include "gl/glu.h"

HUDRenderer::HUDRenderer()// : m_playership(0), m_currentLives(0), m_ebombType(EBOMB_TYPE_UNKNOWN)
{
	Texture *tex = TextureIO::instance()->getTexture("hudLifeIcon.dds");
	m_textureList.push_back(tex);
	//tex = TextureIO::instance()->getTexture("hudBar.dds");
	tex = TextureIO::instance()->getTexture("hudBg.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("hudEnergyBg.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("redBar.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("yellowBar.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("blueBar.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("hudBar2.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("hudBomb.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("ebombCreated.dds");
	m_textureList.push_back(tex);

	//EventManager::instance().registerEventListener<Player_Spawned>(this);
	//EventManager::instance().registerEventListener<Player_Despawned>(this);
	//EventManager::instance().registerEventListener<Level_Unload>(this);
	EventManager::instance().registerEventListener<Ebomb_Created>(this);
	EventManager::instance().registerEventListener<Ebomb_Uncreated>(this);

	m_displayEbombMsg = false;
	m_startEbombMessageTime = 0.0f;
	m_messageDisplayTime = 1.0f;

	m_currentTime = 0.0f;
}

HUDRenderer::~HUDRenderer()
{
	//EventManager::instance().unRegisterEventListener<Player_Spawned>(this);
	//EventManager::instance().unRegisterEventListener<Player_Despawned>(this);
	//EventManager::instance().unRegisterEventListener<Level_Unload>(this);
}



void HUDRenderer :: render(Graphics& g) const
{
	// draw energy bars
	// draw number of lives
	// draw type of e-bomb. You could listen to events for e-bomb created/uncreated
	// but asking GameLogic about it is just as easy :)
	// if type is unknown, no ebomb exists
	

	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int screenWidth = viewPortDims[2];
	int screenHeight = viewPortDims[3];

	const Playership* playership = GameLogic::instance().getPlayership();
	int currentLives = GameLogic::instance().getCurrentLives();
	EbombType ebombType = GameLogic::instance().getCurrentEbombType();


	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	//here goes all the 2D rendering
	glPushMatrix();
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderManager::instance()->begin("hudShader");
		m_textureList[1]->bind(0);
		CHECK_GL_ERROR();
		ShaderManager::instance()->setUniform1i("tex",0);
		CHECK_GL_ERROR();
		const GLfloat transparency = 0.6f;
		ShaderManager::instance()->setUniform1fv("transparency", &transparency);
		CHECK_GL_ERROR();
		const GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
		ShaderManager::instance()->setUniform4fv("constantColor", white);
		CHECK_GL_ERROR();

		if (playership != NULL) {
			int energyCapacity = playership->getEnergyCapacity();
			int redEnergyAmount = playership->getEnergy(ENERGY_TYPE_RED);
			int yellowEnergyAmount = playership->getEnergy(ENERGY_TYPE_YELLOW);
			int blueEnergyAmount = playership->getEnergy(ENERGY_TYPE_BLUE);

			//DRAW HUD BARS
			//life bar
			RenderEngine::drawTexturedQuad(Vector3(0.0f, 0, 0), Vector3(1024, 0, 0), Vector3(0, 50, 0), Vector2(0,0), Vector2(1,1));

			//RenderEngine::drawTexturedQuad(Vector3(0.0f, 0, 0), Vector3(180.0f, 0, 0), Vector3(0, 50.0f, 0), Vector2(0,0), Vector2(1,1));
			//m_textureList[2]->bind(0);
			//energy bar
			//RenderEngine::drawTexturedQuad(Vector3((float) (screenWidth/2.0f-(energyCapacity*10)/2)-10, 5, 0), Vector3((float) (energyCapacity*10)+20, 0, 0), Vector3(0, 40.0f, 0), Vector2(0,0), Vector2(1,1));
			//m_textureList[6]->bind(0);
			//RenderEngine::drawTexturedQuad(Vector3(screenWidth - 100.0f, 0, 0), Vector3(100.0f, 0, 0), Vector3(0, 50.0f, 0), Vector2(0,0), Vector2(1,1));

			const GLfloat transparency2 = 1.0f;
			ShaderManager::instance()->setUniform1fv("transparency", &transparency2);
			CHECK_GL_ERROR();

			m_textureList[0]->bind(0);
			//DRAW LIFES INFO
			for (int i = 0; i < currentLives; i++)
				RenderEngine::drawTexturedQuad(Vector3((float) (i*50.0f), 0, 0), Vector3(50.0f, 0, 0), Vector3(0, 50.0f, 0), Vector2(0,0), Vector2(1,1));

			const GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
			const GLfloat yellow[4] = {1.0f,1.0f,0.0f,1.0f};
			const GLfloat blue[4] = {0.0f,0.0f,1.0f,1.0f};
			const GLfloat green[4] = {0.0f,1.0f,0.0f,1.0f};
			const GLfloat orange[4] = {1.0f,0.5f,0.0f,1.0f};
			const GLfloat purple[4] = {1.0f,0.0f,1.0f,1.0f};
			
			//DRAW E-BOMB INFO
			m_textureList[7]->bind(0);
			switch (ebombType) {
				case EBOMB_TYPE_UNKNOWN:
					break;
				case EBOMB_TYPE_RED:
					ShaderManager::instance()->setUniform4fv("constantColor", red);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				case EBOMB_TYPE_YELLOW:
					ShaderManager::instance()->setUniform4fv("constantColor", yellow);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				case EBOMB_TYPE_BLUE:
					ShaderManager::instance()->setUniform4fv("constantColor", blue);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				case EBOMB_TYPE_GREEN:
					ShaderManager::instance()->setUniform4fv("constantColor", green);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				case EBOMB_TYPE_ORANGE:
					ShaderManager::instance()->setUniform4fv("constantColor", orange);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				case EBOMB_TYPE_PURPLE:
					ShaderManager::instance()->setUniform4fv("constantColor", purple);
					CHECK_GL_ERROR();
					RenderEngine::drawTexturedQuad(Vector3(screenWidth-70.0f, 10, 0), Vector3(30.0f, 0, 0), Vector3(0, 30.0f, 0), Vector2(0,0), Vector2(1,1));
					break;
				default:
					break;
			}

			//DRAW EBOMB CREATION MESSAGE IF NEEDED
			if (m_displayEbombMsg) {
				m_textureList[8]->bind(0);
				RenderEngine::drawTexturedQuad(Vector3(screenWidth/2-128.0f, screenHeight/2-32, 0), Vector3(256.0f, 0, 0), Vector3(0, 64.0f, 0), Vector2(0,0), Vector2(1,1));
			}

			//DRAW ENERGY BARS
			ShaderManager::instance()->setUniform4fv("constantColor", white);
			CHECK_GL_ERROR();
			//RED BAR
			m_textureList[3]->bind(0);
			for (int i = 0; i < redEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 30.0f, 0), Vector3(10.0f, 0.0f, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
			//YELLOW BAR
			m_textureList[4]->bind(0);
			for (int i = 0; i < yellowEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 20.0f, 0), Vector3(10.0f, 0, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
			//BLUE BAR
			m_textureList[5]->bind(0);
			for (int i = 0; i < blueEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 10.0f, 0), Vector3(10.0f, 0, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
		}

		glDisable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);

		// Draw the crosshair
		Point2 mousep = Input::instance().getMousePosition();
		RenderEngine::instance().getConstRenderSettings().getCrosshair()->bind();
		RenderEngine::drawTexturedQuad(Vector3(mousep.getX()-25.0f,float(screenHeight) - mousep.getY() - 12.5f,0),Vector3(50,0,0),Vector3(0,25,0), Vector2(0,0), Vector2(1,1));
		glDisable(GL_ALPHA_TEST);

		ShaderManager::instance()->end();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
}

void HUDRenderer :: update(float dt)
{
	//m_playership = GameLogic::instance().getPlayership();
	//m_currentLives = GameLogic::instance().getCurrentLives();
	//m_ebombType = GameLogic::instance().getCurrentEbombType();

	m_currentTime += dt;

	if ((m_currentTime - m_startEbombMessageTime < m_messageDisplayTime) && m_currentTime > 1.0f)
		m_displayEbombMsg = true;
	else
		m_displayEbombMsg = false;
}


//void HUDRenderer :: onEvent(Player_Spawned& evt)
//{
//	m_playership = evt.getValue();
//}
//
//void HUDRenderer :: onEvent(Player_Despawned&)
//{
//	m_playership = 0;
//}
//
//void HUDRenderer :: onEvent(Level_Unload&)
//{
//	m_playership = 0;
//}

void HUDRenderer :: onEvent(Ebomb_Created &ebomb)
{
	m_startEbombMessageTime = m_currentTime;
}

void HUDRenderer :: onEvent(Ebomb_Uncreated &ebomb)
{
	//if the ebomb is uncreated while the creation message is displayed do this hack to not display the message anymore
	m_startEbombMessageTime = m_startEbombMessageTime+m_messageDisplayTime;
}