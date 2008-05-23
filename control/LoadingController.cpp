#include "LoadingController.h"
//#include "../rendering/RenderEngine.h"

#include <iostream>
using namespace std;

static const float TIME_TILL_LOAD = 0.1f;


LoadingController :: LoadingController() :
	m_loader(0),
	m_timeTillWeCanLoad(0.0f)
{
}

LoadingController :: ~LoadingController()
{
	_deleteLoader();
}


void LoadingController :: activate()
{
	cout << "Activating LoadingController" << endl;
	// deactivate all user input, activate loading renderer
	//RenderEngine& re = RenderEngine::safeInstance();
	//re.deactivateAllRenderers();
	//re.activateRenderer("loading");
	m_timeTillWeCanLoad = TIME_TILL_LOAD;
}

void LoadingController :: deactivate()
{
	cout << "Deactivating LoadingController" << endl;
	//RenderEngine& re = RenderEngine::instance();
	//re.deactivateAllRenderers();
	_deleteLoader();
}

void LoadingController :: update(float dt)
{
	m_timeTillWeCanLoad -= dt;
	if (m_timeTillWeCanLoad < 0.0f && m_loader != 0)
	{
		m_loader->call();
		m_timeTillWeCanLoad = 10000.0f;
	}
}

void LoadingController :: _deleteLoader()
{
	if (m_loader != 0) delete m_loader;
	m_loader = 0;
}