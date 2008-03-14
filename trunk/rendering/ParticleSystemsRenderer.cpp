//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"
#include "../GameLogic/GameLogic.h"
#include "../GameLogic/Enemies/Enemyship.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{

	EventManager::instance().registerEventListener<Key_GoingDown>(this);
	EventManager::instance().registerEventListener<Enemy_Despawned>(this);

	
}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{
	// FIXME : Deleting the hacky way the dummy ps
	//delete m_psList[0];
	//m_psList.clear();

	std::vector<PS_Base *>::iterator it = m_psList.begin();
	while(it != m_psList.end())
	{
		PS_Base *ps = *it;
		it = m_psList.erase(it);
		delete ps;
	}
}



void ParticleSystemsRenderer :: render( Graphics& g ) const
{
	// render particle systems
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	for(std::vector<PS_Base *>::const_iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		(*it)->render();
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticleSystemsRenderer :: update( float dt )
{
	std::vector<PS_Base *>::iterator it = m_psList.begin();
	// Do particle system updates
	//for(std::vector<PS_Base *>::iterator it = m_psList.begin();
	//	it != m_psList.end();
	//	++it)
	while(it != m_psList.end())
	{
		(*it)->update(dt);

		//remove dead particle systems
		if ((*it)->getCurrentTime() > (*it)->getSystemLife()) { 
			PS_Base *ps = *it;
			it = m_psList.erase(it);
			delete ps;
		} else
			it++;
	}
}

void ParticleSystemsRenderer::onEvent(Key_GoingDown &key) {
	// FIXME : Adding the hacky way the dummy ps
	int keyPressed = key.getValue();

	CoordinateFrame cf;

	switch (keyPressed) {
		case 'Q':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'E':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnergyLoss"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'R':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'T':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Fountain"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'Y':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnemyExplosion"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
	}
}

void ParticleSystemsRenderer::onEvent(Enemy_Despawned &enemy) {
	
	CoordinateFrame cf = enemy.getValue()->getCoordinateFrame();
	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnemyExplosion"));
	m_psList.back()->setTransform(cf);
}
