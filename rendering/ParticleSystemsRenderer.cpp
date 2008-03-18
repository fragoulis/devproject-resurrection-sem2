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
#include "../GameLogic/Objects/Playership.h"
#include "../ParticleSystem/PS_Jet.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{

	EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	EventManager::instance().registerEventListener<Enemy_Despawned>(this);
	EventManager::instance().registerEventListener<Player_EnergyDrained>(this);
	EventManager::instance().registerEventListener<Player_Spawned>(this);

	
}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{

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

//LISTENING THE KEYBOARD FOR DEBUGGING PURPOSES ONLY
void ParticleSystemsRenderer::onEvent(Key_GoingDown &key) {
	//FIXME : Adding the hacky way the dummy ps
	int keyPressed = key.getValue();

	CoordinateFrame cf;

	switch (keyPressed) {
		case 'Q':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'E':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
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
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
			cf.move(Vector3(64,1450,-64));
			m_psList.back()->setTransform(cf);
			break;
	}
}

void ParticleSystemsRenderer::onEvent(Enemy_Despawned &enemy) {

	CoordinateFrame cf = enemy.getValue()->getCoordinateFrame();
	EnergyType energyType = enemy.getValue()->getEnergyType();
	//depending on the enemy energy type generates a different explosion
	if (energyType == EnergyTypeFromString("red"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
	else if (energyType == EnergyTypeFromString("yellow"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
	else if (energyType == EnergyTypeFromString("blue"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));

	m_psList.back()->setTransform(cf);
}

void ParticleSystemsRenderer::onEvent(Player_EnergyDrained& playerEnergy) {

	CoordinateFrame cf = playerEnergy.getValue1()->getCoordinateFrame(); 
	EnergyType energyType = playerEnergy.getValue2();
	//depending on the drained energy type generates a different effect
	if (energyType == EnergyTypeFromString("red"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss"));
	else if (energyType == EnergyTypeFromString("yellow"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
	else if (energyType == EnergyTypeFromString("blue"))
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss"));

	m_psList.back()->setTransform(cf);

}

void ParticleSystemsRenderer::onEvent(Player_Spawned &player)
{
	PS_Jet *ps_jet = (PS_Jet*) PS_Manager::instance().fetchNewPS("PS_Jet");
	ps_jet->setEmitterShip(player.getValue());
	m_psList.push_back(ps_jet);
}
